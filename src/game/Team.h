#ifndef TEAM_H
#define TEAM_H

#include <Arduino.h>

enum class Team {
    TEAM_RED,
    TEAM_BLUE,
    NEUTRAL
};

struct TeamColor {
    uint8_t r, g, b;
};

const TeamColor TEAM_COLORS[] = {
    {255, 0, 0},   // TEAM_RED
    {0, 0, 255},   // TEAM_BLUE
    {128, 128, 128} // NEUTRAL
};

#endif