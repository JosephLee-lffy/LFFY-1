// 定义LED引脚
const int ledPin = 2;  

// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)
const int ledcChannel = 0;      // 新增：LEDC通道号（0~15任选）

void setup() {
  Serial.begin(115200);

  // 标准ESP32 LEDC用法：先配置通道，再绑定引脚
  ledcSetup(ledcChannel, freq, resolution);
  ledcAttachPin(ledPin, ledcChannel);
}

void loop() {
  // 逐渐变亮
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // 标准写法：通过通道号写入占空比
    ledcWrite(ledcChannel, dutyCycle);   
    delay(5);
  }

  // 逐渐变暗
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(ledcChannel, dutyCycle);   
    delay(5);
  }
  
  Serial.println("Breathing cycle completed");
}