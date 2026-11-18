#define d0 16
#define d1 5
#define d2 4
#define d3 0
#define d4 2
#define d5 14
#define d6 12
#define d7 13
#define d8 15

#define rx 3
#define tx 1

#define led1 d6

void setup() {
  pinMode(led1, OUTPUT);

}

void loop() {
  digitalWrite(led1, HIGH);
  delay(1000);

  digitalWrite(led1, LOW);
  delay(1000);

}
