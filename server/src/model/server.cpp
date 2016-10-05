#include "server.h"

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "ClientConnection.h"
#include "../utils/Logger.h"

using namespace std;

Server::Server(string path) {
    this->userloader = new UserLoader(path);
}

Server::~Server() {
    this->shutdownServer();
}

/* Función para el thread de comunicación con el cliente
 * Manda los mensajes que se ingresen por cin()
 */
void client_comm(Server *srv, int client) {
    /* Mensaje de bienvenida. Se manda una vez fijo */

    SocketUtils sockutils;
    /* Recibo user y pass del cliente */
    char user[20];
    char pass[20];
    recv(client, user, 20, 0); /* Nombre de usuario */
    recv(client, pass, 20, 0); /* Contrasena */
    srv->connect_user(user);

    /* Le informo al cliente que se logueo ok */
    struct event resp;
    resp.data.code = EventCode::LOGIN_OK;
    sockutils.writeSocket(client, resp);

    /* Le empiezo a mandar los datos de la partida */
    XMLData xmldata = srv->load_xml(); /* Se carga el XML o se da el que ya tiene cargado */
    ClientConnection* handler = new ClientConnection(client, srv, user);
    srv->add_connection(handler); /* El clientconnection se podría crear dentro de add_connection */
    handler->start();

    /* Mando la configuración del juego desde el XML al usuario */
    Event* gameconfig = new Event(xmldata); /* Cargo los datos del XML en el mensaje */
    EventUtils eventutils; /* Para partir el xml grande en pedazos */
    vector<struct event> gameconfig_pedacitos = eventutils.buildEvents(gameconfig); /* armo el vector de pedazos */
    /* Le digo al handler que los mande */
    for (auto req : gameconfig_pedacitos) {
        handler->push_event(req);
    }

    if (srv->auth_user(user, pass)) {
        struct event resp;
        resp.data.code = EventCode::LOGIN_OK;

        sockutils.writeSocket(client, resp);

        /* Envio de lista de usuarios */
        Event* usersListMsg = new Event();
        usersListMsg->setContent((srv->getUserLoader())->getUsersList());
        // MessageUtils messageUtils;
        // vector<struct event> requests =  messageUtils.buildRequests(usersListMsg, EventCode:: USERS_LIST_MSG);
        vector<struct event> requests;

        ClientConnection* handler = new ClientConnection(client, srv, user);

        handler->start();
        for (auto req : requests) {
            handler->push_event(req);
        }

    } else {
        struct event resp;
        resp.data.code = EventCode::LOGIN_FAIL;
        sockutils.writeSocket(client, resp);
    }
    /* Esto crea un nuevo objeto ClientConnection que
     * se comunicará con el cliente en cuestión. Le paso el fd */
}

bool Server::auth_user(char *user, char *pass) {
    userloader->isPasswordOk(user, pass);
}

void Server::add_connection(ClientConnection *handler) {
    /* No usar nunca más el puntero pelado luego de esta
       llamada a emplace_back */
    connections.emplace_back(handler);
}

int Server::get_listen_socket() {
    return listen_socket_fd;
}

int Server::initialize_server(string ip, int port) {
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    /* Creo el primer socket
     * AF_INET indica IPv4
     * SOCK_STREAM indica tipo de socket de stream binario
     * IPPROTO_TCP indica TCP
     * socket() devuelve un file descriptor (int) que se refiere a
     * el socket asignado por el sistema operativo. El servidor todavia
     * no esta conectado al socket
    */
    listen_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket_fd < 0) {
        cout << "\nError en la creacion del socket" << endl;
        exit(1);
    }
    /* Configuro los datos del servidor
     * Para enviarlos al socket
    */
    server_addr.sin_family = AF_INET;
    /* inet_addr() transforma el string en el unsigned long que espera s_addr */
    server_addr.sin_addr.s_addr = inet_addr(ip.data());
    /* htons() devuelve el tipo de dato necesario para sin_port (unsigned int)*/
    server_addr.sin_port = htons(port);
    addr_size = sizeof(server_addr);

    /* Asigno el nombre al socket usando bind():
     * es decir, le asigno una direccion
    */
    if (bind(listen_socket_fd, (struct sockaddr *) &server_addr, addr_size) < 0) {
        string error = strerror(errno);
        LOGGER_WRITE(Logger::ERROR, "Hubo un error al hacer el binding del socket: " + error, "Server.class")

        cout << "Hubo un error al hacer el binding del socket: " << error << endl;
        cout << "Cerrando..." << endl;
        exit(1);
    }
    return listen_socket_fd;
}

void Server::start_listening() {
    listen(listen_socket_fd, MAX_CONN);
}

void Server::shutdownServer() {
    LOGGER_WRITE(Logger::INFO, "Apagando Server", "Server.class")
    delete userloader;
    close_all_connections();
    shutdown(listen_socket_fd, 2);
    LOGGER_WRITE(Logger::INFO, "El server se a apagado", "Server.class")
}

void Server::accept_incoming_connections() {
    int client_id;
    struct sockaddr_in client_addr;
    socklen_t caddr_size;

    /* accept() devuelve un file descriptor asociado a la conexión con el cliente
        * y sólo a el */
    client_id = accept(listen_socket_fd, (struct sockaddr *) &client_addr,
                       &caddr_size);
    if (shouldClose) {
        cout << "Servidor cerrado" << endl;
        exit(1);
    }

    if (client_id < 0 and !shouldClose) {
        string error = strerror(errno);
        LOGGER_WRITE(Logger::INFO, "Hubo un error al conectar con el cliente: " + error, "Server.class")

        cout << "Hubo un error al conectar con el cliente: " << error << endl;
        cout << "Cerrando..." << endl;
        exit(1);
    }

    if (!shouldClose) {
        LOGGER_WRITE(Logger::INFO, "Ingresando cliente numero " + to_string(client_id), "Server.class")

        cout << "Ingresando cliente numero " << client_id << endl;
        client_comm(this, client_id);
        client_id++;
    }
}

int Server::close_connection(char *username) {
    /* responsabilidad de connectionHandler?
    * el es el dueño del socket después de todo
    */
    for (unsigned int i = 0; i < connections.size(); ++i) {
        if (strcmp(connections[i]->getUsername(), username) == 0) {
            connections[i]->stop();
            connections.erase(connections.begin() + i);
            break;
        }
    }
    return 0;
}

void Server::close_all_connections() {
    for (unsigned int i = 0; i < connections.size(); ++i) {
        close_connection(connections[i]->getUsername());
    }
}

vector<shared_ptr<ClientConnection> > Server::get_connections() {
    return connections;
}

// void filter_and_send(Server *server, const char *requester, shared_ptr<ClientConnection> handler) {
//     LOGGER_WRITE(Logger::INFO, "Filtrando mensajes de cliente " + string(requester), "Server.class")
//     auto realmessages = server->get_messages_of(requester);
//     MessageUtils messageutils;
//     for (auto message : realmessages) {
//         vector<struct event> requests = messageutils.buildRequests(message, EventCode::CLIENT_RECEIVE_MSGS);
//         for (auto req : requests) {
//             handler->push_event(req);
//         }
//     }
//     Message *lastmsgmsg = new Message("server", "user", "Ud. no tiene mas mensajes");
//     struct event lastMsg = messageutils.buildRequests(lastmsgmsg, EventCode::LAST_MESSAGE).front();
//     handler->push_event(lastMsg);
// }

void Server::handle_message(Event *message, EventCode code, char* username) {
    shared_ptr<ClientConnection> handler;
    /* Me llega un Evento y me tengo que fijar de quién viene */
    /* Tengo que pasarle al escenario el evento y el nombre de usuario que lo mandó */

    switch(code) {
    case EventCode::CLIENT_SEND_MSG:
        cout << "CLIENT_SEND_MSG" << endl;
        store_message(message);
        break;

    case EventCode::CLIENT_DISCONNECT:
        cout << "CLIENT_DISCONNECT" << endl;
        handler = this->get_user_handler(message->getFrom().data());
        close_connection(handler->getUsername());
        break;

    default:
        this->incoming_events.push(message);
        break;
    }
}

shared_ptr<ClientConnection> Server::get_user_handler(const char *username) {
    for (auto user : this->connections) {
        if (strcmp(user->getUsername(), username) == 0) {
            return user;
        }
    }
    return nullptr;
}

void Server::store_message(Event *message) {
    LOGGER_WRITE(Logger::INFO, "Guardando mensaje de " + message->getFrom() + " para " + message->getTo(),
                 "Server.class")
    cout << "Guardando el mensaje: " << message->getContent() << endl;
    msglist_mutex.lock();
    messagesList.push_back(message);
    msglist_mutex.unlock();
}

void Server::shouldCloseFunc(bool should) {
    shouldClose = should;
}

list<Event *> Server::get_messages_of(const char *user) {
    std::list<Event *> messagesFiltered;
    msglist_mutex.lock();
    for (auto it = messagesList.begin(); it != messagesList.cend();) {
        if (strcmp((*it)->getTo().data(), user) == 0) {
            messagesFiltered.push_back(*it);
            it = messagesFiltered.erase(it); // ...
        } else {
            it++;
        }
    }
    msglist_mutex.unlock();
    return messagesFiltered;
}
/* Si  necesito acceso aleatorio, uso vector
pero si necesito recorrer de principio a fin o voy borrando/insertando
elementos en el medio, uso list */

UserLoader* Server::getUserLoader() {
    return this->userloader;
}
