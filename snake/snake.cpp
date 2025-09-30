#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <graphics.h>
#include <conio.h>//包含getch()函数
#include <stdlib.h>
#include <mmsystem.h>
//#include <windows.h>
#pragma comment(lib,"winmm.lib")
//#undef UNICODE
//#undef _UNICODE

#define SNAKE_NUM 500
#define length 640
#define width 480

enum dir { up, down, left, right };//0 1 2 3

bool sign=true,signScore=true, signStart1 = true,signStart2=true, signTurn = false;

int score,end,best_score,signEnding,speed=30;

MOUSEMSG m,n,o;

int r1[] = { length / 2 - 50,width / 2 - 204,length / 2 + 50,width / 2 - 129 };
int r2[] = { length / 2 - 50,width / 2 - 93,length / 2 + 50,width / 2 - 18 };
int r3[] = { length / 2 - 50,width / 2 + 18,length / 2 + 50,width / 2 + 93 };
int r4[] = { length / 2 - 50,width / 2 + 129,length / 2 + 50,width / 2 + 204 };
int r5[] = { 0,0,50,25 };//返回

RECT R1 = { r1[0], r1[1], r1[2], r1[3] };//按钮1的矩形区域 左 上 右 下
RECT R2 = { r2[0],r2[1],r2[2],r2[3] };//按钮2的矩形区域
RECT R3 = { r3[0],r3[1],r3[2],r3[3] };//按钮3的矩形区域
RECT R4 = { r4[0],r4[1],r4[2],r4[3] };//按钮4的矩形区域
RECT R5 = { r5[0],r5[1],r5[2],r5[3] };//按钮4的矩形区域

FILE* fp;

struct Snake 
{
	int size;//节数
	int direction;//方向
	int speed;//移速
	int r=5;
	POINT coor[SNAKE_NUM];//坐标

}snake;

struct Food 
{
	int x;
	int y;
	int r;//食物大小/半径
	bool flag;
	DWORD color;
}food;

//数据的初始化
void GameInit() 
{
	/*cleardevice();*/
	//初始化图形窗口
	initgraph(length, width);
	signStart1 = true, signStart2 = true, signTurn = false;
	//唤醒动画
	for (int i = 0;i < 256;i += 5)
	{
		setbkcolor(RGB(i, i, i));//设置背景色，原来默认黑色
		cleardevice();//清屏（取决于背景色）
		Sleep(30);//延时15ms
	}
	//播放音乐
	/*mciSendString("open ",0,0,0)*/
	//mciSendString("play ",0,0,0)
	snake.size = 3;
	snake.direction=right;
	snake.speed = 10;
	for (int i = 0; i <snake.size ; i++)
	{
		snake.coor[i].x = 40-10*i;//?
		snake.coor[i].y = 10;
		//printf("a%d %d", snake.coor[i].x, snake.coor[i].y);
	}
	//获取系统开机到现在所用毫秒数
	srand(GetTickCount());
	//初始化食物位置
	food.x = rand() % 540+10;
	food.y = rand() % 400+10;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.r = rand() % 10 + 10;
	food.flag = true;
	fopen_s(&fp, "D:/BestScore.txt", "a");
	fclose(fp);
	/*setbkcolor(0xF0FFF0);*/
}

void speedChoose(int a[4], int b,RECT c)
{
	int d = 40 - 10 * b;
	LOGFONT g;
	gettextstyle(&g);				//获取字体样式
	_tcscpy(g.lfFaceName, _T("宋体"));	//设置字体为宋体
	g.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&g);                     // 设置字体样式
	settextcolor(BLACK);				//BLACK在graphic.h头文件里面被定义为黑色的颜色常量
	drawtext(" Level:", &c, DT_LEFT | DT_VCENTER | DT_SINGLELINE);//在矩形区域R1内输入文字，水平居中，垂直居中，单行显示
	setlinecolor(BLACK);
	rectangle(a[0], a[1], a[2], a[3]);
	if (m.x >= a[0] && m.x <= a[2] && m.y >= a[1] && m.y <= a[3])
	{
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//如果按下鼠标左键,实现相应功能.
			speed = d;
			signTurn = true;
		}
	}
}


//返回按键
void fanhui()
{
	o = GetMouseMsg();
	LOGFONT g;
	gettextstyle(&g);				//获取字体样式
	_tcscpy(g.lfFaceName, _T("宋体"));	//设置字体为宋体
	g.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&g);                     // 设置字体样式
	settextcolor(BLACK);				//BLACK在graphic.h头文件里面被定义为黑色的颜色常量
	drawtext("返回", &R5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//在矩形区域R1内输入文字，水平居中，垂直居中，单行显示
	setlinecolor(BLACK);
	rectangle(r5[0], r5[1], r5[2], r5[3]);
	if (o.x >= r5[0] && o.x <= r5[2] && o.y >= r5[1] && o.y <= r5[3])
	{
		if (o.uMsg == WM_LBUTTONDOWN)
		{
			//如果按下鼠标左键,实现相应功能.
			signTurn = true;
			
		}
	}
}

//主界面的选择
void menuChoose(int a[4],int b)
{
	if (n.x >= a[0] && n.x <= a[2] && n.y >= a[1] && n.y <= a[3])
	{
		if (n.uMsg == WM_LBUTTONDOWN)
		{
			//如果按下鼠标左键,实现相应功能.
			switch (b)
			{
			case 1:
				signStart1 = false;
				signStart2 = false;
				break;
			case 2:
				signStart1 = false;
				break;
			case 3:
				cleardevice();
				settextstyle(20, 10, _T("宋体"));
				outtextxy(160, 235, _T("制作：L 思路来源：Bilibili CSDN"));
				while (1)
				{
					fanhui();
					if (signTurn == true)
					{
						cleardevice();
						signTurn = false;
						break;
					}
				}
				break;
			}
		}
	}
}

//主界面
void Menu()
{
	LOGFONT f;
	gettextstyle(&f);				//获取字体样式
	_tcscpy(f.lfFaceName, _T("宋体"));	//设置字体为宋体
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式
	settextcolor(BLACK);				//BLACK在graphic.h头文件里面被定义为黑色的颜色常量
	drawtext("开始游戏", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//在矩形区域R1内输入文字，水平居中，垂直居中，单行显示
	drawtext("难度设置", &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("开发名单", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	gettextstyle(&f);				//获取字体样式
	_tcscpy(f.lfFaceName, _T("宋体"));	//设置字体为宋体
	outtextxy(0, 0, _T("按键：wsad  上下左右"));
	outtextxy(0, 30, _T("      esc   退出    "));
	outtextxy(0, 60, _T("      space 暂停    "));
	outtextxy(0, 90, _T("            重新开始游戏(结束后)"));
	setlinecolor(BLACK);
	rectangle(r1[0], r1[1], r1[2], r1[3]);
	rectangle(r2[0], r2[1], r2[2], r2[3]);
	rectangle(r3[0], r3[1], r3[2], r3[3]);
}

//主界面功能实现
void menuSetting()
{
	n = GetMouseMsg();
	Menu();
	menuChoose(r1, 1);
	menuChoose(r2, 2);
	menuChoose(r3, 3);
}

//游戏速度设置
void Setting()
{
	// 获取一条鼠标消息
	m = GetMouseMsg();
	BeginBatchDraw();
	//难度1
	speedChoose(r1, 1, R1);
	
	settextstyle(20, 10, _T("宋体"));
	outtextxy(340, 63, _T("1"));
	//难度2
	speedChoose(r2, 2, R2);
	
	settextstyle(20, 10, _T("宋体"));
	outtextxy(340, 174, _T("2"));
	//难度3
	speedChoose(r3, 3, R3);
	
	settextstyle(20, 10, _T("宋体"));
	outtextxy(340, 285, _T("3"));
	//难度4
	speedChoose(r4, 4, R4);
	
	settextstyle(20, 10, _T("宋体"));
	outtextxy(340, 396, _T("4"));
	EndBatchDraw();
}

//蛇的移动
void SnakeMove()
{

		//移动，移x坐标
		for (int i = snake.size-1; i >0 ; i--)
		{
			snake.coor[i] = snake.coor[i - 1];
		}
		int snake_speed = 10;
		//方向改变
		switch (snake.direction)
		{
		case up:
			snake.coor[0].y -= snake_speed;
			if (snake.coor[0].y  <= 0)
			{
				/*snake.coor[0].y = width;*/
				sign = false;
				signEnding = 1;
			}
			break;
		case left:
			snake.coor[0].x -= snake_speed;
			if (snake.coor[0].x  <= 0)
			{
				/*snake.coor[0].x = length;*/
				sign = false;
				signEnding = 1;
			}
			break;
		case right:
			snake.coor[0].x += snake_speed;
			if (snake.coor[0].x  >= 560)
			{
				/*snake.coor[0].x = 0;*/
				sign = false;
				signEnding = 1;
			}
			break;
		case down:
			snake.coor[0].y += snake_speed;
			if (snake.coor[0].y  >= 420)
			{
				/*snake.coor[0].y = 0;*/
				sign = false;
				signEnding = 1;
			}
			break;
		}
}

//绘制游戏界面
void GameDraw()
{
	//双缓冲绘图
	BeginBatchDraw();
	//设置背景板颜色
	setbkcolor(0xF0FFF0);
	cleardevice();
	//绘制蛇头
	setfillcolor(0xEE82EE);
	solidcircle(snake.coor[0].x, snake.coor[0].y,snake.r);
	//绘制蛇身
	setfillcolor(RED);
	for (int i = 1; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y,snake.r);
		Sleep(speed);//30 20 10 0
	}
	if (food.flag)
	{
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();

}

//按键控制
void KeyControl()
{
	//判断有无按键
	if (_kbhit())
	{
		char k = _getch();
		switch (k)
		{
		case 'w':
		case 'W':
		case 72:
			if (snake.direction != down)
			{
				snake.direction = up;
			}
			break;
		case 'a':
		case 'A':
		case 75:
			if (snake.direction != right)
			{
				snake.direction = left;
			}
			break;
		case 'd':
		case 'D':
		case 77:
			if (snake.direction != left)
			{
				snake.direction = right;
			}
			break;
		case 's':
		case 'S':
		case 80:
			if (snake.direction != up)
			{
				snake.direction = down;
			}
			break;
		//暂停游戏
		case ' ':
			while (1)
			{
				if (_getch() == ' ')
				{
					break;
				}
			}
			break;
		//退出
		case 27:
			sign = false;
			signEnding = 2;
			break;
		}
	}
}

//进食
void EatFood()
{
	if (food.flag&&snake.coor[0].x>=food.x-food.r&& snake.coor[0].x <= food.x + food.r&& snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		snake.size++;
		score++;
		signScore = true;
		food.flag = false;
	}
	if (!food.flag)
	{
		food.x = rand() % 540+10;
		food.y = rand() % 400+10;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
		food.flag = true;
	}

}

//绘制围墙
void drawWall()
{
	BeginBatchDraw();
	//上
	for (int i = 0; i < 560; )
	{
		fillrectangle(i,0,i+5,5);
		i = i + 5;
	}
	//EndBatchDraw();
	//下
	//BeginBatchDraw();
	for (int i = 0; i < 560; )
	{
		fillrectangle(i, 415, i + 5, 420);
		i = i + 5;
	}
	//EndBatchDraw();
	//左
	//BeginBatchDraw();
	for (int i = 0; i < 420; )
	{
		fillrectangle(0, i, 5, i+5);
		i = i + 5;
	}
	//EndBatchDraw();
	//右
	/*BeginBatchDraw();*/
	for (int i = 0; i < 420; )
	{
		fillrectangle(555, i, 560, i + 5);
		i = i + 5;
	}
	EndBatchDraw();
}

//die
void snakeDie()
{
	for (int i = 0; i < snake.size; i++)
	{
		for (int j = 0; j < snake.size; j++)
		{
			int a = snake.coor[i].x - snake.coor[j].x;
			int b = snake.coor[i].y - snake.coor[j].y;
			if (i != j)
			{
				if (a > -10 && a<10 && b>-10 && b < 10)
				{
					sign = false;
				/*	end = 0;*/
					signEnding = 3;
					
				}
			}
		}
	}
	

}

//结束动画
void showDie()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(30, 15, _T("宋体"));
	outtextxy(250, 170, _T("游戏结束"));	
	if (signEnding == 3)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLUE);
		settextstyle(30, 15, _T("宋体"));
		outtextxy(220, 120, _T("你咬到自己了！"));
	}
	else if (signEnding == 1)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLUE);
		settextstyle(30, 15, _T("宋体"));
		outtextxy(220, 120, _T("你撞到墙了！"));
	}
	else if (signEnding == 2)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLUE);
		settextstyle(30, 15, _T("宋体"));
		outtextxy(220, 120, _T("玩家主动退出："));
		
	}
}

//显示分数
void showScore()
{
	//双缓冲绘图
	BeginBatchDraw();
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(20, 10, _T("宋体"));
	//输出当前分数
	outtextxy(5, 460, _T("当前分数："));
	char s1[5];//声明数组，用来存放字符串
	sprintf_s(s1, _T("%d"), score);//将整型转换成字符串
	outtextxy(140, 460, s1);//将字符串输出到界面上
	//输出历史最高分
	char s2[5];
	outtextxy(5, 440, _T("历史de最高分："));
	sprintf_s(s2, _T("%d"), best_score);
	outtextxy(140, 440, s2); 

	EndBatchDraw();
}

//游戏最高分存储
void writeRecord()
{
	//FILE* fp;
	fopen_s(&fp, "D:/BestScore.txt", "w");
	fprintf_s(fp, "%d", score);
	fclose(fp);
}

//游戏历史最高分读取
void readRecord()
{
	fopen_s(&fp, "D:/BestScore.txt", "r");
	fscanf(fp, "%d", &best_score);
	fclose(fp);
}

//void introduce()
//{
//	setbkmode(TRANSPARENT);
//	settextcolor(BLUE);
//	settextstyle(20, 10, _T("宋体"));
//	//输出当前分数
//	outtextxy(5, 460, _T("当前分数："));
//	//char s1[5];//声明数组，用来存放字符串
//	//sprintf_s(s1, _T("%d"), score);//将整型转换成字符串
//	//outtextxy(140, 460, s1);//将字符串输出到界面上
//	//输出历史最高分
//	//char s2[5];
//	outtextxy(5, 440, _T("历史最高分："));
//	//sprintf_s(s2, _T("%d"), best_score);
//	//outtextxy(140, 440, s2);
//}

int main()
{
Again:
	GameInit();
	while (signStart1)
	{
		A:
		menuSetting();		
	}
	cleardevice();
	while (signStart2)
	{
		Setting();	
		fanhui();
		if (signTurn)
		{
			signStart1 = true;
			signStart2 = true;
			signTurn = false;
			cleardevice();
			goto A;
		}
	};
	cleardevice();
	score = 0;
	/*introduce();*/
	readRecord();
	while (sign)
	{	
		SnakeMove();
		GameDraw();
		drawWall();
		KeyControl();
		EatFood();
		showScore();
		snakeDie();
		Sleep(50);
		if (score > best_score)
		{
			writeRecord();
			readRecord();
		}
		/*if (!sign)
		{

			showDie();
		}*/
		
	}	
	showDie();
	while (1)
	{
		if (_getch() == ' ')
		{
			cleardevice();
			closegraph();
			sign = true;
			/*GameInit();*/
			goto Again;
		}
		else if (_getch() == 27)
		{
			cleardevice();
			closegraph();
			return 0;
		}
	}	
	return 0;
}