#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "retry.h"

typedef enum
{
	error,
	ok
}State;

struct _block
{
	int roundMineNum;//���׸���
	State containMine;//�Ƿ���
}Block[COLS][ROWS];

void InitBox()//��ʼ��BOX
{
	int i,j;
	for(i=0;i<COLS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			Block[i][j].containMine = error;
			Block[i][j].roundMineNum = 0;
		}
	}
}

void RandomSetMines()//�������
{
	int count = MINENUM;
	int i,j;
	srand((unsigned)time(NULL));
	while(count)
	{
		i = rand() % 9 + 1;
		j = rand() % 9 + 1;
		if(Block[i][j].containMine == error)//���������ɵ����겻�ظ�
		{
			Block[i][j].containMine = ok;//����
			count--;//ʣ��������һ
		}
	}
}

void ComputeRoundMineNum()
{
	int i,j;
	for(i=0;i<COLS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			if(i>0&&Block[i-1][j].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(i>0&&j<ROWS&&Block[i-1][j+1].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(i>0&&&j>0&Block[i-1][j-1].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(j>0&&Block[i][j+1].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(j>0&&Block[i][j-1].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(i<COLS&&j<ROWS&&Block[i+1][j+1].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(i<COLS&&Block[i+1][j].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
			if(i<COLS&&j>0&&Block[i+1][j-1].containMine==ok)
			{
				Block[i][j].roundMineNum++;
			}
		}
	}
}

void dialog(int i,int j)
{
	if(Block[i][j].containMine == ok) 
	{
		MessageBoxA(0, "Game Over", "Game Over", 0);//�ȵ�ը���ĶԻ���
		ExitProcess(0);
	}
	if(n == 0&&m == 10)
	{
		MessageBoxA(0, "Win", "Win", 0);//�ȵ�ը���ĶԻ���
		ExitProcess(0);
	}
}

void flagnum(int x,int y)//ͳ�ƺ��������
{
	flag = 0;
	for(x = 0;x < COLS;x++)
	{
		for(y = 0;y<ROWS;y++)
		{
			if(NUM[x][y] == 1)
				flag++;
		}
	}
}

int getnum()//����ʣ������
{
	n=10-flag;
	return n;
}
