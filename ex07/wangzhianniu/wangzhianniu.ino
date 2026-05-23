#include <WiFi.h>
#include <WebServer.h>

// WiFi配置（改成你的热点信息）
const char* ssid = "JosephLee";
const char* password = "fy060212";

const int LED_PIN = 2;   // 板载LED引脚
WebServer server(80);

// 用来保存用户输入的数值
int userValue = 0;

// 生成网页
String makePage() {
    String state = digitalRead(LED_PIN) ? "ON" : "OFF";
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>实验2</title>
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin-top: 50px;
        }
        button, input {
            padding: 10px 20px;
            font-size: 16px;
            margin: 5px;
        }
    </style>
</head>
<body>
    <h1>第二部分：按钮控制 LED</h1>
    <p>当前状态：<b>)rawliteral" + state + R"rawliteral(</b></p >

    <a href=" "><button>点亮 LED</button></a >
    <a href="/off"><button>熄灭 LED</button></a >

    <h3>输入数值控制（示例）</h3>
    <form action="/setvalue" method="get">
        <input type="number" name="value" placeholder="输入0-255的数值">
        <button type="submit">提交</button>
    </form>

    <p>当前输入的数值：<b>)rawliteral" + String(userValue) + R"rawliteral(</b></p >
</body>
</html>
)rawliteral";
    return html;
}

// 根页面
void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", makePage());
}

// 点亮LED
void handleOn() {
    digitalWrite(LED_PIN, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);
}

// 熄灭LED
void handleOff() {
    digitalWrite(LED_PIN, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
}

// 处理用户提交的数值
void handleSetValue() {
    if (server.hasArg("value")) {
        String valueStr = server.arg("value");
        userValue = valueStr.toInt(); // 字符串转成整数

        // 这里写你对数值的处理逻辑，比如PWM调亮度：
        // analogWrite(LED_PIN, userValue);
        Serial.print("收到用户输入的数值：");
        Serial.println(userValue);
    }
    // 提交后跳回主页
    server.sendHeader("Location", "/");
    server.send(303);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // 连接WiFi
    WiFi.begin(ssid, password);
    Serial.print("连接WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n连接成功！");
    Serial.print("访问地址: http://");
    Serial.println(WiFi.localIP());

    // 注册路由
    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.on("/setvalue", handleSetValue); // 新增：处理数值提交
    server.begin();
}

void loop() {
    server.handleClient();
}