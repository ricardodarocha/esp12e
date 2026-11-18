#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

#define RX 3
#define TX 1

#define LED D6
#define DHT_PIN D2
#define DHT_TYPE DHT22

// WiFi
const char* WIFI_SSID = "SUA_REDE";
const char* WIFI_PASSWORD = "SUA_SENHA";

// Servidor
const char* SERVER_URL = "http://SEU_SERVIDOR:3000/api/temperatura";

// Identificação
const char* ID_SENSOR = "ESP12E_001";

// Sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Intervalo de transmissão
const unsigned long INTERVALO = 2000;
unsigned long ultimaLeitura = 0;

void conectar_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Conectando ao WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void enviar_dados(float temperatura, float umidade) {

    WiFiClient client;
    HTTPClient http;

    http.setTimeout(5000);

    if (!http.begin(client, SERVER_URL)) {
        Serial.println("Erro ao iniciar HTTP");
        return;
    }

    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"id_sensor\":\"" + String(ID_SENSOR) + "\",";
    json += "\"temperatura\":" + String(temperatura, 2) + ",";
    json += "\"umidade\":" + String(umidade, 2);
    json += "}";

    int resposta = http.POST(json);

    Serial.print("HTTP: ");
    Serial.println(resposta);

    if (resposta > 0) {
        Serial.println(http.getString());

        if (resposta >= 200 && resposta < 300) {
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
        }
    } else {
        Serial.println(http.errorToString(resposta));
    }

    http.end();
}

void setup() {
    Serial.begin(115200);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    dht.begin();

    conectar_wifi();
}

void loop() {

    if (WiFi.status() != WL_CONNECTED) {
        conectar_wifi();
    }

    unsigned long agora = millis();

    if (agora - ultimaLeitura >= INTERVALO) {

        ultimaLeitura = agora;

        float temperatura = dht.readTemperature();
        float umidade = dht.readHumidity();

        if (isnan(temperatura) || isnan(umidade)) {
            Serial.println("Erro na leitura do DHT");
            return;
        }

        Serial.printf(
            "Temperatura: %.2f C | Umidade: %.2f %%\n",
            temperatura,
            umidade
        );

        enviar_dados(temperatura, umidade);
    }
}
