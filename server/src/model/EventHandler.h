

#ifndef SERVER_EVENTHANDLER_H
#define SERVER_EVENTHANDLER_H


class EventHandler {

public:
    virtual void handle(EventCode eventCode, GameObject *gameObject)= 0;
};


#endif //SERVER_EVENTHANDLER_H
