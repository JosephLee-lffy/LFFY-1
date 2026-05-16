// ESP32 反相双闪 双向呼吸 无卡顿跳变
#define LED_A 18
#define LED_B 19

int val = 0;
int step = 5;
// 方向变量：1递增  -1递减
int dir = 1;

void setup()
{
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop()
{
  val = val + dir * step;

  // 到边界反转方向，不是清零
  if(val >= 255)
  {
    val = 255;
    dir = -1;
  }
  else if(val <= 0)
  {
    val = 0;
    dir = 1;
  }

  int brightA = val;
  int brightB = 255 - val;

  analogWrite(LED_A, brightA);
  analogWrite(LED_B, brightB);

  delay(15);
}