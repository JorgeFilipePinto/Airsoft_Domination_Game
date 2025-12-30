#include "App.h"
#include "wifi/OTA/OTA.h"
#include "game/game.h"

Game game = Game();

void app_update(void)
{
    xTaskCreatePinnedToCore(
        updateIsEnable,    // Function that implements the task.
        "WiFi OTA Update", // Text name for the task.
        4096,              // Stack size in words, not bytes.
        NULL,              // Parameter passed into the task.
        1,                 // Priority at which the task is created.
        NULL,              // Task handle to keep track of created task.
        0                  // Core where the task should run (0 or 1).
    );
}


void app_init(void)
{
    Serial.begin(115200);
    game.init();
}

void app_start(void)
{
        xTaskCreatePinnedToCore(
        Game::start,    // Function that implements the task.
        "Game", // Text name for the task.
        4096,              // Stack size in words, not bytes.
        NULL,              // Parameter passed into the task.
        1,                 // Priority at which the task is created.
        NULL,              // Task handle to keep track of created task.
        1                  // Core where the task should run (0 or 1).
    );
}