#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

class Entity;

class Player
{
public:
    Player();

    Entity* createPlayerEntity();
protected:
    Entity* m_playerEntity;
};

#endif // PLAYER_H
