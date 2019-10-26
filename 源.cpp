#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>//���������
#include<conio.h>//���̰����ж����ͷ�ļ�

//����ȫ�ֱ���
#define FRAMEX  0 //�������ϽǺ�����
#define FRAMEY  0 //�������Ͻ�������
#define FRAMEWIDTH  29 //��Ϸ���ڿ��
#define FRAMEHEIGHT 26 //��Ϸ���ڸ߶�
int tail[2];//��ס��β���꣬����tail[0]��tail[1]�ֱ��ʾ�ᡢ������

//����ƶ�
void gotoxy(HANDLE hOut, int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOut, pos);
}

//��ɫ
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //����������ɫ
	return 0;
}

//���Ʊ߿�
void MakeFrame(HANDLE hOut)
{
	{
		for (int i = 0; i < 58; i += 2)//���±߿�		
		{
			gotoxy(hOut, i, 0);
			color(5);
			printf("��");
			gotoxy(hOut, i, 26);
			printf("��");
		}
		for (int i = 1; i < 26; i++)//���ұ߿�
		{
			gotoxy(hOut, 0, i);
			printf("��");
			gotoxy(hOut, 56, i);
			printf("��");
		}
		for (int i = 2; i < 56; i += 2)	//�м�����
		{
			for (int j = 1; j < 26; j++)
			{
				gotoxy(hOut, i, j);
				color(3);
				printf("��");
			}
		}
	}

    //��ӡ��Ϸ����
	gotoxy(hOut, FRAMEX, FRAMEY + FRAMEHEIGHT + 2);
	color(3);
	printf("��Ϸ����:   ��: ��    ��: ��    ��: ��    ��: ��");
	gotoxy(hOut, FRAMEX, FRAMEY + FRAMEHEIGHT + 4);
	color(3);
	printf("�˳�: ESC ");

}

//�ߵĶ���
typedef struct SNAKE
{
	int x[100];                 //�ߵĺ�����, x[0]��β������
	int y[100];                 //�ߵ�������, y[0]��β������
	int foodCount;              //�߳�ʳ������
	int nLength;                //�ߵĳ���
	int nSpeed;                 //�ߵ��ƶ��ٶ�
}Snake;

//ʳ�ﶨ��
typedef struct FOOD
{
	int x;  //ʳ��ĺ�����
	int y;  //ʳ���������
}Food;

//��ʼ��
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

//��ӡ��
void PrintfSnake(HANDLE hOut, Snake* pSnake)
{
	for (int i = 0; i < pSnake->nLength; i++)
	{
		gotoxy(hOut, pSnake->x[i], pSnake->y[i]);
		color(14);
		printf("��");
	}
}

//�˵���Ϣ
void PrintfMenu(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	//��Ϸ�ȼ�
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 1);
	printf("��Ϸ�ȼ���%d", pSnake->foodCount / 5 + 1);          //ÿ�����ʳ����һ��

	//��Ϸ�÷�
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 3);
	printf("��Ϸ�÷֣�%d��", pSnake->foodCount);                //ÿ��һ��ʳ���һ��

	//ʳ�����
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 5);
	printf("����ʳ���ܸ�����%d��", pSnake->foodCount);           //������Ե�ʳ������

	//��Ϸ�ٶ�
	if (pSnake->nSpeed == 80)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ��1");
	}
	else if (pSnake->nSpeed == 70)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ��2");
	}
	else if (pSnake->nSpeed == 60)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ��3");
	}
	else if (pSnake->nSpeed == 50)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ��4");
	}
	else if (pSnake->nSpeed == 40)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ��5");
	}
	else if (pSnake->nSpeed == 30)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ��6");
	}
	else
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 7);
		printf("̰���ߵ��ٶȼ���Ϊ�����켶��");
	}


	//ʳ������
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 9);
	printf("ʳ������꣺��%d�� %d��", pFood->x, pFood->y);

	//��ܰ��ʾ��
	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 12);
	color(3);
	printf("��Ϸ���򣺳Ե�ʳ���1�� ���������ESC������������");

	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 16);
	color(3);
	printf("PS�����Դ�ǽ");

	gotoxy(hOut, FRAMEX + FRAMEWIDTH * 2 + 5, FRAMEY + 14);
	color(3);
	printf("ÿ�����ʳ���ٶ����һ���ȼ�Ŷ");
	

}

//����ʳ��
void MakeFood(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	srand((unsigned)time(NULL));//�������
	/*
	1.����Ϸ�����
	2.����������
	*/
	while (1)
	{
		pFood->x = rand() % (FRAMEWIDTH - 1);       //�����
		pFood->y = rand() % FRAMEHEIGHT;

		if (pFood->x == 0 || pFood->y == 0)         //ʳ�ﲻ���ڱ߽紦
			continue;

		pFood->x = 2 * pFood->x + FRAMEX;
		pFood->y += FRAMEY;

		//�ж�ʳ���ǲ������ߵ�����
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
			printf("��");
			break;
		}
	}
}

//�ߵ��˶�
void MoveSnake(HANDLE hOut, Snake* pSnake)
{
	gotoxy(hOut, pSnake->x[0], pSnake->y[0]);
	color(3);
	printf("��");
	//printf("  ");

	//��һ���������ǰһ������
	for (int i = 1; i < pSnake->nLength; i++)
	{
		pSnake->x[i - 1] = pSnake->x[i];
		pSnake->y[i - 1] = pSnake->y[i];
	}

}

//�߳�ʳ��
void EatFood(HANDLE hOut, Snake* pSnake, Food* pFood)
{
	//��ͷλ����ʳ��λ����ͬ
	if (pSnake->x[pSnake->nLength - 1] == pFood->x && pSnake->y[pSnake->nLength - 1] == pFood->y)
	{
		pSnake->nLength++;//�߱䳤����Ϸ�Ѷ�����
		//��ʱ�ߵ�λ��
		for (int i = pSnake->nLength - 1; i > 0; i--)
		{
			pSnake->x[i] = pSnake->x[i - 1];
			pSnake->y[i] = pSnake->y[i - 1];
		}
		pSnake->x[0] = tail[0]; //�õ���β�ƶ�ǰ������
		pSnake->y[0] = tail[1];


		MakeFood(hOut, pSnake, pFood);//���²���ʳ��
		pSnake->foodCount++;//�˵��������

		//����
		if (pSnake->foodCount % 5 == 0)
			pSnake->nSpeed -= 10;
	}
}

//��ǽʵ��
void ThroughWall(HANDLE hOut, Snake* pSnake, char ch)
{
	//���Ͽ������ƶ�
	if (ch == 72 && pSnake->y[pSnake->nLength - 1] == FRAMEY)
	{
		pSnake->y[pSnake->nLength - 1] = FRAMEY + FRAMEHEIGHT - 1;
	}

	//���¿������ƶ�
	if (ch == 80 && pSnake->y[pSnake->nLength - 1] == FRAMEY + FRAMEHEIGHT)
	{
		pSnake->y[pSnake->nLength - 1] = FRAMEY + 1;
	}

	//���ҿ������ƶ�
	if (ch == 75 && pSnake->x[pSnake->nLength - 1] == FRAMEX)
	{
		pSnake->x[pSnake->nLength - 1] = FRAMEX + 2 * FRAMEWIDTH - 4;
	}

	//����������ƶ�
	if (ch == 77 && pSnake->x[pSnake->nLength - 1] == FRAMEX + 2 * FRAMEWIDTH - 2)
	{
		pSnake->x[pSnake->nLength - 1] = FRAMEX + 2;
	}
}

//�ߵ�����
bool SnakeDie(Snake* pSnake)
{
	//����ͷ��������ʱ,����,����ֵΪTRUE
	for (int i = 0; i < pSnake->nLength - 1; i++)
	{
		if (pSnake->x[pSnake->nLength - 1] == pSnake->x[i] && pSnake->y[pSnake->nLength - 1] == pSnake->y[i])
			return TRUE;
	}
	return FALSE;
}

//��Ϸ����
bool OverGame(HANDLE hOut, Snake* pSnake)
{
	system("cls");//����
	gotoxy(hOut, FRAMEX + FRAMEWIDTH + 10, FRAMEY + FRAMEHEIGHT / 4);
	printf("GAME OVER");
	//��ӡ�÷�
	if (pSnake->foodCount < 30)
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH, FRAMEY + FRAMEHEIGHT / 4 + 2);
		printf("�����Ϸ�÷�Ϊ��%d��, Ҫ��Ҫ����һ�̣�", pSnake->foodCount);
	}

	else
	{
		gotoxy(hOut, FRAMEX + FRAMEWIDTH, FRAMEY + FRAMEHEIGHT / 4 + 2);
		printf("�����Ϸ�÷�Ϊ��%d��, ����Ŷ������һ�����԰ɣ�", pSnake->foodCount);
	}
	gotoxy(hOut, FRAMEX + FRAMEWIDTH, FRAMEY + FRAMEHEIGHT / 4 + 4);
	printf("���¿�ʼ��Ϸ��Enter    �˳���Ϸ��ESC");

	int key = 0;//��ʼ������
	while (1)
	{
		if ((key = _getch()) == 13)//���¿�ʼ��Ϸ
		{
			return FALSE;
			//bool State = StartGame();
		}
		else if ((key = _getch()) == 27)//�˳�
			return TRUE;
		else
			continue;
	}
}

//��ʼ��Ϸ
bool StartGame()
{
	char chOld = 77;//��
	char chNew = 77;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//���

	//������
	Snake snake;

	//����ʳ��
	Food food;

	//������Ϸ����
	MakeFrame(hOut);

	//��ʼ���ߺ�ʳ��
	InitSnake(hOut, &snake, &food);

	//����ʳ��
	MakeFood(hOut, &snake, &food);

	while (1)
	{
		//��ӡ�˵���Ϣ
		PrintfMenu(hOut, &snake, &food);

		tail[0] = snake.x[0];       //��ס��β�ĺ�����
		tail[1] = snake.y[0];       //��ס��β��������

		bool speedUp = FALSE;
		//�жϼ����Ƿ���
		if (_kbhit())
		{
			chNew = _getch();
			speedUp = TRUE;
		}

		switch (chNew)
		{
		case 72:    //����
		{

			MoveSnake(hOut, &snake);
			if (chOld == 80)        //����ߵ�ͷ��������
			{
				snake.y[snake.nLength - 1] += 1;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.y[snake.nLength - 1] -= 1;
			break;
		}
		case 80:    //����
		{
			MoveSnake(hOut, &snake);
			if (chOld == 72) //����ߵ�ͷ��������
			{
				snake.y[snake.nLength - 1] -= 1;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.y[snake.nLength - 1] += 1;
			break;
		}
		case 75:    //����
		{
			MoveSnake(hOut, &snake);
			if (chOld == 77) //����ߵ�ͷ��������
			{
				snake.x[snake.nLength - 1] += 2;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.x[snake.nLength - 1] -= 2;
			break;
		}
		case 77:    //����
		{
			MoveSnake(hOut, &snake);
			if (chOld == 75) //����ߵ�ͷ��������
			{
				snake.x[snake.nLength - 1] -= 2;
				chNew = chOld;
				break;
			}
			chOld = chNew;
			snake.x[snake.nLength - 1] += 2;
			break;
		}
		case 27:        //ESC���˳�             
			break;

		default:        //ȥ������������Ӱ��
			chNew = chOld;
			break;
		}

		//��ǽ
		ThroughWall(hOut, &snake, chNew);

		//�ж����޳�ʳ��
		EatFood(hOut, &snake, &food);

		//��ӡ��
		PrintfSnake(hOut, &snake);

		/*
		��Ϸ����������
		1. ����������
		2. ��ESC��
		*/
		if (SnakeDie(&snake) == TRUE || chNew == 27)
		{
			gotoxy(hOut, FRAMEX + FRAMEWIDTH - 2, FRAMEY + FRAMEHEIGHT / 2 - 1);
			bool State = OverGame(hOut, &snake);
			return State;
		}
		PrintfMenu(hOut, &snake, &food);
		//�ٶȿ���
		if (speedUp == FALSE)
			Sleep(snake.nSpeed);
		else
			Sleep(10);
	}
}

//������
int main()
{
	while (1)
	{
		//��ʼ��Ϸ
		system("cls");
		bool State = StartGame();
		if (State)
			break;
	}
	return 0;
}
