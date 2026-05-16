// 多档位触摸调速呼吸灯
// 引脚定义
const int ledPin = 2;        // LED PWM引脚
const int touchPin = T0;     // 触摸引脚 GPIO4

// 呼吸灯基础变量
int brightness = 0;          // LED亮度 0~255
int fadeDir = 1;             // 呼吸方向 1变亮 / -1变暗
int speedLevel = 1;          // 速度档位 默认1档
unsigned long touchLastTime = 0;
const unsigned long debounceTime = 300;  // 触摸消抖延时

// 根据档位返回对应呼吸步长
int getStepByLevel(int level)
{
  switch(level)
  {
    case 1: return 1;   // 1档：慢速呼吸
    case 2: return 3;   // 2档：中速呼吸
    case 3: return 6;   // 3档：快速呼吸
    default: return 1;
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  // 1. 触摸检测 + 档位循环切换
  uint16_t touchValue = touchRead(touchPin);
  
  // 触摸阈值：数值越小代表触摸越强烈，小于40判定为触摸按下
  if(touchValue < 40 && millis() - touchLastTime > debounceTime)
  {
    speedLevel++;
    if(speedLevel > 3)
    {
      speedLevel = 1;  // 三档循环回1档
    }
    touchLastTime = millis();
    Serial.print("当前档位：");
    Serial.println(speedLevel);
  }

  // 2. 获取当前档位的呼吸步长
  int step = getStepByLevel(speedLevel);

  // 3. PWM呼吸逻辑
  brightness = brightness + fadeDir * step;

  // 亮度边界翻转
  if(brightness >= 255)
  {
    brightness = 255;
    fadeDir = -1;
  }
  else if(brightness <= 0)
  {
    brightness = 0;
    fadeDir = 1;
  }

  // 输出PWM到LED
  analogWrite(ledPin, brightness);
  
  // 基础延时，保证呼吸顺滑
  delay(15);
}