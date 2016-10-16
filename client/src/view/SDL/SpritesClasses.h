#ifndef SDLBASE_SPRITE_H
#define SDLBASE_SPRITE_H



using namespace std;
#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "../../utils/Protocol.h"

class Sprite {
protected:
    SDL_Rect sourceRect; //rect donde se dibuja el sprite
    SDL_Rect destRect;   //rect donde se hospeda el sprite anterior, este define la pos y tamanio

    SDL_Texture* layer;
    SDL_Renderer* renderer;

    int spriteImageWidth,spriteImageHeight;

    int frameWidth, frameHeight, window_height, window_width;

public:
    //CONSTRUCTOR
    Sprite(SDL_Texture *layer, SDL_Renderer *renderer, int window_width, int window_height);
    SDL_Texture* loadTexture(SDL_Renderer* renderer,string imageTexturePath);
    void setUpImage(string imageSpritePath);
//_________________________________________________________________________________________________________
    //ACTUALIZACION DEL SPRITE
    void handle(struct event nuevoEvento);
    void actualizarDibujo();
//_________________________________________________________________________________________________________
    //TAMANO DEL SPRITE:
    void setWidth(int w){ Sprite::destRect.w = w;}
    void setHeight(int h){ Sprite::destRect.h = h;}
//_________________________________________________________________________________________________________
    //POSICION DEL SPRITE:
    void set_position(int x, int y);
    int getXPosition(){ return Sprite::destRect.x;}
    int getYPosition(){ return Sprite::destRect.y;}
//________________________________________________________________________________________________________

    SDL_Texture* getLayer() const { return layer;}
//_________________________________________________________________________________________________________

};

//______________________________________________________________________________________________
//PLAYERSPRITE
class PlayerSprite : public Sprite{

private:
    int wFramesCant,wActualPosFrame;
public:
//_______________________________________________________________________________________________
    //Constructor:

    PlayerSprite(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height) : Sprite(texture, renderer,window_width,window_height) {
//        TODO: este seteo se hace a partir del XML
        //DEFAULT SIZE
        PlayerSprite::setHeight(100);
        PlayerSprite::setWidth(100);
        //DEFAULT POSITION
        PlayerSprite::set_position(10,550);
        caminandoDerecha(); //TODO: CUANDO ESTEN LOS SPRITES DE MARCO COMPLETO BORRAR ESTA LINEA Y DESCOMENTAR LA SIGUIENTE
//        mirandoDerechaQuieto();
    }

//________________________________________________________________
    //OVERRIDE FUNCTIONS:

    void setUpImage(string imageSpritePath, int wFramesCant, int hFramesCant);


//__________________________________________________________________

    void handle(struct event nuevoEvento);

    void setNextSpriteFrame();
    void caminandoIzquierda();
    void mirandoArribaCaminandoIzquierda();
    void agachadoMirandoAIzquierdaQuieto();
    void mirandoArribaIzquierdaQuieto();
    void caminandoDerecha();
    void mirandoArribaCaminandoDerecha();
    void agachadoMirandoDerechaQuieto();
    void mirandoArribaDerechaQuieto();
    void agachadoAvanzandoAIzquierda();
    void agachadoAvanzandoADerecha();
    void mirandoDerechaQuieto();
    void mirandoIzquierdaQuieto();

};
//______________________________________________________________________________________________
//BACKGROUNDSPRITE
class BackgroundSprite : public Sprite{
private:
    int spriteImageWidthPanned; //es la posicion horizontal que se va a a ir actualizando

public:
    BackgroundSprite(SDL_Texture* texture, SDL_Renderer* renderer, int window_width,int window_height) :
            Sprite(texture, renderer,window_width,window_height) {

        BackgroundSprite::set_position(0,0);
        BackgroundSprite::spriteImageWidthPanned=0;

        BackgroundSprite::frameWidth = window_width;
        BackgroundSprite::frameHeight = window_height;

        BackgroundSprite::setWidth(window_width);
        BackgroundSprite::setHeight(window_height);

        BackgroundSprite::sourceRect.w = window_width;
        BackgroundSprite::sourceRect.h = window_height;
    }

//_______________________________________________________________________________________________

    void setUpImage(string imageSpritePath);

    void handle(struct event nuevoEvento);

    void setFramePosition(int x);
};
//_______________________________________________________________________________________________
//MARCO
class Marco : public PlayerSprite {
public:
    Marco(SDL_Texture *texture,SDL_Renderer *renderer, int window_width, int window_height) : PlayerSprite(texture,renderer,window_width,window_height) {
//        setUpImage("sprites/marco.png",12,12);//TODO: CUANDO SANTIAGO TERMINE SPRITES DESCOMENTAR Y BORRAR LA SIGUIENTE LINEA
        setUpImage("sprites/marco.png",12,10);
    }
    void setUpImage(string imageSpritePath, int wFramesCant, int hFramesCant){PlayerSprite::setUpImage(imageSpritePath,wFramesCant,hFramesCant);}
};
//_______________________________________________________________________________________________
//TARMA
class Tarma : public PlayerSprite{
public:
    Tarma(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height) : PlayerSprite(texture,renderer,window_width,window_height){
        setUpImage("sprites/tarma.png",12,12);
    }
};
//_______________________________________________________________________________________________
//FIO
class Fio : public PlayerSprite{
public:
    Fio(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height) : PlayerSprite(texture,renderer,window_width,window_height){
        setUpImage("sprites/tarma.png",12,12); //TODO: cuando este el png listo, setearlo
    }
};
//_______________________________________________________________________________________________
//ERI
class Eri : public PlayerSprite{
public:
    Eri(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height) : PlayerSprite(texture,renderer,window_width,window_height){
        setUpImage("sprites/tarma.png",12,12); //TODO: cuando este el png listo, setearlo
    }
};




#endif //SDLBASE_SPRITE_H