#ifndef CONFIG_H
#define CONFIG_H

#define NFC_SDA_PIN 21
#define NFC_SCL_PIN 22
#define NFC_RST_PIN 5


#define LED_RED_PIN 15
#define LED_GREEN_PIN 2
#define LED_BLUE_PIN 4

// Pin para definir modo de operação (self-host)
// HIGH = Access Point (self-host) | LOW = Client (conectar a rede existente)
#define SELF_HOST_PIN 25

#define CAPTURE_ZONE 5000
#define NEUTRALIZE_ZONE 10000


#define MAX_TEAMS 2


#define WIFI_SSID "MontainWolves_Domination"
#define WIFI_PASSWORD "Domination2026"


#endif