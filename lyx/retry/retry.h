#include<Windows.h>

#define ROWS 10//行
#define COLS 10//列
#define MINENUM 10//总雷数

//游戏坐标
typedef struct _coord
{
	short x;
	short y;
}game_coord,*Pgame_coord;

game_coord boundary;
//Pgame_coord box[20];
game_coord box[ROWS*COLS];

Pgame_coord GetBoundary()
{
	return &boundary;
}
void SetBoundary()
{
	boundary.x = COLS-1;
	boundary.y = ROWS-1;
}

void CreateBox()
{
	int n=0;
	int i,j;
	for(i=0;i<COLS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			box[n].x=i;
			box[n].y=j;
			n++;
		}
	}
}

void CreateGame(HWND hwnd)
{
	SetBoundary();
	CreateBox();

}

//获得小方格坐标
Pgame_coord GetBox(int i)
{
	return &box[i];
}




//
//void GamePaint(HWND hwnd)                               //是色素块和图片一起显示的
//{
//	HPEN hpen;
//	HDC hdc, hdcmem, hdcBitmapSrc;
//	HBITMAP hbmMem;	
//	BITMAP bmp;
//	HPEN hPenBoundary;
//	HFONT hFont, hOldFont;
//	RECT rect;
//	int i, j;
//
//	GetClientRect(hwnd, &rect);
//     hdc = GetDC(hwnd); 
//
//	hdcmem = CreateCompatibleDC(hdc);//一个是创建dc
//	hbmMem = CreateCompatibleBitmap(hdc,//一个是创建位图
//	rect.right - rect.left, rect.bottom - rect.top);
//
//	SelectObject(hdcmem, hbmMem);
//	hpen = CreatePen(PS_NULL, 0, RGB(90, 90, 90));  // PEN， PS_NULL表示不可见
//	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);
//
//    // 画背景
//	 (HPEN)SelectObject(hdcmem, hpen);
//	 hdcBitmapSrc = CreateCompatibleDC(hdc);
//	SelectObject(hdcBitmapSrc, hbmpFighter0);
//	 GetObject(hbmpFighter0, sizeof(BITMAP), &bmp);
//
//	 FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
//
//	//画方格  画出所有的星星  后期只要再改变笔刷就行了
//	for (j= 0; j < 10; j++)
//	{
//    for (i= 0; i < 10; i++)
//	{
//		if (StarMap[i][j] == 0)		SelectObject(hdcBitmapSrc, hbmpFighter0);
//		else if (StarMap[i][j] == 1)	SelectObject(hdcBitmapSrc, hbmpFighter1);
//		else if (StarMap[i][j] == 2)    SelectObject(hdcBitmapSrc, hbmpFighter2);
//		else if (StarMap[i][j] == 3)	SelectObject(hdcBitmapSrc, hbmpFighter3);
//		else if (StarMap[i][j] == 4)	SelectObject(hdcBitmapSrc, hbmpFighter4);
//		else if (StarMap[i][j] == 5)    SelectObject(hdcBitmapSrc, hbmpFighter5);
//	StretchBlt(hdcmem,rectBoundary.left + j*CELL_PIXEL * 2, rectBoundary.top + i*CELL_PIXEL * 2, 40, 40,	
//hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeig                      ht,SRCCOPY);
//	}
//}
//	SelectObject(hdcmem, hPenBoundary);
////	 将PEN移动到需要绘制的方框的左上角
//	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
////	 画了一个方框。演示LineTo函数
//	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
//	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
//	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
//	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);
//
//	//	 创建了一个字体对象
//	hFont = CreateFont(25, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));
//	// 将这个FONT对象放入DC中
//	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
//	{
//		CHAR szSourceInfo[1024];
//		wsprintf(szSourceInfo, "Goal %d Score %d Level %d", GetLevelScore(),score,level);
//		//	 设置输出颜色
//		SetTextColor(hdcmem, COLOR_TEXT);
//		//	 输出字符串。
//		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
//			szSourceInfo, lstrlen(szSourceInfo));
//		//	 输出完成，将原来的字体对象放回DC中
//		SelectObject(hdcmem, hOldFont);
//	}
//	BitBlt(hdc,rect.left, rect.top,	rect.right - rect.left, rect.bottom - rect.top,	hdcmem,	0, 0,	SRCCOPY);
//  //   回收资源
//	DeleteObject(hbmMem);
//	DeleteObject(hdcmem);
//	DeleteObject(hpen);
//	DeleteObject(hPenBoundary);
//	DeleteObject(hFont);
//	DeleteObject(hdcBitmapSrc);
//	//释放占用的DC等资源
//	ReleaseDC(hwnd, hdc);
//}