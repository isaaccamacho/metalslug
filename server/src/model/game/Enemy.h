
#ifndef SERVER_ENEMY_H
#define SERVER_ENEMY_H

#include "../../utils/Protocol.h"
#include "AnimatedObject.h"


class Enemy : public AnimatedObject {
private:
    int posAtJump;
    int gravity;
    int jumpPos[25] = {0, 23, 44, 63, 80, 95, 108, 119, 128, 135, 140, 143, 144, 143, 140, 135, 128, 119, 108, 95, 80,
                       63, 44, 23, 0};
    int valorEnPuntos;
    vector <Entity> fightAgainst = {MARCO, TARMA, FIO, ERI}; // Si, es horrible tener esto aca.. pero es lo mas rapido de codear :P

    GameObject *findCloserPlayerToFollow(vector<GameObject *> gameObjects);

public:
    Enemy(Entity enemySelected, int spawnX, int spawnY);

    virtual ~Enemy();

    virtual event getState() override;

    bool isJumping();

    void avanzar();

    int retroceder();

    void updatePosition(vector<GameObject *> game_objects);

    void set_position(int posx, int posy);

    bool canIMove(vector<GameObject *> game_objects, int newX, int newY);


};


#endif //SERVER_ENEMY_H
