#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <graphics.h>
#include <conio.h>//����getch()����
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
int r5[] = { 0,0,50,25 };//����

RECT R1 = { r1[0], r1[1], r1[2], r1[3] };//��ť1�ľ������� �� �� �� ��
RECT R2 = { r2[0],r2[1],r2[2],r2[3] };//��ť2�ľ�������
RECT R3 = { r3[0],r3[1],r3[2],r3[3] };//��ť3�ľ�������
RECT R4 = { r4[0],r4[1],r4[2],r4[3] };//��ť4�ľ�������
RECT R5 = { r5[0],r5[1],r5[2],r5[3] };//��ť4�ľ�������

FILE* fp;

struct Snake 
{
	int size;//����
	int direction;//����
	int speed;//����
	int r=5;
	POINT coor[SNAKE_NUM];//����

}snake;

struct Food 
{
	int x;
	int y;
	int r;//ʳ���С/�뾶
	bool flag;
	DWORD color;
}food;

//���ݵĳ�ʼ��
void GameInit() 
{
	/*cleardevice();*/
	//��ʼ��ͼ�δ���
	initgraph(length, width);
	signStart1 = true, signStart2 = true, signTurn = false;
	//���Ѷ���
	for (int i = 0;i < 256;i += 5)
	{
		setbkcolor(RGB(i, i, i));//���ñ���ɫ��ԭ��Ĭ�Ϻ�ɫ
		cleardevice();//������ȡ���ڱ���ɫ��
		Sleep(30);//��ʱ15ms
	}
	//��������
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
	//��ȡϵͳ�������������ú�����
	srand(GetTickCount());
	//��ʼ��ʳ��λ��
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
	gettextstyle(&g);				//��ȡ������ʽ
	_tcscpy(g.lfFaceName, _T("����"));	//��������Ϊ����
	g.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&g);                     // ����������ʽ
	settextcolor(BLACK);				//BLACK��graphic.hͷ�ļ����汻����Ϊ��ɫ����ɫ����
	drawtext(" Level:", &c, DT_LEFT | DT_VCENTER | DT_SINGLELINE);//�ھ�������R1���������֣�ˮƽ���У���ֱ���У�������ʾ
	setlinecolor(BLACK);
	rectangle(a[0], a[1], a[2], a[3]);
	if (m.x >= a[0] && m.x <= a[2] && m.y >= a[1] && m.y <= a[3])
	{
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//�������������,ʵ����Ӧ����.
			speed = d;
			signTurn = true;
		}
	}
}


//���ذ���
void fanhui()
{
	o = GetMouseMsg();
	LOGFONT g;
	gettextstyle(&g);				//��ȡ������ʽ
	_tcscpy(g.lfFaceName, _T("����"));	//��������Ϊ����
	g.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&g);                     // ����������ʽ
	settextcolor(BLACK);				//BLACK��graphic.hͷ�ļ����汻����Ϊ��ɫ����ɫ����
	drawtext("����", &R5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//�ھ�������R1���������֣�ˮƽ���У���ֱ���У�������ʾ
	setlinecolor(BLACK);
	rectangle(r5[0], r5[1], r5[2], r5[3]);
	if (o.x >= r5[0] && o.x <= r5[2] && o.y >= r5[1] && o.y <= r5[3])
	{
		if (o.uMsg == WM_LBUTTONDOWN)
		{
			//�������������,ʵ����Ӧ����.
			signTurn = true;
			
		}
	}
}

//�������ѡ��
void menuChoose(int a[4],int b)
{
	if (n.x >= a[0] && n.x <= a[2] && n.y >= a[1] && n.y <= a[3])
	{
		if (n.uMsg == WM_LBUTTONDOWN)
		{
			//�������������,ʵ����Ӧ����.
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
				settextstyle(20, 10, _T("����"));
				outtextxy(160, 235, _T("������L ˼·��Դ��Bilibili CSDN"));
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

//������
void Menu()
{
	LOGFONT f;
	gettextstyle(&f);				//��ȡ������ʽ
	_tcscpy(f.lfFaceName, _T("����"));	//��������Ϊ����
	f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f);                     // ����������ʽ
	settextcolor(BLACK);				//BLACK��graphic.hͷ�ļ����汻����Ϊ��ɫ����ɫ����
	drawtext("��ʼ��Ϸ", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//�ھ�������R1���������֣�ˮƽ���У���ֱ���У�������ʾ
	drawtext("�Ѷ�����", &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("��������", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	gettextstyle(&f);				//��ȡ������ʽ
	_tcscpy(f.lfFaceName, _T("����"));	//��������Ϊ����
	outtextxy(0, 0, _T("������wsad  ��������"));
	outtextxy(0, 30, _T("      esc   �˳�    "));
	outtextxy(0, 60, _T("      space ��ͣ    "));
	outtextxy(0, 90, _T("            ���¿�ʼ��Ϸ(������)"));
	setlinecolor(BLACK);
	rectangle(r1[0], r1[1], r1[2], r1[3]);
	rectangle(r2[0], r2[1], r2[2], r2[3]);
	rectangle(r3[0], r3[1], r3[2], r3[3]);
}

//�����湦��ʵ��
void menuSetting()
{
	n = GetMouseMsg();
	Menu();
	menuChoose(r1, 1);
	menuChoose(r2, 2);
	menuChoose(r3, 3);
}

//��Ϸ�ٶ�����
void Setting()
{
	// ��ȡһ�������Ϣ
	m = GetMouseMsg();
	BeginBatchDraw();
	//�Ѷ�1
	speedChoose(r1, 1, R1);
	
	settextstyle(20, 10, _T("����"));
	outtextxy(340, 63, _T("1"));
	//�Ѷ�2
	speedChoose(r2, 2, R2);
	
	settextstyle(20, 10, _T("����"));
	outtextxy(340, 174, _T("2"));
	//�Ѷ�3
	speedChoose(r3, 3, R3);
	
	settextstyle(20, 10, _T("����"));
	outtextxy(340, 285, _T("3"));
	//�Ѷ�4
	speedChoose(r4, 4, R4);
	
	settextstyle(20, 10, _T("����"));
	outtextxy(340, 396, _T("4"));
	EndBatchDraw();
}

//�ߵ��ƶ�
void SnakeMove()
{

		//�ƶ�����x����
		for (int i = snake.size-1; i >0 ; i--)
		{
			snake.coor[i] = snake.coor[i - 1];
		}
		int snake_speed = 10;
		//����ı�
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

//������Ϸ����
void GameDraw()
{
	//˫�����ͼ
	BeginBatchDraw();
	//���ñ�������ɫ
	setbkcolor(0xF0FFF0);
	cleardevice();
	//������ͷ
	setfillcolor(0xEE82EE);
	solidcircle(snake.coor[0].x, snake.coor[0].y,snake.r);
	//��������
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

//��������
void KeyControl()
{
	//�ж����ް���
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
		//��ͣ��Ϸ
		case ' ':
			while (1)
			{
				if (_getch() == ' ')
				{
					break;
				}
			}
			break;
		//�˳�
		case 27:
			sign = false;
			signEnding = 2;
			break;
		}
	}
}

//��ʳ
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

//����Χǽ
void drawWall()
{
	BeginBatchDraw();
	//��
	for (int i = 0; i < 560; )
	{
		fillrectangle(i,0,i+5,5);
		i = i + 5;
	}
	//EndBatchDraw();
	//��
	//BeginBatchDraw();
	for (int i = 0; i < 560; )
	{
		fillrectangle(i, 415, i + 5, 420);
		i = i + 5;
	}
	//EndBatchDraw();
	//��
	//BeginBatchDraw();
	for (int i = 0; i < 420; )
	{
		fillrectangle(0, i, 5, i+5);
		i = i + 5;
	}
	//EndBatchDraw();
	//��
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

//��������
void showDie()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(30, 15, _T("����"));
	outtextxy(250, 170, _T("��Ϸ����"));	
	if (signEnding == 3)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLUE);
		settextstyle(30, 15, _T("����"));
		outtextxy(220, 120, _T("��ҧ���Լ��ˣ�"));
	}
	else if (signEnding == 1)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLUE);
		settextstyle(30, 15, _T("����"));
		outtextxy(220, 120, _T("��ײ��ǽ�ˣ�"));
	}
	else if (signEnding == 2)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLUE);
		settextstyle(30, 15, _T("����"));
		outtextxy(220, 120, _T("��������˳���"));
		
	}
}

//��ʾ����
void showScore()
{
	//˫�����ͼ
	BeginBatchDraw();
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(20, 10, _T("����"));
	//�����ǰ����
	outtextxy(5, 460, _T("��ǰ������"));
	char s1[5];//�������飬��������ַ���
	sprintf_s(s1, _T("%d"), score);//������ת�����ַ���
	outtextxy(140, 460, s1);//���ַ��������������
	//�����ʷ��߷�
	char s2[5];
	outtextxy(5, 440, _T("��ʷde��߷֣�"));
	sprintf_s(s2, _T("%d"), best_score);
	outtextxy(140, 440, s2); 

	EndBatchDraw();
}

//��Ϸ��߷ִ洢
void writeRecord()
{
	//FILE* fp;
	fopen_s(&fp, "D:/BestScore.txt", "w");
	fprintf_s(fp, "%d", score);
	fclose(fp);
}

//��Ϸ��ʷ��߷ֶ�ȡ
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
//	settextstyle(20, 10, _T("����"));
//	//�����ǰ����
//	outtextxy(5, 460, _T("��ǰ������"));
//	//char s1[5];//�������飬��������ַ���
//	//sprintf_s(s1, _T("%d"), score);//������ת�����ַ���
//	//outtextxy(140, 460, s1);//���ַ��������������
//	//�����ʷ��߷�
//	//char s2[5];
//	outtextxy(5, 440, _T("��ʷ��߷֣�"));
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