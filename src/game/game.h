#pragma once
#include "config/config.h"
#include "leds/leds.h"
#include "buttons/buttons.h"
#include "buzzer/buzzer.h"
#include "lcd/liquidDysplay2004.h"

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

    int team1Button = 23;
    int team2Button = 19;

    bool newDataAvailable = true;
    bool changeTeam = false;
    Buttons buttons = Buttons(team1Button, team2Button, "NC", "team1", "team2");
    LEDs leds = LEDs(22, 22, 27, 14);
    Buzzer buzzer = Buzzer(18);
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
    bool capturing(String teamName, uint8_t teamButton, uint8_t opponentButton, int &teamPoints, int &opponentPoints, bool isNeutralizing);
    bool teamIsCapturingZone(bool isNeutralizing = false);
    void isCaptured();
};