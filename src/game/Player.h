#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "Team.h"

struct Player {
    uint32_t nfcId;
    Team team;
    char name[32];
    bool isActive;
    int score;
};


#endif