#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET

#include <Blinker.h>

char auth[] = "ID";
char ssid[] = "WIFI名称";
char pswd[] = "WIFI密码";

bool oState = false, mark = 0;

//新建组件对象
BlinkerButton Button1("btn-abc");

//按下按钮后执行
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(0,!digitalRead(0));
  if (digitalRead(0) == 0)
  {
    Button1.print("off");
  }
  if (digitalRead(0) == 1)
  {
    Button1.print("on");
  }
}

//小爱同学唤醒执行
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    digitalWrite(0, 1);

    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();

    oState = true;
  }
  else if (state == BLINKER_CMD_OFF) {
    digitalWrite(0, 0);

    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();

    oState = false;
  }
}

//小爱查询电源状态
void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}

//接受其他数据的返回值
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void setup()
{
  //设置端口
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);

  //初始化接口
  pinMode(2, INPUT_PULLUP);
  pinMode(0, OUTPUT);
  digitalWrite(0,0);

  //初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);

  //注册回调函数
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachQuery(miotQuery);
  Button1.attach(button1_callback);
}

void loop()
{
  if (digitalRead(2) == 0 && mark == 0)
  {
    digitalWrite(0, !digitalRead(0));
    oState = digitalRead(0);
    mark = 1;
  }
  if (digitalRead(2) == 1 && mark == 1)
  {
    mark = 0;
  }
  Blinker.run();
}
