const int ledPin = 13;

unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}