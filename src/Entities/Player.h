#ifndef BASIC_SERVER_ENTITIES_PLAYER_H
#define BASIC_SERVER_ENTITIES_PLAYER_H

#include <list>

#include "Character.h"

class Client;

class Player: public Character
{
public:
    Player(std::string name, Client* client);
    virtual ~Player();

    void UpdateLoS(TypeObjectsMap newObjectsInSight);

private:
    
};

#endif