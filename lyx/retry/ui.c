// ���õ�ͷ�ļ�
#include <windows.h>// Windows���ڳ����̣���Ҫ����ͷ�ļ� Windows.h
#include<stdio.h>
#include <tchar.h>
#include "mine.h"


// ��ͼʱʹ�õı�ʾ�ߺ�ʳ���Բ�ε�ֱ�����ص������
#define CELL_PIXEL			50

// ������ͼ����ɫ
//#define COLOR_GENERAL_1			RGB(255,215,0)//orange
//#define COLOR_GENERAL_2			RGB(238,180,180)//pink
//#define COLOR_GENERAL_3			RGB(205,197,191)//gray
//#define COLOR_GENERAL_4			RGB(186,85,211)//purple
//#define COLOR_GENERAL_5			RGB(32,178,170)//blue
//#define COLOR_CMAN              RGB(255,0,0)//red
//#define COLOR_SOLIDER		RGB(127,255,0)//green
#define COLOR_BOUNDARY		RGB(139, 134, 130)
#define COLOR_TEXT			RGB(255,215,0)
#define COLOR_BOX           RGB(205,197,191)


//Pgame_coord soldier[4];
//Pgame_coord general;
//Pgame_coord cman;
Pgame_coord Box[ROWS*COLS];

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
	
	HPEN hpen;
	//HBRUSH hbrush;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hbrushBox;
	//HBRUSH hbrushSolider;
	//HBRUSH hBrushGeneral[5];
	//HBRUSH hBrushCMAN;
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	int i;

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);
	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	hbrushBox = CreateSolidBrush(COLOR_BOX);
	//// ������Ҫ�õ���PEN��BRUSH
	//hbrushSolider = CreateSolidBrush(COLOR_SOLIDER); // RGB��ɫ��ʵ��BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);


	/*******************************************************************************
	* #############  ������  ################
	*
	*******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	/*******************************************************************************
	* #############  ������  ################
	*
	*******************************************************************************/

	// ����ͼ��Ҫ�õ�PEN��BRUSHѡ��DC��
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBox);
	//hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushSolider);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);
	for(i = 0;i < ROWS*COLS;i++)
	{
		Box[i] = GetBox(i);
		Rectangle(hdcmem,
			Box[i]->x * CELL_PIXEL + rectBoundary.left,
			Box[i]->y * CELL_PIXEL + rectBoundary.top,
			(Box[i]->x + 1)*CELL_PIXEL + rectBoundary.left,
			(Box[i]->y + 1)*CELL_PIXEL + rectBoundary.top);
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
		wsprintfA(szSourceInfo, "ɨ��");                                                       //�ټ�һ������ ��ʾʣ������
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
		CreateGame(hwnd);
		ReSizeGameWnd(hwnd);
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
		gameopration(x,y);
		break;

	case WM_RBUTTONDOWN:
		if(wParam && MK_RBUTTON)
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
		}
		//����һ��������ʹ��ʾ���죬������������Ѿ��Ǻ��죬��ȡ�����졣
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
//
//void GamePaint(HWND hwnd)                               //��ɫ�ؿ��ͼƬһ����ʾ��
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
//	hdcmem = CreateCompatibleDC(hdc);//һ���Ǵ���dc
//	hbmMem = CreateCompatibleBitmap(hdc,//һ���Ǵ���λͼ
//	rect.right - rect.left, rect.bottom - rect.top);
//
//	SelectObject(hdcmem, hbmMem);
//	hpen = CreatePen(PS_NULL, 0, RGB(90, 90, 90));  // PEN�� PS_NULL��ʾ���ɼ�
//	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);
//
//    // ������
//	 (HPEN)SelectObject(hdcmem, hpen);
//	 hdcBitmapSrc = CreateCompatibleDC(hdc);
//	SelectObject(hdcBitmapSrc, hbmpFighter0);
//	 GetObject(hbmpFighter0, sizeof(BITMAP), &bmp);
//
//	 FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
//	//������  �������е�����  ����ֻҪ�ٸı��ˢ������
//	for (j= 0; j < 10; j++)
//	{
//    for (i= 0; i < 10; i++)
//	{
//		if (StarMap[i][j] == 0)		SelectObject(hdcBitmapSrc, hbmpFighter0);
//		else if (StarMap[i][j] == 1)		SelectObject(hdcBitmapSrc, hbmpFighter1);
//		else if (StarMap[i][j] == 2)   SelectObject(hdcBitmapSrc, hbmpFighter2);
//		else if (StarMap[i][j] == 3)	SelectObject(hdcBitmapSrc, hbmpFighter3);
//		else if (StarMap[i][j] == 4)	SelectObject(hdcBitmapSrc, hbmpFighter4);
//		else if (StarMap[i][j] == 5)    SelectObject(hdcBitmapSrc, hbmpFighter5);
//	StretchBlt(hdcmem,rectBoundary.left + j*CELL_PIXEL * 2, rectBoundary.top + i*CELL_PIXEL * 2, 40, 40,	hdcBitmapSrc,0, 0, bmp.bmWidth, bmp.bmHeight,SRCCOPY);
//	}
//}
//	SelectObject(hdcmem, hPenBoundary);
////	 ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
//	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
////	 ����һ��������ʾLineTo����
//	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
//	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
//	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
//	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);
//
//	//	 ������һ���������
//	hFont = CreateFont(25, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));
//	// �����FONT�������DC��
//	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
//	{
//		CHAR szSourceInfo[1024];
//		wsprintf(szSourceInfo, "Goal %d Score %d Level %d", GetLevelScore(),score,level);
//		//	 ���������ɫ
//		SetTextColor(hdcmem, COLOR_TEXT);
//		//	 ����ַ�����
//		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
//			szSourceInfo, lstrlen(szSourceInfo));
//		//	 �����ɣ���ԭ�����������Ż�DC��
//		SelectObject(hdcmem, hOldFont);
//	}
//	BitBlt(hdc,rect.left, rect.top,	rect.right - rect.left, rect.bottom - rect.top,	hdcmem,	0, 0,	SRCCOPY);
//  //   ������Դ
//	DeleteObject(hbmMem);
//	DeleteObject(hdcmem);
//	DeleteObject(hpen);
//	DeleteObject(hPenBoundary);
//	DeleteObject(hFont);
//	DeleteObject(hdcBitmapSrc);
//	//�ͷ�ռ�õ�DC����Դ
//	ReleaseDC(hwnd, hdc);
//}

//void GamePaint2(HWND hwnd,int x)           //��ר�Ż�ͼƬ�ĺ���                                        //x�Ǿ������ĸ�ͼƬ��  
//{//�л��ؿ�                                //HBitmap  HbmpBackground x (x��1-6)
//	HPEN hpen;                               //hbmpbackground3=Loadlmage(NULL,BITMAP_FILE_BK3,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);�����2�����ǲ˵��Ĺؿ�
//	HDC hdc, hdcmem;                         //#define BITMAP_FILE_BK3 "help.bmp"  ���������ͼƬ�����л�
//	HBITMAP hbmMem;
//	HPEN hPenBoundary;
//	HBRUSH hOldBrush;
//	HFONT hFont, hOldFont;
//	RECT rect;
//	int i, j;  
//	GetClientRect(hwnd, &rect);
//	hdc = GetDC(hwnd);
//	hdcmem = CreateCompatibleDC(hdc);
//	hbmMem = CreateCompatibleBitmap(hdc,rect.right - rect.left, rect.bottom - rect.top);
//      
//	SelectObject(hdcmem, hbmMem);
//	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�
//	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);
//	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));//������
//	SelectObject(hdcmem, hpen);//���λͼ
//	
//	  BITMAP bmp;
//	  if (x == 1)
//	  {
//		  SelectObject(hdcmem, hbmpBackground1);
//		  GetObject(hbmpBackground1, sizeof(BITMAP), &bmp);
//	  }
//	  else if (x == 2)
//	  {
//		  SelectObject(hdcmem, hbmpBackground2);
//		  GetObject(hbmpBackground2, sizeof(BITMAP), &bmp);
//	  }
//	  else if (x == 3)
//	  {
//		  SelectObject(hdcmem, hbmpBackground3);
//		  GetObject(hbmpBackground3, sizeof(BITMAP), &bmp);
//	  }
//	  else if (x == 4)
//	  {
//		  SelectObject(hdcmem, hbmpBackground4);
//		  GetObject(hbmpBackground3, sizeof(BITMAP), &bmp);
//	  }
//	  else if (x == 5)
//	  {
//		  SelectObject(hdcmem, hbmpBackground5);
//		  GetObject(hbmpBackground5, sizeof(BITMAP), &bmp);
//	  }
//	  else if (x == 6)
//	  {
//		  SelectObject(hdcmem, hbmpBackground6);
//		  GetObject(hbmpBackground6, sizeof(BITMAP), &bmp);
//	  }
//	//��ͼƬ���л���  ���ڴ���Ƶ�ͼƬ
//	  SelectObject(hdcmem, hPenBoundary);
//	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
//	BitBlt(hdc, 10, 10, rect.right - rect.left, rect.bottom - rect.top, hdcmem, 0, 0, SRCCOPY);
//	//������Դ
//	DeleteObject(hbmMem);
//	DeleteObject(hdcmem);
//	DeleteObject(hpen);
//	DeleteObject(hPenBoundary);
//	//�ͷ�ռ�õ�DC����Դ
//	ReleaseDC(hwnd, hdc);
//}