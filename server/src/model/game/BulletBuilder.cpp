
#include "BulletBuilder.h"
#include "NormalBulletMovementStrategy.h"
#include "TeledirBulletMovementStrategy.h"
#include "NotMoveBulletStrategy.h"

Bullet *BulletBuilder::createBullet(Entity bulletType, AnimatedObject *firedBy) {
    Bullet *bullet = nullptr;
    Vector *vector = firedBy->getBulletdirection();
    if (vector->getX() == 0 && vector->getY() == 0) return nullptr;
    switch (bulletType) {
        case Entity::BT_HEAVY_BULLET:
            bullet = new Bullet(bulletType, firedBy->getX() + firedBy->getBoxAncho(),
                                firedBy->getY() + firedBy->getBoxAlto() / 2,
                                firedBy->getDireccionX(),
                                firedBy->getDireccionY(), firedBy->getShootsTo(),
                                new NormalBulletMovementStrategy());
            setupBullet(bullet, 20, 20, 100, 60, vector->getX(), vector->getY(), firedBy->getShootsTo());
            break;
        case Entity::BT_MISSILE:
            bullet = new Bullet(bulletType, firedBy->getX() + firedBy->getBoxAncho(),
                                firedBy->getY() + firedBy->getBoxAlto() / 2,
                                firedBy->getDireccionX(),
                                firedBy->getDireccionY(), firedBy->getShootsTo(),
                                new NormalBulletMovementStrategy());
            setupBullet(bullet, 20, 20, 100, 30, vector->getX(), vector->getY(), firedBy->getShootsTo());
            break;
        case Entity::BT_TELE_MISSILE:
            bullet = new Bullet(bulletType, firedBy->getX() + firedBy->getBoxAncho(),
                                firedBy->getY() + firedBy->getBoxAlto() / 2,
                                firedBy->getDireccionX(),
                                firedBy->getDireccionY(), firedBy->getShootsTo(),
                                new TeledirBulletMovementStrategy());
            setupBullet(bullet, 20, 20, 100, 10, vector->getX(), vector->getY(), firedBy->getShootsTo());
            break;
        case Entity::BT_SHOT:
            bullet = new Bullet(bulletType, firedBy->getX() + firedBy->getBoxAncho(),
                                firedBy->getY() + firedBy->getBoxAlto() / 2,
                                firedBy->getDireccionX(),
                                firedBy->getDireccionY(), firedBy->getShootsTo(),
                                new NotMoveBulletStrategy());
            setupBullet(bullet, 20, 20, 200, 20, vector->getX(), vector->getY(), firedBy->getShootsTo());
            break;
        default:
            bullet = new Bullet(bulletType, firedBy->getX() + firedBy->getBoxAncho(),
                                firedBy->getY() + firedBy->getBoxAlto() / 2,
                                firedBy->getDireccionX(),
                                firedBy->getDireccionY(), firedBy->getShootsTo(),
                                new NormalBulletMovementStrategy());

            setupBullet(bullet, 20, 20, 100, 50, vector->getX(), vector->getY(), firedBy->getShootsTo());
            break;
    }
    return bullet;
}

void BulletBuilder::setupBullet(Bullet *bullet, int boxAlto, int boxAncho, int damage, int speed, int directionX,
                                int directionY, vector<Entity> &collisionables) {
    bullet->setBoxAlto(boxAlto);
    bullet->setBoxAncho(boxAncho);
    bullet->setDamage(damage);
    bullet->setSpeed(speed);
    bullet->setDireccionX(directionX);
    bullet->setDireccionY(directionY);
    bullet->setColisionables(collisionables);
}