#include <Windows.h>

#define ROWS 10//行
#define COLS 10//列
#define MINENUM 10//总雷数
int DONE[ROWS][COLS];//存放用户左键戳过的点 -1为未戳过 1为戳过
int NUM[ROWS][COLS];//存放用户右键戳过的点是否被插红旗 -1为没有 1为有
int n=0;//剩余炸弹数
int m=0;//左键没有点过的点
int flag;//红旗数量

HPEN hpen;
HDC hdc, hdcmem,hdcBitmapSrc;//DC
HBITMAP hbmMem;
HBITMAP hbmpBomb,hbmpFlag,hbmpN0,hbmpN1,hbmpN2,hbmpN3,hbmpN4,hbmpN5,hbmpN6,hbmpN7,hbmpN8,hbmpN9;//图片
HPEN hPenBoundary;//边界
HPEN hOldPen;

BITMAP bmp;

HBRUSH hbrushBox;
HBRUSH hOldBrush;

HFONT hFont, hOldFont;//字体
RECT rect;

//游戏坐标
typedef struct _coord
{
	short x;
	short y;
}game_coord,*Pgame_coord;

game_coord boundary;//边框的坐标
game_coord box[ROWS][COLS];//小方格的坐标

void SetBoundary()
{
	boundary.x = COLS-1;//二维数组由(0,0)开始 
	boundary.y = ROWS-1;
}

Pgame_coord GetBoundary()
{
	return &boundary;//返回边框坐标
}

//小方格Box初始化
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

//获得小方格坐标
Pgame_coord GetBox(int i,int j)
{
	return &box[i][j];
}

//初始化DONE数组
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

//初始化NUM数组
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
	DONE[x][y] = 1;//存用户左键点过的坐标
}

void change2(int x,int y)
{
	if(NUM[x][y]==-1)
	{
		NUM[x][y] = 1;//存用户右键点过的坐标
	}
	else if(NUM[x][y] == 1)
	{
		NUM[x][y] = -1;
	}
}
void CreateGame(HWND hwnd)
{
	SetBoundary();//设置边框
	CreateBox();//初始化每一个小方格
	InitDone();//初始化DONE数组
	InitNum();//初始化NUM数组
	flag=0;
	hbmpBomb = LoadImage(NULL, _T("bomb.bmp"),       //以下为加载图片函数
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
		MessageBoxA(hwnd, "bmp file not find", "ERROR!",//鉴定图片是否加载正确
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
}

void getm(int x,int y)//求出没有被左键点击过的点
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

void adjust(int* x,int* y)//由坐标求出是box的哪一个
{
	(*x) = (*x-10)/50;
	*y = (*y-10)/50;
}