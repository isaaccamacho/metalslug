#include <iostream>

//TODO: EN EL MAKEFILE AGREGAR LOS .cpp PARA QUE ESTOS SEAN .h
#include "SDL/InitialWindow.cpp"
#include "SDL/SDLRunningGame.cpp"
using namespace std;


int main(int argc, char *argv[]){

//____________________________________________________________________________________________
    bool running = true;

    InitialWindow* initialWindow = new InitialWindow();

    SDLRunningGame* sdlRunningGame = new SDLRunningGame(initialWindow->getMainWindow(),initialWindow->getMainRenderer());

//    Uint32  starting_tick;
//    SDL_Event event;
//

    SDL_Event event;
    while(running){
        while (SDL_PollEvent( &event )) {

            if ( event.type == SDL_QUIT) {
                running = false;
                break;
            }

            sdlRunningGame->eventsHandler(&event); //El eventsHandler envia los mensajes al Server
            sdlRunningGame->updateWindowSprites(); //TODO: EL updateWindow LO DEBE HACER DESP DE RECIBIR LOS MENSAJES DEL SERVER
        }

        SDL_Delay(100); //antes de procesar otro evento esta pausando 2 milisegundos
    }


//    while( running ){
//
//____________________________________________________________________________________________
//
//        backgroundSprite0->increaseFrameTime();
//
//        if(fps / (backgroundSprite0->getFrameTime()) == 10){
//            backgroundSprite0->setNextSpriteFrame();
//            backgroundSprite0->restartFrameTime();
//
//            if(cloud->getXPosition()==window_width){
//                cloud->set_position(0,cloud->getYPosition());
//            }
//            cloud->set_position(cloud->getXPosition()+1,cloud->getYPosition());
//        }
//
//____________________________________________________________________________________________
//
//
//        starting_tick = SDL_GetTicks();
//
//
//        while (SDL_PollEvent( &event )){
//            if ( event.type == SDL_QUIT) {
//                running = false;
//                break;
//            }
//
//
//
//____________________________________________________________________________________________
//
//            player->increaseFrameTime();
//
//____________________________________________________________________________________________
//
//            if(fps / player->getFrameTime() == 30 ){
//
//                player->handlePlayerEvents(event,backgroundSprite1);
//                player->restartFrameTime();
//
//
//            }
//____________________________________________________________________________________________
//        }
//
//        cap_framerate(starting_tick);  //maneja los delays del loop que actualiza la salida de todo el SDL
//
//
//        SDL_RenderClear(mainRenderer);
//
//        backgroundSprite0->update();
//        backgroundSprite1->update();
//        player->update();
//        cloud->update();
//
//        SDL_RenderPresent(mainRenderer);
//
//    }
//____________________________________________________________________________________________

    //Liberar todo

//    SDL_DestroyTexture(backgroundLayer0);
//    SDL_DestroyTexture(backgroundLayer1);
//    SDL_DestroyTexture(layerCloud);
//    SDL_DestroyTexture(layerPlayer);
//    SDL_DestroyRenderer(mainRenderer);
//    freeMemorySoundUsed(sound);
//    Mix_CloseAudio();   //Shutdown and cleanup the mixer API
//____________________________________________________________________________________________
//    SDL_DestroyWindow(mainWindow);
//    SDL_Quit( );


//    ~sdlRunningGame();
    return 0;
}
