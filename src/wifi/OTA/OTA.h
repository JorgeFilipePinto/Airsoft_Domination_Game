#pragma once
#include <Arduino.h>
#include "config/config.h"
#include "wifi/OTA/htmlPages/mainPage.h"
#include "wifi/OTA/htmlPages/uploadPage.h"
#ifdef ESP32
#include <Update.h>
#include <WiFi.h>
#include <WebServer.h>
#elif defined(ESP8266)
#include <Updater.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

void updateIsEnable(void *parameter);

class WebServerModule
{
public:
    WebServerModule();
    void begin();
    void handleClient();
    bool hasUploadStarted() const { return uploadStarted; }
    void resetUploadFlag() { uploadStarted = false; }

private:
#ifdef ESP32
    WebServer server;
#elif defined(ESP8266)
    ESP8266WebServer server;
#endif
    bool apMode;
    bool uploadStarted;

    void wifiConfig();
    void accessPointConfig();
    void serverConfig();

    // Métodos para callbacks (têm acesso aos membros da classe)
    void handleRoot();
    void handleUpdate();
    void handleUpdateEnd();

    // Funções auxiliares
    String getUpTime();
    String getIP();

    // Funções estáticas para callbacks do WebServer
    static void handleRootStatic();
    static void handleUpdateStatic();
    static void handleUpdateEndStatic();

    // Ponteiro estático para a instância atual
    static WebServerModule *instance;
};