#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>//生成随机数
#include<conio.h>//键盘按下判断相关头文件

//定义全局变量
#define FRAMEX  0 //窗口左上角横坐标
#define FRAMEY  0 //窗口左上角纵坐标
#define FRAMEWIDTH  29 //游戏窗口宽度
#define FRAMEHEIGHT 26 //游戏窗口高度
int tail[2];//记住蛇尾坐标，其中tail[0]、tail[1]分别表示横、竖坐标

//光标移动
void gotoxy(HANDLE hOut, int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos);
}

//颜色
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //更改文字颜色
	return 0;
}

//绘制边框
void MakeFrame(HANDLE hOut)
{
	{
		for (int i = 0; i < 58; i += 2)//上下边框		
		{
			gotoxy(hOut, i, 0);
			color(5);
			printf("□");
			gotoxy(hOut, i, 26);
			printf("□");
		}
		for (int i = 1; i < 26; i++)//左右边框
		{
			gotoxy(hOut, 0, i);
			printf("□");
			gotoxy(hOut, 56, i);
			printf("□");
		}
		for (int i = 2; i < 56; i += 2)	//中间网格
		{
			for (int j = 1; j < 26; j++)
			{
				gotoxy(hOut, i, j);
				color(3);
				printf("■");
			}
		}
	}

    //打印游戏操作
	gotoxy(hOut, FRAMEX, FRAMEY + FRAMEHEIGHT + 2);
	color(3);
	printf("游戏操作:   上: ↑    左: ←    右: →    下: ↓");
	gotoxy(hOut, FRAMEX, FRAMEY + FRAMEHEIGHT + 4);
	color(3);
	printf("退出: ESC ");

}

//蛇的定义
typedef struct SNAKE
{
	int x[100];                 //蛇的横坐标, x[0]蛇尾横坐标
	int y[100];                 //蛇的纵坐标, y[0]蛇尾纵坐标
	int foodCount;              //蛇吃食物总数
	int nLength;                //蛇的长度
	int nSpeed;                 //蛇的移动速度
}Snake;

//食物定义
typedef struct FOOD
{
	int x;  //食物的横坐标
	int y;  //食物的纵坐标
}Food;

//初始化
void InitSnake(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	for (int i = 0; i < 15; i++)
	{
		pSnake->x[i] = 2*i + 2;
	}
	for (int i = 0; i < 15; i++)
		pSnake->y[i] = 1;
	pSnake->foodCount = 0;
	pSnake->nLength = 15;
	pSnake->nSpeed = 80;
}

//打印蛇
void PrintfSnake(HANDLE hOut, Snake* pSnake)
{
	for (int i = 0; i < pSnake->nLength; i++)
	{
		gotoxy(hOut, pSnake->x[i], pSnake->y[i]);
		color(14);
		printf("★");
	}
}

//菜单信息
void PrintfMenu(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	//游戏等级
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 1);
	printf("游戏等级：%d", pSnake->foodCount / 5 + 1);          //每吃五个食物升一级

	//游戏得分
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 3);
	printf("游戏得分：%d分", pSnake->foodCount);                //每吃一个食物得一分

	//食物个数
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 5);
	printf("所吃食物总个数：%d个", pSnake->foodCount);           //玩家所吃的食物总数

	//游戏速度
	if (pSnake->nSpeed == 80)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：1");
	}
	else if (pSnake->nSpeed == 70)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：2");
	}
	else if (pSnake->nSpeed == 60)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：3");
	}
	else if (pSnake->nSpeed == 50)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：4");
	}
	else if (pSnake->nSpeed == 40)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：5");
	}
	else if (pSnake->nSpeed == 30)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：6");
	}
	else
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("贪吃蛇的速度级别为：逆天级别");
	}


	//食物坐标
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 9);
	printf("食物的坐标：（%d， %d）", pFood->x, pFood->y);

	//温馨提示：
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 12);
	color(3);
	printf("游戏规则：吃到食物得1分 碰到自身或按ESC键则蛇死亡。");

	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 16);
	color(3);
	printf("PS：可以穿墙");

	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 14);
	color(3);
	printf("每吃五个食物速度提高一个等级哦");
	

}

//生成食物
void MakeFood(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	srand((unsigned)time(NULL));//随机种子
	/*
	1.在游戏框架内
	2.不在蛇身上
	*/
	while (1)
	{
		pFood->x = rand() % (FRAMEWIDTH - 1);       //框架内
		pFood->y = rand() % FRAMEHEIGHT;

		if (pFood->x == 0 || pFood->y == 0)         //食物不能在边界处
			continue;

		pFood->x = 2 * pFood->x + FRAMEX;
		pFood->y += FRAMEY;

		//判断食物是不是在蛇的身上
		int temp;
		for (temp = 0; temp < pSnake->nLength; temp++)
		{
			if (pFood->x == pSnake->x[temp] && pFood->y == pSnake->y[temp])
				break;
		}

		if (temp == pSnake->nLength)
		{
			gotoxy(hOut, pFood->x, pFood->y);
			color(12);
			printf("●");
			break;
		}
	}
}

//蛇的运动
void MoveSnake(HANDLE hOut, Snake* pSnake)
{
	gotoxy(hOut, pSnake->x[0], pSnake->y[0]);
	color(3);
	printf("■");
	//printf("  ");

	//后一个坐标代替前一个坐标
	for (int i = 1; i < pSnake->nLength; i++)
	{
		pSnake->x[i - 1] = pSnake->x[i];
		pSnake->y[i - 1] = pSnake->y[i];
	}

}

//蛇吃食物
void EatFood(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	//蛇头位置与食物位置相同
	if (pSnake->x[pSnake->nLength - 1] == pFood->x && pSnake->y[pSnake->nLength - 1] == pFood->y)
	{
		pSnake->nLength++;//蛇变长，游戏难度增加
		//此时蛇的位置
		for (int i = pSnake->nLength - 1; i > 0; i--)
		{
			pSnake->x[i] = pSnake->x[i - 1];
			pSnake->y[i] = pSnake->y[i - 1];
		}
		pSnake->x[0] = tail[0]; //得到蛇尾移动前的坐标
		pSnake->y[0] = tail[1];


		MakeFood(hOut, pSnake, pFood);//重新产生食物
		pSnake->foodCount++;//菜单界面更新

		//加速
		if (pSnake->foodCount % 5 == 0)
			pSnake->nSpeed -= 10;
	}
}

//穿墙实现
void ThroughWall(HANDLE hOut, Snake* pSnake, char ch)
{
	//过上框向上移动
	if (ch == 72 && pSnake->y[pSnake->nLength - 1] == FRAMEY)
	{
		pSnake->y[pSnake->nLength - 1] = FRAMEY + FRAMEHEIGHT - 1;
	}

	//过下框向下移动
	if (ch == 80 && pSnake->y[pSnake->nLength - 1] == FRAMEY + FRAMEHEIGHT)
	{
		pSnake->y[pSnake->nLength - 1] = FRAMEY + 1;
	}

	//过右框向右移动
	if (ch == 75 && pSnake->x[pSnake->nLength - 1] == FRAMEX)
	{
		pSnake->x[pSnake->nLength - 1] = FRAMEX + 2 * FRAMEWIDTH - 4;
	}

	//过左框向左移动
	if (ch == 77 && pSnake->x[pSnake->nLength - 1] == FRAMEX + 2 * FRAMEWIDTH - 2)
	{
		pSnake->x[pSnake->nLength - 1] = FRAMEX + 2;
	}
}

//蛇的死亡
bool SnakeDie(Snake* pSnake)
{
	//当蛇头碰到自身时,蛇死,返回值为TRUE
	for (int i = 0; i < pSnake->nLength - 1; i++)
	{
		if (pSnake->x[pSnake->nLength - 1] == pSnake->x[i] && pSnake->y[pSnake->nLength - 1] == pSnake->y[i])
			return TRUE;
	}
	return FALSE;
}

//游戏结束
bool OverGame(HANDLE hOut, Snake* pSnake)
{
	system("cls");//清屏
	gotoxy(hOut, FRAMEX + FRAMEWIDTH + 10, FRAMEY + FRAMEHEIGHT / 4);
	printf("GAME OVER");
	//打印得分
	if (pSnake->foodCount < 30)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH, FRAMEY + FRAMEHEIGHT / 4 + 2);
		printf("你的游戏得分为：%d分, 要不要再来一盘？", pSnake->foodCount);
	}

	else
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH, FRAMEY + FRAMEHEIGHT / 4 + 2);
		printf("你的游戏得分为：%d分, 不错哦！再来一盘试试吧！", pSnake->foodCount);
	}
	gotoxy(hOut, FRAMEX + FRAMEWIDTH, FRAMEY + FRAMEHEIGHT / 4 + 4);
	printf("重新开始游戏：Enter    退出游戏：ESC");

	int key = 0;//初始化变量
	while (1)
	{
		if ((key = _getch()) == 13)//重新开始游戏
		{
			return FALSE;
			//bool State = StartGame();
		}
		else if ((key = _getch()) == 27)//退出
			return TRUE;
		else
			continue;
	}
}

//开始游戏
bool StartGame()
{
	char chOld = 77;//右
	char chNew = 77;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//句柄

	//定义蛇
	Snake snake;

	//定义食物
	Food food;

	//制作游戏窗口
	MakeFrame(hOut);

	//初始化蛇和食物
	InitSnake(hOut, &snake, &food);

	//产生食物
	MakeFood(hOut, &snake, &food);

	while (1)
	{
		//打印菜单信息
		PrintfMenu(hOut, &snake, &food);

		tail[0] = snake.x[0];       //记住蛇尾的横坐标
		tail[1] = snake.y[0];       //记住蛇尾的纵坐标

		bool speedUp = FALSE;
		//判断键盘是否按下
		if (_kbhit())
		{
			chNew = _getch();
			speedUp = TRUE;
		}

		switch (chNew)
		{
		case 72:    //向上
		{

			MoveSnake(hOut, &snake);
			if (chOld == 80)        //如果蛇调头，则无视
			{
				snake.y[snake.nLength - 1] += 1;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.y[snake.nLength - 1] -= 1;
			break;
		}
		case 80:    //向下
		{
			MoveSnake(hOut, &snake);
			if (chOld == 72) //如果蛇调头，则无视
			{
				snake.y[snake.nLength - 1] -= 1;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.y[snake.nLength - 1] += 1;
			break;
		}
		case 75:    //向左
		{
			MoveSnake(hOut, &snake);
			if (chOld == 77) //如果蛇调头，则无视
			{
				snake.x[snake.nLength - 1] += 2;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.x[snake.nLength - 1] -= 2;
			break;
		}
		case 77:    //向右
		{
			MoveSnake(hOut, &snake);
			if (chOld == 75) //如果蛇调头，则无视
			{
				snake.x[snake.nLength - 1] -= 2;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.x[snake.nLength - 1] += 2;
			break;
		}
		case 27:        //ESC键退出             
			break;

		default:        //去掉其他按键的影响
			chNew = chOld;
			break;
		}

		//穿墙
		ThroughWall(hOut, &snake, chNew);

		//判断有无吃食物
		EatFood(hOut, &snake, &food);

		//打印蛇
		PrintfSnake(hOut, &snake);

		/*
		游戏结束条件：
		1. 蛇碰到自身
		2. 按ESC键
		*/
		if (SnakeDie(&snake) == TRUE || chNew == 27)
		{
			gotoxy(hOut, FRAMEX + FRAMEWIDTH - 2, FRAMEY + FRAMEHEIGHT / 2 - 1);
			bool State = OverGame(hOut, &snake);
			return State;
		}
		PrintfMenu(hOut, &snake, &food);
		//速度控制
		if (speedUp == FALSE)
			Sleep(snake.nSpeed);
		else
			Sleep(10);
	}
}

//主函数
int main()
{
	while (1)
	{
		//开始游戏
		system("cls");
		bool State = StartGame();
		if (State)
			break;
	}
	return 0;
}
