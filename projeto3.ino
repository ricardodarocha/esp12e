#define PIR_PIN 14

#define TRIG_PIN 5
#define ECHO_PIN 13

#define LDR_PIN A0

DHT dht(DHT_PIN, DHT22);

// Temperatura e umidade
void enviar_temperatura() {

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    if (isnan(temperatura) || isnan(umidade))
        return;

    enviar_dados(temperatura, umidade);
}

// Presença
void enviar_presenca() {

    int presenca = digitalRead(PIR_PIN);

    enviar_dados("presenca", presenca);
}

// Distância
void enviar_distancia() {

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    unsigned long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duracao == 0)
        return;

    float distancia = duracao * 0.0343 / 2.0;

    enviar_dados("distancia_cm", distancia);
}

// Luminosidade
void enviar_luminosidade() {

    int luminosidade = analogRead(LDR_PIN);

    enviar_dados("luminosidade", luminosidade);
}

void setup() {
    pinMode(PIR_PIN, INPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

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

        enviar_temperatura();
        enviar_presenca();
        enviar_distancia();
        enviar_luminosidade();
    }
}
