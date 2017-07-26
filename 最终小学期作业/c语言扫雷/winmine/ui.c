// 引用的头文件
#include <windows.h>// Windows窗口程序编程，需要引用头文件 Windows.h
#include <stdio.h>
#include <tchar.h>
#include "mine.h"

// 画图时使用的表示小方格的直径像素点个数。
#define CELL_PIXEL			50

//图片
#define BITMAP_FILE_BK1		"bomb.bmp"
#define BITMAP_FILE_BK2		"flag.bmp"
#define BITMAP_FILE_BK3		"0.bmp"
#define BITMAP_FILE_BK4		"1.bmp"
#define BITMAP_FILE_BK5		"2.bmp"
#define BITMAP_FILE_BK6		"3.bmp"
#define BITMAP_FILE_BK7  	"4.bmp"
#define BITMAP_FILE_BK8 	"5.bmp"
#define BITMAP_FILE_BK9 	"6.bmp"
#define BITMAP_FILE_BK10	"7.bmp"
#define BITMAP_FILE_BK11	"8.bmp"

// 用来绘图的颜色 
#define COLOR_BOUNDARY		RGB(139, 134, 130)
#define COLOR_TEXT			RGB(255,215,0)
#define COLOR_BOX           RGB(205,197,191)

//Box是画出小方格的数组，Block是每一个小方格属性的数组
Pgame_coord Box[ROWS][COLS];

/********************************************************************************
* ##########关于Windows数据类型##########
*
* Windows在C语言的基础上定义了很多Windows特有的类型。都是通过C语言关键字typedef定义的。
* Windows类型都是全大写。
*
* DWORD LPSTR WPARAM LPARAM HWND等
* 其中，以'H'大头的数据类型都是句柄
*
*******************************************************************************/

/*******************************************************************************
* ##########   关于句柄    ##########
*
* 句柄是Windows系统中的基础数据类型
* 句柄是系统对象的标识符，用来唯一表示一个系统对象
* 例如：文件、窗口、线程、计时器等等。
* 句柄的类型也有细分，基础的句柄如HANDLE，窗口句柄HWND，GDI画刷句柄HBRUSH等。
* 所有以H开头的类型基本都是句柄类型。
*
*******************************************************************************/

// 全局变量
HINSTANCE hinst; /// HINSTANCE是用来表示程序运行实例的句柄，某些API函数会使用到这个变量。
RECT rectBoundary;

// 函数声明

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


/*******************************************************************************
* ##########   入口函数WinMain    ##########
*
* Windows应用程序的入口函数是WinMain函数，其地位和作用类似标准C语言的main函数
*
*******************************************************************************/

int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{

	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_HAND);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = _T("MainMenu");
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = _T("MainWClass");

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBoxA(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		_T("MainWClass"),			// 窗口类名，必须是已经注册了的窗口类
		TEXT("扫雷"),		// title-bar string 
		//WS_CAPTION | WS_SYSMENU,	// 窗口的style，这个表示为top-level window 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBoxA(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);


	/*******************************************************************************
	* ##########   MSG类型和消息    ##########
	*
	* Windows窗口应用程序是一种消息驱动的程序框架。
	* Windows窗口应用程序运行以后，其主线程就一直处于
	* "等待消息--获取消息--效应消息" 的循环之中，直到程序退出。
	*
	* 所谓消息是Windows系统特有的一种系统对象和机制，
	* 是操作系统对某些系统状态（如时钟）和用户操作（如鼠标点击）产生的系统中断进行封装以后，
	* 可以发送到应用程序，并由应用程序进行处理的用户态对象。
	*
	* "MSG"就是用来表示"消息"的数据类型。 这里定义了一个MSG类型的变量，变量名为msg
	*
	*******************************************************************************/


	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void GamePaint(HWND hwnd)
{
	int i,j;

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);
	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);
	hdcBitmapSrc = CreateCompatibleDC(hdc);
	SelectObject(hdcBitmapSrc,hbmpBomb);
	GetObject(hbmpBomb,sizeof(BITMAP),&bmp);
	SelectObject(hdcmem, hbmMem);

	hbrushBox = CreateSolidBrush(COLOR_BOX);
	// 创建需要用到的PEN和BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN， PS_NULL表示不可见
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);


	/*******************************************************************************
	* #############  画背景  ################
	*
	*******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); 

	/*******************************************************************************
	* #############  画方格  ################
	*
	*******************************************************************************/

	// 将画图需要用的PEN和BRUSH选择到DC中
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBox);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);
	for(i = 0;i < COLS;i++)
	{
		for(j = 0;j<ROWS;j++)
		{
			if(NUM[i][j]==1)//右键戳过 单数次
			{
				SelectObject(hdcBitmapSrc,hbmpFlag);
				GetObject(hbmpFlag, sizeof(BITMAP), &bmp);
				StretchBlt(hdcmem,rectBoundary.left + i*CELL_PIXEL, rectBoundary.top + j*CELL_PIXEL, 50, 50,	
					hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);//该函数从源矩形中复制一个位图到目标矩形，必要时按目标设备设置的模式进行图像的拉伸或压缩
			}

			else if(DONE[i][j]==1)//戳过
			{
				if(Block[i][j].containMine == ok) 
				{//若含雷，显示雷的图片
					SelectObject(hdcBitmapSrc,hbmpBomb);
					GetObject(hbmpBomb, sizeof(BITMAP), &bmp);
					StretchBlt(hdcmem,rectBoundary.left + i*CELL_PIXEL, rectBoundary.top + j*CELL_PIXEL, 50, 50,	
						hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);//该函数从源矩形中复制一个位图到目标矩形，必要时按目标设备设置的模式进行图像的拉伸或压缩
				}
				else
				{

					if(Block[i][j].roundMineNum == 0)//以下为周围八个的雷数目，加载对应图片
					{
						SelectObject(hdcBitmapSrc,hbmpN0);
						GetObject(hbmpN0, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 1)
					{
						SelectObject(hdcBitmapSrc,hbmpN1);
						GetObject(hbmpN1, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 2)
					{
						SelectObject(hdcBitmapSrc,hbmpN2);
						GetObject(hbmpN2, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 3)
					{
						SelectObject(hdcBitmapSrc,hbmpN3);
						GetObject(hbmpN3, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 4)
					{
						SelectObject(hdcBitmapSrc,hbmpN4);
						GetObject(hbmpN4, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 5)
					{
						SelectObject(hdcBitmapSrc,hbmpN5);
						GetObject(hbmpN5, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 6)
					{
						SelectObject(hdcBitmapSrc,hbmpN6);
						GetObject(hbmpN6, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 7)
					{
						SelectObject(hdcBitmapSrc,hbmpN7);
						GetObject(hbmpN7, sizeof(BITMAP), &bmp);
					}
					if(Block[i][j].roundMineNum == 8)
					{
						SelectObject(hdcBitmapSrc,hbmpN8);
						GetObject(hbmpN8, sizeof(BITMAP), &bmp);
					}
					StretchBlt(hdcmem,rectBoundary.left + i*CELL_PIXEL, rectBoundary.top + j*CELL_PIXEL, 50, 50,	
						hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);//该函数从源矩形中复制一个位图到目标矩形，必要时按目标设备设置的模式进行图像的	拉伸或压缩
				}
			}

			else         //DONE数组存的是用户戳过的点，-1为未戳过                                                                      
			{//未戳过的显示灰色小方格
				Box[i][j] = GetBox(i,j);
				Rectangle(hdcmem,
					Box[i][j]->x * CELL_PIXEL + rectBoundary.left,
					Box[i][j]->y * CELL_PIXEL + rectBoundary.top,
					(Box[i][j]->x + 1)*CELL_PIXEL + rectBoundary.left,
					(Box[i][j]->y + 1)*CELL_PIXEL + rectBoundary.top);
			}
		}
	}


	/*******************************************************************************
	* #############  画墙  ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// 将PEN移动到需要绘制的方框的左上角
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// 画了一个方框。演示LineTo函数
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	/*******************************************************************************
	* #############  写一行字  ################
	*
	*******************************************************************************/

	// 创建了一个字体对象
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintfA(szSourceInfo, "扫雷  雷数：%d",getnum());                                                       //再加一个参数 表示剩余雷数
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT);
		// 输出字符串。
		TextOutA(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlenA(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 在内存DC中画完，一次输出的窗口DC上。
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  回收和释放资源  ################
	*
	*******************************************************************************/
	// 回收资源
	// DeleteObject 释放GDI对象
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hbrushBox);
	DeleteObject(hdcBitmapSrc);
	//DeleteObject(hbrushSolider);
	//DeleteObject(hBrushCMAN);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC 函数  ################
	* 释放占用的DC等系统资源。
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}


/*******************************************************************************
* ##########   根据游戏界面的坐标系大小设置游戏窗口大小    ##########
*******************************************************************************/

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// 左上角
	POINT ptRightBottom;	// 右下角
	RECT rectWindow;
	Pgame_coord pCoordBoundary = GetBoundary();

	// 设置游戏边界
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// 计算上下左右角的位置
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// 计算好了，设置窗口大小。
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
		rectBoundary.bottom + 120, //给积分信息留出显示空间。
		TRUE);
}


/*******************************************************************************
* ##########   消息处理回调函数    ##########
*
* 当窗口每收到一个消息，此函数就被调用一次。
* 由于消息的到达不确定性，因此，消息函数的调用是回调的，即：
* 应用程序定义好消息处理函数，但是不直接调用，而是将函数指针传递给操作系统（通过RigesterClass）
* 当消息达到时，操作系统通过这个函数指针调用消息处理函数，处理和相应消息。
*
* Windows窗口应用程序是一种消息驱动的程序框架。
* Windows系统中一共有几百种消息，常用的有几十种，如：
*
* WM_CREATE		当窗口被创建是会收到此消息
* WM_PAINT		当窗口应该绘制GDI图形是会收到此消息
* WM_SIZE		当窗口大小变化后会
* WM_MOVE		当窗口被移动后
* WM_DESTROY		当点击窗口的关闭按钮后
* WM_KEYDOWN		当键盘的键被按下以后
* WM_KEYUP		当键盘的键弹起以后
* WN_CHAR		当输入字符以后
* WM_TIMER		当计时器到时以后
* WM_LBUTTONDOWN WM_LBUTTONUP WM_RBUTTONDOWN WM_RBUTTONUP		分别为鼠标左右键按下和弹起
* WM_LBUTTONDBLCLK		鼠标左键双击
* WM_COMMAND		一般在窗口的菜单被点击后
* WN_NOTIFY		一般在子窗口被进行了某项操作时，父窗口会收到此消息，在子窗口是控件的情况下常被使用
* WM_SIZING		当窗口在大小变化的过程中
* WM_MOVING		当窗口在移动过程中
* WM_SETFOCUS	当窗口获得鼠标\键盘焦点
* WM_KILLFOCUS	当窗口失去鼠标\键盘焦点
*
*******************************************************************************/


LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	int x,y;
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		CreateGame(hwnd);//初始化游戏初始数
		RandomSetMines();
		ReSizeGameWnd(hwnd);
		ComputeRoundMineNum();
		break;

		// 当系统认为窗口上的GDI对象应该被重绘时，会向窗口发送一个WM_PAINT消息。
		// 当然应用程序也可以通过调用 UpateWindow来主动向窗口发送一个WM_PAINT消息。
		// 所有使用GDI在窗口上绘制图形的程序都 “必须” 写在这里。
		// 如果不是在WM_PAINT消息的处理过程中绘制GDI图形，那么在窗口刷新时就会被新被抹除和覆盖
	case WM_PAINT:
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN://当鼠标左键击中窗口 触发该事件
		if(wParam && MK_LBUTTON)
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
		}
		//xy为鼠标点击的数据
		//添加函数使xy存进DONE数组
		adjust(&x,&y);
		change1(x,y);//调用函数，把DONE[x][y]-1改成1
		GamePaint(hwnd);
		getm(x,y);
		dialog(x,y);
		break;

	case WM_RBUTTONDOWN:
		if(wParam && MK_RBUTTON)
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
		}
		adjust(&x,&y);
		change2(x,y);
		flagnum(x,y);
		GamePaint(hwnd);
		break;

	case WM_DESTROY://【注】窗口被关闭
		ExitProcess(0);//【注】推出进程 
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}
