#include "retry.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef enum
{
	error,
	ok
}State;

struct _block
{
	int roundMineNum;//���׸���
	State containMine;//�Ƿ���
	State isOpened;//�Ƿ񱻴�
	//Pgame_coord p;
}Block[COLS][ROWS];

void InitBox()
{
	int i,j,numberOpened;
	numberOpened = 0;
	for(i=0;i<COLS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			Block[i][j].containMine = error;
			Block[i][j].isOpened = error;
			Block[i][j].roundMineNum = 0;
		}
	}
}

void RandomSetMines()
{
	int count = MINENUM;
	int i,j;
	srand((unsigned)time(NULL));
	while(count)
	{
		i = rand() % 9 + 1;
		j = rand() % 9 + 1;
		if(Block[i][j].containMine == error)
		{
			Block[i][j].containMine = ok;
			count--;
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

State allRoundIsOpened(int i,int j)
{
	State flag = ok;
	if(i>0&&!Block[i-1][j].isOpened)
		flag = error;
	if(i>0&&j<ROWS&&!Block[i-1][j+1].isOpened)
		flag = error;
	if(i>0&&j>0&&!Block[i-1][j-1].isOpened)
		flag = error;
	if(j<ROWS&&!Block[i][j+1].isOpened)
		flag = error;
	if(j>0&&!Block[i][j-1].isOpened)
		flag = error;
	if(i<COLS&&j>0&&!Block[i+1][j-1].isOpened)
		flag = error;
	if(i<COLS&&!Block[i+1][j].isOpened)
		flag = error;
	if(i<COLS&&j<ROWS&&!Block[i+1][j+1].isOpened)
		flag = error;
	return flag;
}

//�Զ���С�������������С����Χ������Ϊ0�����Զ�����Χû�д򿪵�С��
//ͬʱ�ݹ飬�ٴε��øú�����
void AutoOpenBlocks(int i,int j)
{
	if(Block[i][j].roundMineNum == 0&&!allRoundIsOpened&&i>=0&&j>=0&&i<COLS&&j<ROWS)
	{
		AutoOpenBlocks(i-1,j);
		AutoOpenBlocks(i-1,j+1);
		AutoOpenBlocks(i-1,j-1);
		AutoOpenBlocks(i,j+1);	
		AutoOpenBlocks(i,j-1);
		AutoOpenBlocks(i+1,j);
		AutoOpenBlocks(i+1,j+1);
		AutoOpenBlocks(i+1,j-1);
	}
}

//��Ϸ���̣�����һ�����꣬����ը������ֱ��ʧ�ܣ������ǣ�����Χ������Ϊ0��
//���Զ�����Χ���ӣ���֮���������Ϸ
void gameopration(int i,int j)
{
	if(Block[i][j].containMine == ok)
	{
		MessageBoxA(0, "Game Over", "Game Over", 0);
		ExitProcess(0);

		//��ʾը��

	}
	else if(Block[i][j].containMine == error)
	{
		if(Block[i][j].roundMineNum == 0)
			AutoOpenBlocks(i,j);
		//else

			//��ʾ����
	}
	return ;
}