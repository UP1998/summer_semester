#include <Windows.h>

#define ROWS 10//��
#define COLS 10//��
#define MINENUM 10//������
int DONE[ROWS][COLS];//����û���������ĵ� -1Ϊδ���� 1Ϊ����
int NUM[ROWS][COLS];//����û��Ҽ������ĵ��Ƿ񱻲���� -1Ϊû�� 1Ϊ��
int n=0;//ʣ��ը����
int m=0;//���û�е���ĵ�
int flag;//��������

HPEN hpen;
HDC hdc, hdcmem,hdcBitmapSrc;//DC
HBITMAP hbmMem;
HBITMAP hbmpBomb,hbmpFlag,hbmpN0,hbmpN1,hbmpN2,hbmpN3,hbmpN4,hbmpN5,hbmpN6,hbmpN7,hbmpN8,hbmpN9;//ͼƬ
HPEN hPenBoundary;//�߽�
HPEN hOldPen;

BITMAP bmp;

HBRUSH hbrushBox;
HBRUSH hOldBrush;

HFONT hFont, hOldFont;//����
RECT rect;

//��Ϸ����
typedef struct _coord
{
	short x;
	short y;
}game_coord,*Pgame_coord;

game_coord boundary;//�߿������
game_coord box[ROWS][COLS];//С���������

void SetBoundary()
{
	boundary.x = COLS-1;//��ά������(0,0)��ʼ 
	boundary.y = ROWS-1;
}

Pgame_coord GetBoundary()
{
	return &boundary;//���ر߿�����
}

//С����Box��ʼ��
void CreateBox()
{
	int i,j;
	for(i=0;i<COLS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			box[i][j].x=i;
			box[i][j].y=j;
		}
	}
}

//���С��������
Pgame_coord GetBox(int i,int j)
{
	return &box[i][j];
}

//��ʼ��DONE����
void InitDone()
{
	int i,j;
	for(i = 0;i < COLS;i++)
	{
		for(j = 0;j < ROWS;j++)
		{
			DONE[i][j]=-1;
		}
	}
}

//��ʼ��NUM����
void InitNum()
{
	int i,j;
	for(i = 0;i < COLS;i++)
	{
		for(j = 0;j < ROWS;j++)
		{
			NUM[i][j]=-1;
		}
	}
}

void change1(int x,int y)
{
	DONE[x][y] = 1;//���û�������������
}

void change2(int x,int y)
{
	if(NUM[x][y]==-1)
	{
		NUM[x][y] = 1;//���û��Ҽ����������
	}
	else if(NUM[x][y] == 1)
	{
		NUM[x][y] = -1;
	}
}
void CreateGame(HWND hwnd)
{
	SetBoundary();//���ñ߿�
	CreateBox();//��ʼ��ÿһ��С����
	InitDone();//��ʼ��DONE����
	InitNum();//��ʼ��NUM����
	flag=0;
	hbmpBomb = LoadImage(NULL, _T("bomb.bmp"),       //����Ϊ����ͼƬ����
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpFlag = LoadImage(NULL, _T("flag.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN0 = LoadImage(NULL, _T("0.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN1 = LoadImage(NULL, _T("1.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN2 = LoadImage(NULL, _T("2.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN3 = LoadImage(NULL, _T("3.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN4 = LoadImage(NULL, _T("4.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN5 = LoadImage(NULL, _T("5.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN6 = LoadImage(NULL, _T("6.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN7 = LoadImage(NULL, _T("7.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpN8 = LoadImage(NULL, _T("8.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpBomb == NULL&&hbmpFlag == NULL&&hbmpN0 == NULL&&hbmpN1 == NULL&&hbmpN2 == NULL&&hbmpN3 == NULL&&hbmpN4 == NULL&&
		hbmpN5 == NULL&&hbmpN6 == NULL&&hbmpN7 == NULL&&hbmpN8 == NULL)
	{
		MessageBoxA(hwnd, "bmp file not find", "ERROR!",//����ͼƬ�Ƿ������ȷ
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
}

void getm(int x,int y)//���û�б����������ĵ�
{
	for(x=0;x<10;x++)
	{
		for(y=0;y<10;y++)
		{
			if(DONE[x][y]==-1)
			{
				m++;
			}
		}
	}
}

void adjust(int* x,int* y)//�����������box����һ��
{
	(*x) = (*x-10)/50;
	*y = (*y-10)/50;
}