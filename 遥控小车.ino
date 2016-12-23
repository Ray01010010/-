#include <Microduino_Motor.h>

#define my_Serial Serial //定义串口通讯为Serial

#define OUT1A 6     //D6，D8控制1A，1B的电机

#define OUT1B 8

#define OUT2A 5     //D5，D7控制2A，2B的电机

#define OUT2B 7

void Go();

void Left();

void Right();

void Back();

void Stop();

void setup()       //设置数字端口输入输出状态

{

Serial.begin(9600);  //初始化串口监视器通讯波特率

Serial.println("init finish");

my_Serial.begin(9600);//BT通讯波特率

pinMode(OUT1A, OUTPUT);

pinMode(OUT1B, OUTPUT);

pinMode(OUT2A, OUTPUT);

pinMode(OUT2B, OUTPUT);

}

char i;

void loop()

{

if(my_Serial.available()>0)

{

i=my_Serial.read();

my_Serial.println(i);  //向蓝牙通信的另一端发送数据

switch(i)

{

case'g':

{Go( );  break; }

case'l':

{Left( );   break;}

case'r':

{Right( );   break;}

case'b':

{Back( );   break;}

case's':

{Stop();   break;}

}

}   

}

void Go()//前进程序

{

digitalWrite(OUT1A, HIGH);

digitalWrite(OUT1B, LOW);

digitalWrite(OUT2A, LOW);

digitalWrite(OUT2B, HIGH);

}

void Right()//右转程序

{

digitalWrite(OUT1A, HIGH);

digitalWrite(OUT1B, LOW);

digitalWrite(OUT2A, LOW);

digitalWrite(OUT2B, LOW);

}

void Left()//左转程序

{

digitalWrite(OUT1A, LOW);

digitalWrite(OUT1B, LOW);

digitalWrite(OUT2A, HIGH);

digitalWrite(OUT2B, LOW);

}

void Back()//后退程序

{

digitalWrite(OUT1A, LOW);

digitalWrite(OUT1B, HIGH);

digitalWrite(OUT2A, HIGH);

digitalWrite(OUT2B, LOW);

}

void Stop()//停止程序

{

digitalWrite(OUT1A, LOW);

digitalWrite(OUT1B, LOW);

digitalWrite(OUT2A, LOW);

digitalWrite(OUT2B, LOW);

}
