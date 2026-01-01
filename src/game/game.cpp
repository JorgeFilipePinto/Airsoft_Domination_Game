#include "game.h"
#include "lcd/liquidDysplay2004.h"

Game *Game::instance = nullptr;

void Game::init()
{
    Serial.println("[Game::init] Iniciando...");
    instance = this;
    currentGameState = IDLE;
    pointsTeam1 = 0;
    pointsTeam2 = 0;

    MessageStruct welcomeMessages[] = {
        {"Montain Wolves", 2, 0},
        {"Airsoft Domination", 1, 1},
        {"   Jogo Pronto   ", 1, 2},
        {"  Boa Sorte! ;)  ", 1, 3}};

    Serial.println("[Game::init] Inicializando LCD...");
    lcd.init();
    Serial.println("[Game::init] LCD OK");

    Serial.println("[Game::init] Inicializando buzzer...");
    buzzer.init();
    Serial.println("[Game::init] Buzzer OK");

    Serial.println("[Game::init] Inicializando buttons...");
    buttons.init();
    Serial.println("[Game::init] Buttons OK");

    Serial.println("[Game::init] Inicializando leds...");
    leds.init();
    Serial.println("[Game::init] LEDs OK");

    lcd.print(welcomeMessages, 4);
    Serial.println("[Game::init] Completo!");
    delay(3000);
    newDataAvailable = true;
}

void Game::start(void *parameter)
{
    if (instance == nullptr)
    {
        return;
    }

    Serial.println("Game task started.");
    while (true)
    {
        instance->loop();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void Game::loop()
{
    switch (currentGameState)
    {
    case IDLE:
    {
        MessageStruct idleMessages[] = {
            {"Montain Wolves", 3, 0},
            {"Domination", 4, 1},
            {"Press Blue and Red", 1, 2},
            {"Buttons", 6, 3}};
        if (newDataAvailable)
        {
            lcd.clear();
            newDataAvailable = false;
            lcd.print(idleMessages, 4);
        }
        if (buttons.isPressed(team1Button) && buttons.isPressed(team2Button))
        {
            currentGameState = NEUTRALIZED;
            lastCurrentGameState = NEUTRALIZED;
            newDataAvailable = true;
            Serial.println("A neutralizing state has started.");
        }
        break;
    }
    case NEUTRALIZED:
    {
        MessageStruct idleMessages[] = {
            {"Neutral", 6, 1},
            {"Team1: " + String(pointsTeam1), 0, 2},
            {"Team2: " + String(pointsTeam2), 0, 3}};
        if (newDataAvailable)
        {
            lcd.clearLine(1);
            lcd.clearLine(2);
            lcd.clearLine(3);
            newDataAvailable = false;
            lcd.print(idleMessages, 3);
        }

        if (buttons.isPressed(team1Button) && buttons.isPressed(team2Button))
        {
            break;
        }
        else if (buttons.isPressed(team1Button))
        {
            newDataAvailable = true;
            lastCurrentGameState = currentGameState;
            currentGameState = CAPTURING;
            Serial.println("Team 1 is capturing the zone.");
        }
        else if (buttons.isPressed(team2Button))
        {
            newDataAvailable = true;
            lastCurrentGameState = currentGameState;
            currentGameState = CAPTURING;
            Serial.println("Team 2 is capturing the zone.");
        }
        break;
    }
    case CAPTURING:
    {
        if (!teamIsCapturingZone())
        {
            newDataAvailable = true;
            currentGameState = lastCurrentGameState;
            Serial.println("Capturing aborted, returning to previous state.");
        }
        break;
    }
    case CAPTURED1:
    {
        if (newDataAvailable)
        {
            printPoints("Team 1");
            newDataAvailable = false;
        }
        if (buttons.isPressed(team2Button))
        {
            lastCurrentGameState = currentGameState;
            const bool captured = teamIsCapturingZone(true);
            if(!captured)
            {
                newDataAvailable = true;
            }
        }
        break;
    }
    case CAPTURED2:
    {
        if (newDataAvailable)
        {
            printPoints("Team 2");
            newDataAvailable = false;
        }
        if (buttons.isPressed(team1Button))
        {
            lastCurrentGameState = currentGameState;
            const bool captured = teamIsCapturingZone(true);
            if(!captured)
            {
                newDataAvailable = true;
            }
        }
        break;
    }
    }
    printGameState();
}


void Game::printGameState()
{
    if (pointsTeam1 != lastPointsTeam1)
    {
        lcd.clearLine(2);
        lastPointsTeam1 = pointsTeam1;
        lcd.printAt("Team1: " + String(pointsTeam1) + "   ", 0, 2);
    }
    if (pointsTeam2 != lastPointsTeam2)
    {
        lcd.clearLine(3);
        lastPointsTeam2 = pointsTeam2;
        lcd.printAt("Team2: " + String(pointsTeam2) + "   ", 0, 3);
    }
}


void Game::printPoints(String teamName)
{
    lastPointsTeam1 = pointsTeam1;
    lastPointsTeam2 = pointsTeam2;
    lcd.clearLine(1);
    lcd.clearLine(2);
    lcd.clearLine(3);

    lcd.printAt("Captured by " + teamName, 1, 1);
    lcd.printAt("Team1: " + String(pointsTeam1) + "   ", 0, 2);
    lcd.printAt("Team2: " + String(pointsTeam2) + "   ", 0, 3);
}


bool Game::teamIsCapturingZone(bool isNeutralizing)
{
    if (buttons.isPressed(team1Button) && buttons.isPressed(team2Button))
    {
        return false;
    }

    if (buttons.isPressed(team1Button))
    {
        newDataAvailable = true;
        const bool captured = capturing("Team 1", team1Button, pointsTeam1, pointsTeam2, isNeutralizing);
        if (captured)
        {
            (isNeutralizing ? currentGameState = NEUTRALIZED : currentGameState = CAPTURED1);
            newDataAvailable = true;
        }
        Serial.println("Team 1 is capturing...");
        return captured;
    }

    if (buttons.isPressed(team2Button))
    {
        newDataAvailable = true;
        const bool captured = capturing("Team 2", team2Button, pointsTeam2, pointsTeam1, isNeutralizing);
        if (captured)
        {
            (isNeutralizing ? currentGameState = NEUTRALIZED : currentGameState = CAPTURED2);
            newDataAvailable = true;
        }
        Serial.println("Team 2 is capturing...");
        return captured;
    }
    return false;
}


bool Game::capturing(String teamName, uint8_t teamButton, int &teamPoints, int &opponentPoints, bool isNeutralizing)
{
    if (newDataAvailable)
    {
        lcd.clearLine(1);
        lcd.clearLine(2);
        lcd.clearLine(3);
        lcd.printAt((isNeutralizing ? "Neutralizing" : teamName + " is capturing"), (isNeutralizing ? 3 : 1), 1);
        newDataAvailable = false;
    }

    Serial.println(teamName + " is capturing...");
    for (int i = 0; i < 20; i++)
    {
        for (int j = 1; j <= 5; j++)
        {
            if (!buttons.isPressed(teamButton))
            {
                if(lastCurrentGameState != NEUTRALIZED)
                {
                    (isNeutralizing ? teamPoints += 50 : opponentPoints += 50);
                }
                return false;
            }
            lcd.drawLoadingBar(i, j);
            delay(25);
        }
        delay(25);
    }
    (isNeutralizing ? teamPoints += 0 : teamPoints += 100);

    return true;
}


void Game::isCaptured()
{
    // Implementar lógica para verificar se a zona foi capturada
}