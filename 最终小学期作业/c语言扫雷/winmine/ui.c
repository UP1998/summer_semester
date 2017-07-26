// ���õ�ͷ�ļ�
#include <windows.h>// Windows���ڳ����̣���Ҫ����ͷ�ļ� Windows.h
#include <stdio.h>
#include <tchar.h>
#include "mine.h"

// ��ͼʱʹ�õı�ʾС�����ֱ�����ص������
#define CELL_PIXEL			50

//ͼƬ
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

// ������ͼ����ɫ 
#define COLOR_BOUNDARY		RGB(139, 134, 130)
#define COLOR_TEXT			RGB(255,215,0)
#define COLOR_BOX           RGB(205,197,191)

//Box�ǻ���С��������飬Block��ÿһ��С�������Ե�����
Pgame_coord Box[ROWS][COLS];

/********************************************************************************
* ##########����Windows��������##########
*
* Windows��C���ԵĻ����϶����˺ܶ�Windows���е����͡�����ͨ��C���Թؼ���typedef����ġ�
* Windows���Ͷ���ȫ��д��
*
* DWORD LPSTR WPARAM LPARAM HWND��
* ���У���'H'��ͷ���������Ͷ��Ǿ��
*
*******************************************************************************/

/*******************************************************************************
* ##########   ���ھ��    ##########
*
* �����Windowsϵͳ�еĻ�����������
* �����ϵͳ����ı�ʶ��������Ψһ��ʾһ��ϵͳ����
* ���磺�ļ������ڡ��̡߳���ʱ���ȵȡ�
* ���������Ҳ��ϸ�֣������ľ����HANDLE�����ھ��HWND��GDI��ˢ���HBRUSH�ȡ�
* ������H��ͷ�����ͻ������Ǿ�����͡�
*
*******************************************************************************/

// ȫ�ֱ���
HINSTANCE hinst; /// HINSTANCE��������ʾ��������ʵ���ľ����ĳЩAPI������ʹ�õ����������
RECT rectBoundary;

// ��������

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


/*******************************************************************************
* ##########   ��ں���WinMain    ##########
*
* WindowsӦ�ó������ں�����WinMain���������λ���������Ʊ�׼C���Ե�main����
*
*******************************************************************************/

int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{

	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon��Ա����ָ�����ڵ�ͼ��
	// ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor������꣬�������趨����������ʽ��
	// ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hCursor = LoadCursor(NULL,
		IDC_HAND);
	// GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
	// ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.lpszMenuName = _T("MainMenu");
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.lpszClassName = _T("MainWClass");

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBoxA(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�


	// Create the main window. 

	hwnd = CreateWindow(
		_T("MainWClass"),			// �����������������Ѿ�ע���˵Ĵ�����
		TEXT("ɨ��"),		// title-bar string 
		//WS_CAPTION | WS_SYSMENU,	// ���ڵ�style�������ʾΪtop-level window 
		WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBoxA(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
	UpdateWindow(hwnd);


	/*******************************************************************************
	* ##########   MSG���ͺ���Ϣ    ##########
	*
	* Windows����Ӧ�ó�����һ����Ϣ�����ĳ����ܡ�
	* Windows����Ӧ�ó��������Ժ������߳̾�һֱ����
	* "�ȴ���Ϣ--��ȡ��Ϣ--ЧӦ��Ϣ" ��ѭ��֮�У�ֱ�������˳���
	*
	* ��ν��Ϣ��Windowsϵͳ���е�һ��ϵͳ����ͻ��ƣ�
	* �ǲ���ϵͳ��ĳЩϵͳ״̬����ʱ�ӣ����û����������������������ϵͳ�жϽ��з�װ�Ժ�
	* ���Է��͵�Ӧ�ó��򣬲���Ӧ�ó�����д�����û�̬����
	*
	* "MSG"����������ʾ"��Ϣ"���������͡� ���ﶨ����һ��MSG���͵ı�����������Ϊmsg
	*
	*******************************************************************************/


	// ���½�����Ϣѭ������ȡ��Ϣ--������Ϣ--������Ϣ���ɴ��ڵ���Ϣ��������������Ϣ��
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
	// ������Ҫ�õ���PEN��BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);


	/*******************************************************************************
	* #############  ������  ################
	*
	*******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); 

	/*******************************************************************************
	* #############  ������  ################
	*
	*******************************************************************************/

	// ����ͼ��Ҫ�õ�PEN��BRUSHѡ��DC��
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBox);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);
	for(i = 0;i < COLS;i++)
	{
		for(j = 0;j<ROWS;j++)
		{
			if(NUM[i][j]==1)//�Ҽ����� ������
			{
				SelectObject(hdcBitmapSrc,hbmpFlag);
				GetObject(hbmpFlag, sizeof(BITMAP), &bmp);
				StretchBlt(hdcmem,rectBoundary.left + i*CELL_PIXEL, rectBoundary.top + j*CELL_PIXEL, 50, 50,	
					hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);//�ú�����Դ�����и���һ��λͼ��Ŀ����Σ���Ҫʱ��Ŀ���豸���õ�ģʽ����ͼ��������ѹ��
			}

			else if(DONE[i][j]==1)//����
			{
				if(Block[i][j].containMine == ok) 
				{//�����ף���ʾ�׵�ͼƬ
					SelectObject(hdcBitmapSrc,hbmpBomb);
					GetObject(hbmpBomb, sizeof(BITMAP), &bmp);
					StretchBlt(hdcmem,rectBoundary.left + i*CELL_PIXEL, rectBoundary.top + j*CELL_PIXEL, 50, 50,	
						hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);//�ú�����Դ�����и���һ��λͼ��Ŀ����Σ���Ҫʱ��Ŀ���豸���õ�ģʽ����ͼ��������ѹ��
				}
				else
				{

					if(Block[i][j].roundMineNum == 0)//����Ϊ��Χ�˸�������Ŀ�����ض�ӦͼƬ
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
						hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);//�ú�����Դ�����и���һ��λͼ��Ŀ����Σ���Ҫʱ��Ŀ���豸���õ�ģʽ����ͼ���	�����ѹ��
				}
			}

			else         //DONE���������û������ĵ㣬-1Ϊδ����                                                                      
			{//δ��������ʾ��ɫС����
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
	* #############  ��ǽ  ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// ����һ��������ʾLineTo����
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	/*******************************************************************************
	* #############  дһ����  ################
	*
	*******************************************************************************/

	// ������һ���������
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintfA(szSourceInfo, "ɨ��  ������%d",getnum());                                                       //�ټ�һ������ ��ʾʣ������
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		// ����ַ�����
		TextOutA(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlenA(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  ���պ��ͷ���Դ  ################
	*
	*******************************************************************************/
	// ������Դ
	// DeleteObject �ͷ�GDI����
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
	* #############  ReleaseDC ����  ################
	* �ͷ�ռ�õ�DC��ϵͳ��Դ��
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}


/*******************************************************************************
* ##########   ������Ϸ���������ϵ��С������Ϸ���ڴ�С    ##########
*******************************************************************************/

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;
	Pgame_coord pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// �����������ҽǵ�λ��
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rectBoundary.bottom + 120, //��������Ϣ������ʾ�ռ䡣
		TRUE);
}


/*******************************************************************************
* ##########   ��Ϣ����ص�����    ##########
*
* ������ÿ�յ�һ����Ϣ���˺����ͱ�����һ�Ρ�
* ������Ϣ�ĵ��ﲻȷ���ԣ���ˣ���Ϣ�����ĵ����ǻص��ģ�����
* Ӧ�ó��������Ϣ�����������ǲ�ֱ�ӵ��ã����ǽ�����ָ�봫�ݸ�����ϵͳ��ͨ��RigesterClass��
* ����Ϣ�ﵽʱ������ϵͳͨ���������ָ�������Ϣ���������������Ӧ��Ϣ��
*
* Windows����Ӧ�ó�����һ����Ϣ�����ĳ����ܡ�
* Windowsϵͳ��һ���м�������Ϣ�����õ��м�ʮ�֣��磺
*
* WM_CREATE		�����ڱ������ǻ��յ�����Ϣ
* WM_PAINT		������Ӧ�û���GDIͼ���ǻ��յ�����Ϣ
* WM_SIZE		�����ڴ�С�仯���
* WM_MOVE		�����ڱ��ƶ���
* WM_DESTROY		��������ڵĹرհ�ť��
* WM_KEYDOWN		�����̵ļ��������Ժ�
* WM_KEYUP		�����̵ļ������Ժ�
* WN_CHAR		�������ַ��Ժ�
* WM_TIMER		����ʱ����ʱ�Ժ�
* WM_LBUTTONDOWN WM_LBUTTONUP WM_RBUTTONDOWN WM_RBUTTONUP		�ֱ�Ϊ������Ҽ����º͵���
* WM_LBUTTONDBLCLK		������˫��
* WM_COMMAND		һ���ڴ��ڵĲ˵��������
* WN_NOTIFY		һ�����Ӵ��ڱ�������ĳ�����ʱ�������ڻ��յ�����Ϣ�����Ӵ����ǿؼ�������³���ʹ��
* WM_SIZING		�������ڴ�С�仯�Ĺ�����
* WM_MOVING		���������ƶ�������
* WM_SETFOCUS	�����ڻ�����\���̽���
* WM_KILLFOCUS	������ʧȥ���\���̽���
*
*******************************************************************************/


LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	int x,y;
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
	case WM_CREATE:
		CreateGame(hwnd);//��ʼ����Ϸ��ʼ��
		RandomSetMines();
		ReSizeGameWnd(hwnd);
		ComputeRoundMineNum();
		break;

		// ��ϵͳ��Ϊ�����ϵ�GDI����Ӧ�ñ��ػ�ʱ�����򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ��ȻӦ�ó���Ҳ����ͨ������ UpateWindow�������򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ����ʹ��GDI�ڴ����ϻ���ͼ�εĳ��� �����롱 д�����
		// ���������WM_PAINT��Ϣ�Ĵ�������л���GDIͼ�Σ���ô�ڴ���ˢ��ʱ�ͻᱻ�±�Ĩ���͸���
	case WM_PAINT:
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN://�����������д��� �������¼�
		if(wParam && MK_LBUTTON)
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
		}
		//xyΪ�����������
		//��Ӻ���ʹxy���DONE����
		adjust(&x,&y);
		change1(x,y);//���ú�������DONE[x][y]-1�ĳ�1
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

	case WM_DESTROY://��ע�����ڱ��ر�
		ExitProcess(0);//��ע���Ƴ����� 
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}
