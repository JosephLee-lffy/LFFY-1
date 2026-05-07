// 定义红绿灯引脚
#define LED_PIN_R 4  // 红灯
#define LED_PIN_Y 22  // 黄灯
#define LED_PIN_G 5   // 绿灯

void setup() {
  Serial.begin(115200);
  // 初始化所有LED引脚为输出模式
  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_Y, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
}

void loop() {
  // 红灯亮1秒
  Serial.println("红灯亮");
  digitalWrite(LED_PIN_R, HIGH);
  digitalWrite(LED_PIN_Y, LOW);
  digitalWrite(LED_PIN_G, LOW);
  delay(1000);

  // 黄灯亮1秒
  Serial.println("黄灯亮");
  digitalWrite(LED_PIN_R, LOW);
  digitalWrite(LED_PIN_Y, HIGH);
  digitalWrite(LED_PIN_G, LOW);
  delay(1000);

  // 绿灯亮1秒
  Serial.println("绿灯亮");
  digitalWrite(LED_PIN_R, LOW);
  digitalWrite(LED_PIN_Y, LOW);
  digitalWrite(LED_PIN_G, HIGH);
  delay(1000);
}