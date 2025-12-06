#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

// Configuração do modo de rede
bool selfHost = true; // true = Access Point | false = Conectar a rede existente

// WiFi credentials para modo Client (selfHost = false)
const char *ssid = "MEO-9E52D0";
const char *password = "Jorge19955991";

// WiFi credentials para modo Access Point (selfHost = true)
const char *ap_ssid = "Domination_AP";
const char *ap_password = "12345678"; // Mínimo 8 caracteres

// Web server
AsyncWebServer server(80);
DNSServer dnsServer;

// Game state
int scoreA = 100;
int scoreB = 50;
bool isActive = false;

const char portal_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Domination - Baliza Airsoft</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            text-align: center;
            padding: 50px 20px;
            margin: 0;
        }
        h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
        }
        h2 {
            font-size: 1.5em;
            font-weight: normal;
            margin-bottom: 30px;
        }
        p {
            font-size: 1.2em;
            margin: 20px 0;
        }
        .button {
            display: inline-block;
            background: #ff4444;
            color: white;
            padding: 20px 50px;
            font-size: 1.5em;
            text-decoration: none;
            border-radius: 50px;
            border: none;
            cursor: pointer;
            box-shadow: 0 4px 15px rgba(0,0,0,0.3);
            transition: transform 0.2s, box-shadow 0.2s;
            margin-top: 30px;
        }
        .button:hover {
            transform: translateY(-3px);
            box-shadow: 0 6px 20px rgba(0,0,0,0.4);
        }
        .button:active {
            transform: translateY(0);
        }
    </style>
</head>
<body>
    <h1>Montain Wolves AIRSOFT</h1>
    <h2>Domination Game</h2>
    <p>Bem-vindo ao sistema de pontuação em tempo real!</p>
    <p>Conectado com sucesso</p>
    <a href="/game" class="button">Ver Placar Ao Vivo</a>
</body>
</html>
)rawliteral";

// HTML page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Baliza Airsoft</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { 
            background-image: url('https://upload.wikimedia.org/wikipedia/commons/thumb/a/a3/Flickr_-_The_U.S._Army_-_Airsoft_adds_hard_edge_to_combat_training.jpg/1280px-Flickr_-_The_U.S._Army_-_Airsoft_adds_hard_edge_to_combat_training.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            font-family: Arial; 
            color: white;
            text-align: center;
            padding: 20px;
        }
        .score {
            font-size: 4em;
            margin: 20px;
        }
        .team-a { 
            color: #ff4444;
        }
        .team-b {
            color: #4444ff;
        }
        #chartCanvas {
            max-width: 400px;
            margin: 30px auto;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 10px;
            padding: 20px;
        }
    </style>
</head>

<body>
    <h1>Montain Wolves AIRSOFT Team</h1>
    <h2>Domination Game</h2>
    <div class="score">
        <span class="team-a" id="scoreA">0</span> : 
        <span class="team-b" id="scoreB">0</span>
    </div>
    
    <canvas id="chartCanvas" width="300" height="300"></canvas>
    
    <p>Sistema Online</p>
    <script>
        // Mostrar alert apenas na primeira visita
        if (!sessionStorage.getItem('welcomed')) {
            alert("Bem-vindo ao sistema de pontuação online da Montain Wolves AIRSOFT Team!");
            sessionStorage.setItem('welcomed', 'true');
        }

        const canvas = document.getElementById('chartCanvas');
        const ctx = canvas.getContext('2d');
        
        function drawPieChart(scoreA, scoreB) {
            const total = scoreA + scoreB;
            if (total === 0) return;
            
            const angleA = (scoreA / total) * 2 * Math.PI;
            const angleB = (scoreB / total) * 2 * Math.PI;
            
            const centerX = canvas.width / 2;
            const centerY = canvas.height / 2;
            const radius = 120;
            
            // Limpar canvas
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            
            // Team A (vermelho)
            ctx.beginPath();
            ctx.moveTo(centerX, centerY);
            ctx.arc(centerX, centerY, radius, 0, angleA);
            ctx.closePath();
            ctx.fillStyle = '#ff4444';
            ctx.fill();
            
            // Team B (azul)
            ctx.beginPath();
            ctx.moveTo(centerX, centerY);
            ctx.arc(centerX, centerY, radius, angleA, angleA + angleB);
            ctx.closePath();
            ctx.fillStyle = '#4444ff';
            ctx.fill();
            
            // Texto com percentagens
            const percentA = ((scoreA / total) * 100).toFixed(1);
            const percentB = ((scoreB / total) * 100).toFixed(1);
            
            ctx.fillStyle = 'white';
            ctx.font = 'bold 16px Arial';
            ctx.textAlign = 'center';
            ctx.fillText('Team A: ' + percentA + '%', centerX, centerY - 10);
            ctx.fillText('Team B: ' + percentB + '%', centerX, centerY + 10);
        }

        function updateScores() {
            fetch('/scores')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('scoreA').textContent = data.scoreA;
                    document.getElementById('scoreB').textContent = data.scoreB;
                    drawPieChart(data.scoreA, data.scoreB);
                })
                .catch(err => console.error('Erro:', err));
        }
        
        // Atualizar a cada 1 segundo
        setInterval(updateScores, 1000);
        
        // Atualizar imediatamente ao carregar
        updateScores();
    </script>
</body>
</html>
)rawliteral";

// Function declarations
void GameLogic(void *parameter);
void Webserver(void *parameter);

void setup()
{
    Serial.begin(115200);

    if (selfHost)
    {
        // Modo Access Point - Criar própria rede WiFi
        Serial.println("Iniciando modo Access Point...");

        // Desligar WiFi mode antes de configurar AP
        WiFi.mode(WIFI_AP);
        delay(100);

        // Configurar AP sem senha facilita o captive portal
        WiFi.softAP(ap_ssid);

        // Configurar IP fixo para o AP
        IPAddress local_IP(192, 168, 4, 1);
        IPAddress gateway(192, 168, 4, 1);
        IPAddress subnet(255, 255, 255, 0);
        WiFi.softAPConfig(local_IP, gateway, subnet);

        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP Address: ");
        Serial.println(IP);
        Serial.print("SSID: ");
        Serial.println(ap_ssid);
        Serial.println("WiFi aberto (sem senha para facilitar captive portal)");

        // Iniciar DNS server para Captive Portal
        dnsServer.start(53, "*", IP);
        Serial.println("Captive Portal ativado!");
        Serial.println("Conecte-se ao WiFi e a página deve abrir automaticamente.");
    }
    else
    {
        // Modo Client - Conectar a rede existente
        Serial.println("Conectando a rede WiFi existente...");
        WiFi.begin(ssid, password);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }

    // HTTP endpoint principal - Página do jogo
    server.on("/game", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", index_html); });

    // Rotas de detecção de Captive Portal (usadas por diferentes sistemas operacionais)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                Serial.println("Request: /");
                request->send(200, "text/html", portal_html); });

    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                Serial.println("Request: /generate_204 (Android)");
                request->redirect("/"); });

    server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                Serial.println("Request: /hotspot-detect.html (iOS/macOS)");
                request->send(200, "text/html", portal_html); });

    server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                Serial.println("Request: /connecttest.txt (Windows)");
                request->send(200, "text/html", portal_html); });

    server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                Serial.println("Request: /redirect (Windows)");
                request->send(200, "text/html", portal_html); });

    server.on("/success.txt", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                Serial.println("Request: /success.txt (Apple)");
                request->send(200, "text/plain", "success"); });

    // Endpoint para retornar scores em JSON
    server.on("/scores", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
        String json = "{\"scoreA\":" + String(scoreA) + ",\"scoreB\":" + String(scoreB) + "}";
        request->send(200, "application/json", json); });

    // Captive Portal - Qualquer outra URL redireciona para o portal
    server.onNotFound([](AsyncWebServerRequest *request)
                      { 
                        Serial.print("Request not found: ");
                        Serial.println(request->url());
                        request->send(200, "text/html", portal_html); });

    server.begin();

    // Create tasks
    xTaskCreatePinnedToCore(
        GameLogic,
        "Game Logic Task",
        8192,
        NULL,
        1,
        NULL,
        0);

    xTaskCreatePinnedToCore(
        Webserver,
        "Webserver Task",
        8192,
        NULL,
        1,
        NULL,
        1);
}

void Webserver(void *parameter)
{
    for (;;)
    {
        if (selfHost)
        {
            dnsServer.processNextRequest(); // Processar requisições DNS para Captive Portal
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void GameLogic(void *parameter)
{
    for (;;)
    {
        // Incrementar scores a cada 3 segundos
        scoreA++;
        Serial.print("Score A: ");
        Serial.print(scoreA);
        Serial.print(" | Score B: ");
        Serial.println(scoreB);

        vTaskDelay(10 / portTICK_PERIOD_MS);

        scoreB++;
        Serial.print("Score A: ");
        Serial.print(scoreA);
        Serial.print(" | Score B: ");
        Serial.println(scoreB);

        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}
