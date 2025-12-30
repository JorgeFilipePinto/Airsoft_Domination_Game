#include "game.h"

Game* Game::instance = nullptr;

void Game::init()
{
    instance = this;
    currentGameState = IDLE;
    pointsTeam1 = 0;
    pointsTeam2 = 0;
    buzzer.init();
    buttons.init();
    leds.init();
}


void Game::start(void *parameter)
{
    if(instance == nullptr)
    {
        return;
    }

    Serial.println("Game task started.");
    while(true)
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
        // Lógica para o estado IDLE
        break;
    }
    case CAPTURING1:
    {
        // Lógica para o estado CAPTURING1
        break;
    }
    case CAPTURING2:
    {
        // Lógica para o estado CAPTURING2
        break;
    }
    case NEUTRALIZING1:
    {
        // Lógica para o estado NEUTRALIZING1
        break;
    }
    case NEUTRALIZING2:
    {
        // Lógica para o estado NEUTRALIZING2
        break;
    }
    case CAPTURED1:
    {
        // Lógica para o estado CAPTURED1
        break;
    }
    case CAPTURED2:
    {
        // Lógica para o estado CAPTURED2
        break;
    }
    case FINISHED:
    {
        // Lógica para o estado FINISHED
        break;
    }
    default:
        break;
    }
}


bool Game::isCapturing()
{
    if(buttons.isPressed(team1Button) || buttons.isPressed(team2Button))
    {
        return true;
    }
    return false;
}


void Game::teamIsCapturingZone()
{
    if(buttons.isPressed(team1Button))
    {

    } else if (buttons.isPressed(team2Button))
    {

    }
}


bool Game::isNeutralizing()
{
    // Implementar lógica para verificar se uma equipa está a neutralizar
    return false;
}


void Game::isCaptured()
{
    // Implementar lógica para verificar se a zona foi capturada
}