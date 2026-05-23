#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "JosephLee";
const char* password = "fy060212";

WebServer server(80);
const int touchPin = 4;

// 上一次的触摸值，用来在串口打印变化
int lastTouchValue = -1;

// 页面：加了UTF-8 + 样式 + 变化提示
const char* htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>触摸传感器实时数据</title>
  <style>
    body { text-align:center; font-family:Arial; background:#f0f0f0; padding-top:50px; }
    h2 { color:#333; }
    #num { font-size:4em; color:#2196F3; transition: all 0.2s ease; }
    .changed { color:#F44336; transform: scale(1.1); }
  </style>
</head>
<body>
  <h2>触摸传感器数值</h2>
  <div id="num">--</div>

  <script>
    const numDiv = document.getElementById("num");
    let lastVal = -1;

    function update(){
      fetch("/val")
      .then(r=>r.text())
      .then(x=>{
        let v = parseInt(x);
        numDiv.innerText = v;
        
        // 数值变化时给个动效
        if (v !== lastVal) {
          numDiv.classList.add("changed");
          setTimeout(() => numDiv.classList.remove("changed"), 200);
          lastVal = v;
        }
      });
    }
    setInterval(update, 200);
  </script>
</body>
</html>
)HTML";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleVal() {
  int v = touchRead(touchPin);
  server.send(200, "text/plain", String(v));
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi连接成功！");
  Serial.print("你的网址：http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/val", handleVal);
  server.begin();
}

void loop() {
  server.handleClient();

  // 每200ms读一次触摸值，只在变化时打印
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 200) {
    lastPrint = millis();
    int v = touchRead(touchPin);
    
    if (v != lastTouchValue) {
      Serial.print("触摸值变化：");
      Serial.println(v);
      lastTouchValue = v;
    }
  }
}