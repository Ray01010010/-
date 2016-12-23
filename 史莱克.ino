#include <IRremote.h>

#include <U8glib.h>

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

IRrecv irrecv(10);

decode_results results;

#define UP 4

#define DOWN 3

#define LEFT 2

#define RIGHT 1

int box_x = 0;

int box_y = 0;

int box_x_length = 98;//x方向32  0-31

int box_y_length = 62;//y方向20  0-19

int snake_max_length = 100; //蛇身体最大长度

int snake_x[100];//蛇身x坐标

int snake_y[100];//蛇身y坐标

int snake_body_width = 3; //蛇身方块宽度（正方形）

int food_x;//食物位置坐标x

int food_y;//食物位置坐标y

int snake_length = 3; //当前蛇身长度

unsigned int game_speed;//游戏速度

int key_temp;//方向缓存

void setup(void)

{

irrecv.enableIRIn(); // 启动红外解码

u8g.firstPage();

do

{

u8g.setFont(u8g_font_gdr14r);

u8g.setPrintPos(0, 20);

u8g.print("Snake");

u8g.setPrintPos(0, 35);

u8g.print("GAME");

u8g.setFont(u8g_font_9x18);

u8g.setPrintPos(0, 55);

u8g.print("By:Freddy");

} while (u8g.nextPage());

delay(1000);

}

//游戏主界面

void snake_frame(int s)

{

u8g.firstPage();

do

{

u8g.drawFrame(box_x, box_y, box_x_length, box_y_length);

u8g.setFont(u8g_font_5x8);

u8g.setPrintPos(box_x_length + 1, 12);

u8g.print("Score");

u8g.setPrintPos(box_x_length + 1, 22);

u8g.print(snake_length-3);

u8g.setPrintPos(box_x_length + 1, 46);

u8g.print("Speed");

u8g.setPrintPos(box_x_length + 1, 56);

u8g.print(30-s);

u8g.drawFrame(food_x*snake_body_width+1, food_y*snake_body_width+1, snake_body_width, snake_body_width);//显示食物

for (int i = 0; i < snake_length; i++)//显示snake

{

u8g.drawFrame(snake_x[i]*snake_body_width+1, snake_y[i]*snake_body_width+1, snake_body_width, snake_body_width);

}

} while (u8g.nextPage());

}

//食物随机坐标生成函数

void food()

{

refood:int flag = 1;

while (flag)

{

food_x = random(0,(box_x_length-2)/3);

food_y = random(0,(box_y_length-2)/3);

for (int i = 0; i < snake_length; i++)

{

if((food_x==snake_x[i])&&(food_y==snake_y[i]))

{

goto refood;

}

flag=0;

}

}

}

//移动后坐标变换函数

void change_con(int DIR)

{

int temp_x[snake_length];

int temp_y[snake_length];

for(int i=0;i<snake_length-1;i++)//将旧坐标数据存放到temp数组  最后一位数据无效

{

temp_x[i]=snake_x[i];

temp_y[i]=snake_y[i];

}

switch(DIR)

{

case RIGHT: {snake_x[0]+=1;break;}

case LEFT: {snake_x[0]-=1;break;}

case UP: {snake_y[0]-=1;break;}

case DOWN: {snake_y[0]+=1;break;}

}

for(int i=1;i<snake_length;i++)

{

snake_x[i]=temp_x[i-1];

snake_y[i]=temp_y[i-1];

}

}

//吃到食物后坐标变换

void change_pos()

{

int temp_x[snake_length];

int temp_y[snake_length];

for(int i=0;i<snake_length-1;i++)//将旧坐标数据存放到temp数组  最后一位数据无效

{

temp_x[i]=snake_x[i];

temp_y[i]=snake_y[i];

}

snake_x[0]=food_x;

snake_y[0]=food_y;

for(int i=1;i<snake_length;i++)

{

snake_x[i]=temp_x[i-1];

snake_y[i]=temp_y[i-1];

}

}

//判断是否吃到食物 0-没有吃到 1-吃到 2-无效

int snake_eat_food(int dir)

{

int x_temp=snake_x[0];

int y_temp=snake_y[0];

switch(dir)

{

case UP :y_temp-=1;break;

case DOWN :y_temp+=1;break;

case LEFT :x_temp-=1;break;

case RIGHT :x_temp+=1;break;

}

if((x_temp==food_x)&&(y_temp==food_y))

{

snake_length+=1;

change_pos();

return 1;

}

else

{

return 0;

}

}

//判断是否撞墙 0-没有撞到 1-撞到 2-无效

int snake_knock_wall(int dir)

{

int x_temp=snake_x[0];

int y_temp=snake_y[0];

switch(dir)

{

case UP :y_temp-=1;break;

case DOWN :y_temp+=1;break;

case LEFT :x_temp-=1;break;

case RIGHT :x_temp+=1;break;

}

if(x_temp<0||x_temp>31||y_temp<0||y_temp>19)

{

return 1;

}

else

{

return 0;

}

}

//判断是否吃到自己 0-没有吃到 1-吃到 2-无效

int snake_eat_body(int dir)

{

int x_temp=snake_x[0];

int y_temp=snake_y[0];

switch(dir)

{

case UP :y_temp-=1;break;

case DOWN :y_temp+=1;break;

case LEFT :x_temp-=1;break;

case RIGHT :x_temp+=1;break;

}

for(int i=1;i<snake_length;i++)

{

if((snake_x[0]==snake_x[i])&&(snake_y[0]==snake_y[i]))

{

return 1;

}

}

return 0;

}

void loop(void)//主循环函数

{

int flag = 1; //标志

snake_x[0]=15;snake_y[0]=15;//snake起始坐标

snake_x[1]=snake_x[0]-1;snake_y[1]=snake_y[0];//snake起始坐标

snake_x[2]=snake_x[1]-1;snake_y[2]=snake_y[1];//snake起始坐标

int snake_dir=RIGHT;//初始方向 right

game_speed=30;

int food_flag=1;

food();

while (flag){

snake_frame(game_speed);

delay(game_speed);

if (irrecv.decode(&results)){irrecv.resume();}

if(results.value==33464415){key_temp = UP;}

if(results.value==33478695){key_temp = DOWN;}

if(results.value==33480735){key_temp = LEFT;}

if(results.value==33460335){key_temp=RIGHT;}

if(key_temp==UP&&snake_dir!=DOWN){snake_dir=UP;}

if(key_temp==DOWN&&snake_dir!=UP){snake_dir=DOWN;}

if(key_temp==LEFT&&snake_dir!=RIGHT){snake_dir=LEFT;}

if(key_temp==RIGHT&&snake_dir!=LEFT){snake_dir=RIGHT;}

if(snake_eat_food(snake_dir)==1){

food();

if(snake_length<10){game_speed=30;}

if(snake_length<30&&snake_length>=10){game_speed=20;}

if(snake_length<50&&snake_length>=30){game_speed=10;}

if(snake_length>=50){game_speed=0;}}

else if(snake_knock_wall(snake_dir)||snake_eat_body(snake_dir)||results.value == 33444015){

u8g.firstPage();

do

{

u8g.setFont(u8g_font_gdr14r);

u8g.setPrintPos(0, 40);

u8g.print("GAME OVER!");

} while (u8g.nextPage()); 

results.value = 33460335;

snake_length=3;

snake_x[0] = 15; snake_y[0] = 15;//snake起始坐标

snake_x[1] = snake_x[0]  - 1; snake_y[1] = snake_y[0];//snake起始坐标

snake_x[2] = snake_x[1]  - 1; snake_y[2] = snake_y[1];//snake起始坐标

flag=0;}

else{change_con(snake_dir);}

}

}
