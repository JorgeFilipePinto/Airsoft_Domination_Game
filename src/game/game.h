#include "config/config.h"
#include "leds/leds.h"
#include "buttons/buttons.h"
#include "buzzer/buzzer.h"
#include "lcd/liquidDysplay2004.h"
#pragma once

enum GameState
{
    IDLE,
    CAPTURING1,
    CAPTURING2,
    NEUTRALIZING1,
    NEUTRALIZING2,
    CAPTURED2,
    CAPTURED1,
    FINISHED
};

class Game
{
public:
    GameState currentGameState = IDLE;
    int pointsTeam1;
    int pointsTeam2;
    int team1Button = 25;
    int team2Button = 26;
    Buttons buttons = Buttons(team1Button, team2Button, "NC", "team1", "team2");
    LEDs leds = LEDs(12, 13, 14);
    Buzzer buzzer = Buzzer(15);
    LiquidDysplay2004 lcd;
    void init();
    static void start(void *parameter);

private:
    static Game *instance;

    bool team1InZone = false;
    bool team2InZone = false;

    void loop();
    bool isCapturing();
    void teamIsCapturingZone();
    bool isNeutralizing();
    void isCaptured();
};