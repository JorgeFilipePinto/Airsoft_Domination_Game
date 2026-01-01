#include "config/config.h"
#include "leds/leds.h"
#include "buttons/buttons.h"
#include "buzzer/buzzer.h"
#include "lcd/liquidDysplay2004.h"
#pragma once

enum GameState
{
    IDLE,
    CAPTURING,
    NEUTRALIZED,
    CAPTURED1,
    CAPTURED2
};

class Game
{
public:
    GameState currentGameState = IDLE;
    int pointsTeam1;
    int pointsTeam2;
    int lastPointsTeam1 = 0;
    int lastPointsTeam2 = 0;

    int team1Button = 36;
    int team2Button = 39;

    bool newDataAvailable = true;
    Buttons buttons = Buttons(team1Button, team2Button, "NO", "team1", "team2");
    LEDs leds = LEDs(12, 13, 14);
    Buzzer buzzer = Buzzer(15);
    LiquidDysplay2004 lcd;
    void init();
    static void start(void *parameter);

private:
    static Game *instance;
    GameState lastCurrentGameState = IDLE;
    bool team1InZone = false;
    bool team2InZone = false;


    void printGameState();
    void printPoints(String teamName);
    void loop();
    bool capturing(String teamName, uint8_t teamButton, int &teamPoints, int &opponentPoints, bool isNeutralizing = false);
    bool teamIsCapturingZone(bool isNeutralizing = false);
    void isCaptured();
};