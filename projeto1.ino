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

const int VELOCIDADE=1000;

void setup() {
  pinMode(LED, OUTPUT);

}

void loop() {
  digitalWrite(LED, HIGH);
  delay(VELOCIDADE);

  digitalWrite(LED, LOW);
  delay(VELOCIDADE);

}
