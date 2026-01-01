#include "OTA.h"

void updateIsEnable(void *parameter)
{
    WebServerModule webServerModule;
    if (WIFI_UPDATE_ENABLED)
        webServerModule.begin();

    unsigned long startTime = millis();
    bool timeoutReached = false;

    while (WIFI_UPDATE_ENABLED)
    {
        webServerModule.handleClient();

        // Verifica timeout apenas se o upload ainda não foi iniciado
        if (!webServerModule.hasUploadStarted())
        {
            if (millis() - startTime >= UPLOAD_TIMEOUT_MS)
            {
                timeoutReached = true;
                Serial.println("\n⏱ Timeout de 5 minutos atingido. Nenhum upload iniciado.");
                Serial.println("🔄 Tarefa OTA será encerrada...");
                break;
            }
        }
        
        // Adicionar delay para evitar watchdog reset
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    if (timeoutReached || !WIFI_UPDATE_ENABLED)
    {
        Serial.println("✓ Tarefa OTA finalizada.");
        vTaskDelete(NULL); // Destrói a tarefa atual
    }
}

WebServerModule *WebServerModule::instance = nullptr;

WebServerModule::WebServerModule() : server(WEB_SERVER_PORT), apMode(false), uploadStarted(false)
{
    instance = this; // Guardar ponteiro para esta instância
}

void WebServerModule::begin()
{
    wifiConfig();
    serverConfig();
}

void WebServerModule::wifiConfig()
{
    Serial.print("A conectar ao WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_ATTEMPTS)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\n✓ WiFi conectado!");
        Serial.print("📱 Acede a: http://");
        Serial.println(WiFi.localIP());
        Serial.printf("🔐 User: %s | Pass: %s\n\n", WEB_USER, WEB_PASS);
    }
    else
    {
        accessPointConfig();
    }
}

void WebServerModule::accessPointConfig()
{
    Serial.println("\n⚠ Falha ao conectar ao WiFi!");
    Serial.println("📡 A criar Access Point próprio...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    apMode = true;

    Serial.println("\n✓ Access Point criado!");
    Serial.printf("📶 SSID: %s\n", AP_SSID);
    Serial.printf("🔑 Password: %s\n", AP_PASSWORD);
    Serial.print("📱 Acede a: http://");
    Serial.println(WiFi.softAPIP());
    Serial.printf("🔐 User: %s | Pass: %s\n\n", WEB_USER, WEB_PASS);
}

void WebServerModule::serverConfig()
{
    // Usar funções estáticas como callbacks
    server.on("/", HTTP_GET, handleRootStatic);
    server.on("/update", HTTP_POST, handleUpdateEndStatic, handleUpdateStatic);
    server.begin();

    Serial.println(F("Servidor Web iniciado!"));
}

void WebServerModule::handleClient()
{
    server.handleClient();
}

// ========== CALLBACKS ESTÁTICOS (ponte para métodos da instância) ==========
void WebServerModule::handleRootStatic()
{
    if (instance)
    {
        instance->handleRoot();
    }
}

void WebServerModule::handleUpdateStatic()
{
    if (instance)
    {
        instance->handleUpdate();
    }
}

void WebServerModule::handleUpdateEndStatic()
{
    if (instance)
    {
        instance->handleUpdateEnd();
    }
}

// ========== MÉTODOS DE INSTÂNCIA (têm acesso aos membros) ==========
void WebServerModule::handleRoot()
{
    if (!server.authenticate(WEB_USER, WEB_PASS))
    {
        return server.requestAuthentication();
    }

    String page = mainPage;
    page.replace("%IP%", getIP());
    page.replace("%HEAP%", String(ESP.getFreeHeap() / 1024));
    page.replace("%UPTIME%", getUpTime());

    server.send(200, "text/html", page);
}

void WebServerModule::handleUpdate()
{
    HTTPUpload &upload = server.upload();

    switch (upload.status)
    {
    case UPLOAD_FILE_START:
    {
        uploadStarted = true;
        Serial.printf("Update iniciado: %s\n", upload.filename.c_str());
#ifdef ESP32
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        {
            Update.printError(Serial);
        }
#elif defined(ESP8266)
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace))
        {
            Update.printError(Serial);
        }
#endif
        break;
    }

    case UPLOAD_FILE_WRITE:
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
            Update.printError(Serial);
        }
        else
        {
            Serial.print(".");
        }
        break;

    case UPLOAD_FILE_END:
        if (Update.end(true))
        {
            Serial.printf("\nUpdate completo: %u bytes\n", upload.totalSize);
            Serial.println("🔄 Upload finalizado. Tarefa OTA será encerrada após reinício...");
        }
        else
        {
            Update.printError(Serial);
        }
        break;

    default:
        break;
    }
}

void WebServerModule::handleUpdateEnd()
{
    if (Update.hasError())
    {
        server.send(500, "text/plain", "Update falhou!");
    }
    else
    {
        server.send(200, "text/html", successPage);
        delay(1000);
        ESP.restart();
    }
}

// ========== FUNÇÕES AUXILIARES ==========
String WebServerModule::getUpTime()
{
    unsigned long seconds = millis() / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;

    if (days > 0)
        return String(days) + "d " + String(hours % 24) + "h";
    if (hours > 0)
        return String(hours) + "h " + String(minutes % 60) + "m";
    return String(minutes) + "m " + String(seconds % 60) + "s";
}

String WebServerModule::getIP()
{
    return apMode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
}