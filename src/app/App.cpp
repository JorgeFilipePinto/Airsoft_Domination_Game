#include "App.h"
#include "wifi/OTA/OTA.h"
#include "game/game.h"

Game game = Game();

void app_update(void)
{
    xTaskCreatePinnedToCore(
        updateIsEnable,
        "WiFi OTA Update",
        8192,
        NULL,
        1,
        NULL,
        0
    );
}

void app_init(void)
{
    Serial.begin(115200);
    delay(250);
    game.init();
}

void app_start(void)
{
    xTaskCreatePinnedToCore(
        Game::start, 
        "Game",
        4096,
        NULL,
        1,
        NULL,
        1
    );
}