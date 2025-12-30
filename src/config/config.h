#pragma once

//============================ UPDATE CONFIGURATIONS ============================//
static constexpr bool WIFI_UPDATE_ENABLED = true;
static constexpr const char WIFI_ATTEMPTS = 20;
static constexpr const unsigned long UPLOAD_TIMEOUT_MS = 300000; // 5 minutos em milissegundos
static constexpr const char *WIFI_SSID = "MEO-9E52D0";
static constexpr const char *WIFI_PASSWORD = "Jorge19955991";

static constexpr const char *AP_SSID = "ESP32-RFID";
static constexpr const char *AP_PASSWORD = "12345678";

static constexpr const int WEB_SERVER_PORT = 80;
static constexpr const char *WEB_USER = "admin";
static constexpr const char *WEB_PASS = "admin";

//===================================================================Game timings===================================================================
#define CAPTURE_ZONE 5000
#define NEUTRALIZE_ZONE 10000
