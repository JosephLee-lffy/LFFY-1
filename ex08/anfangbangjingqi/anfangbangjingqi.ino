#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "JosephLee";
const char* password = "fy060212";

WebServer server(80);
const int ledPin = 2;
const int touchPin = 4;

bool armed = false;
bool inAlarm = false;
unsigned long lastBlink = 0;
bool ledState = false;

// 关键：在HTML开头加上 <meta charset="UTF-8"> 声明
const char* htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
</head>
<body style="text-align:center;">
  <h2>安防报警器</h2>
  <button onclick="fetch('/arm')">布防</button>
  <button onclick="fetch('/disarm')">撤防</button>
</body>
</html>
)HTML";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleArm() {
  armed = true;
  inAlarm = false;
  server.send(200, "text/plain", "ARMED");
}

void handleDisarm() {
  armed = false;
  inAlarm = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "DISARMED");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi连接成功！");
  Serial.print("你的网址：http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();

  if (armed && touchRead(touchPin) < 40) {
    inAlarm = true;
  }

  if (inAlarm) {
    if (millis() - lastBlink > 100) {
      lastBlink = millis();
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }
}