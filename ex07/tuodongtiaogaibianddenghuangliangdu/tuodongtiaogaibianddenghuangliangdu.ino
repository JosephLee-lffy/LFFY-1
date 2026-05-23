#include <WiFi.h>
#include <WebServer.h>

// ====================== 配置区 ======================
// ESP32 自己发出的热点名称和密码（手机要连这个WiFi）
const char* AP_SSID = "ESP32_044";
const char* AP_PASSWORD = "12345678";

// LED引脚（板载LED默认是GPIO2）
const int LED_PIN = 2;
// ===================================================

WebServer server(80);

// 生成控制网页
String makePage() {
    // 获取当前LED状态
    String state = digitalRead(LED_PIN) ? "ON" : "OFF";
    
    String html = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 单灯控制</title>
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin-top: 50px;
        }
        button {
            padding: 10px 20px;
            font-size: 18px;
            margin: 10px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <h1>ESP32 LED 控制</h1>
    <p>当前LED状态：<b>)HTML" + state + R"HTML(</b></p >
    
    <a href=" "><button>点亮LED</button></a >
    <a href="/off"><button>熄灭LED</button></a >
</body>
</html>
)HTML";
    return html;
}

// 根页面处理
void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", makePage());
}

// 点亮LED
void handleOn() {
    digitalWrite(LED_PIN, HIGH);
    server.sendHeader("Location", "/");
    server.send(303); // 跳转回主页
}

// 熄灭LED
void handleOff() {
    digitalWrite(LED_PIN, LOW);
    server.sendHeader("Location", "/");
    server.send(303); // 跳转回主页
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // 初始熄灭LED

    // 1. 启动ESP32热点（AP模式）
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    IPAddress apIP = WiFi.softAPIP();
    Serial.println("ESP32热点启动成功！");
    Serial.print("热点名称："); Serial.println(AP_SSID);
    Serial.print("访问地址：http://"); Serial.println(apIP);

    // 2. 注册网页路由
    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.begin();
}

void loop() {
    server.handleClient(); // 处理网页请求
}