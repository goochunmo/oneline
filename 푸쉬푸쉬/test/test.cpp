#include <windows.h>
#include "resource.h"

#define X_MAX		20
#define Y_MAX		12
#define STAGE_MAX	2

//블럭 크기
#define X_SIZE		60
#define Y_SIZE		60

unsigned char Map[Y_MAX][X_MAX+1];
unsigned char Stage[STAGE_MAX][Y_MAX][X_MAX+1]=
{
					{ // 1판
					"####################",
					"#     ###         .#",
					"## B ####  ### #####",
					"##   ####  ### #####",
					"###  B.      # #####",
					"####  ####  ##    ##",
					"####  ####  ## Z   #",
					"####        ## #####",
					"####  ####       ###",
					"### B ########   ###",
					"###             ####",
					"####################"
					},
					{ // 2판
					"####################",
					"#                  #",
					"#      .   Z       #",
					"####  ###   ########",
					"####  ###   ########",
					"####  ###   ########",
					"#  B  .   B . B    #",
					"#     B            #",
					"#########   ########",
					"#########   ########",
					"#                  #",
					"####################"
					}
};

static unsigned int uiX;
static unsigned int uiY;
static unsigned int uiXMan;
static unsigned int uiYMan;
static RECT	Area;
static unsigned int uiStage;
static unsigned int uiMatch;
static unsigned int uiMatch_Base;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void CopyMap()
{
	unsigned int uiCnt;
	for(uiCnt = 0; (X_MAX+1)*(Y_MAX+1) > uiCnt ; ++uiCnt)
	{
		*((unsigned char*)Map + uiCnt) 
			=*((unsigned char*)Stage[uiStage] + uiCnt);
	}
}


void InitMap()
{
			uiMatch			= 0;
			uiMatch_Base	= 0;
			CopyMap();
			for(uiY = 0; Y_MAX > uiY; ++uiY)
			{
				for(uiX = 0; X_MAX > uiX; ++uiX)
				{
					if('Z' == Map[uiY][uiX])
					{
						uiXMan = uiX;
						uiYMan = uiY;
					}
					if('.' == Map[uiY][uiX])
					{
						++uiMatch_Base;
					}
				}
			}

			Area.top	= uiYMan*Y_SIZE;
			Area.bottom	= uiYMan*Y_SIZE+Y_SIZE;
			Area.left	= uiXMan*X_SIZE;
			Area.right	= uiXMan*X_SIZE+X_SIZE;
}


 
HINSTANCE g_hInst;
HWND hWndMain;
HWND hVFW;
HWND Hwndmain;
HBITMAP hBit;
BITMAPINFO Bm;
LPCTSTR lpszClass=TEXT("test");

 
int APIENTRY WinMain( HINSTANCE hInstance
                    , HINSTANCE hPrevInstance
                    , LPSTR lpszCmdParam
                    , int nCmdShow)
{
        HWND hWnd;
        MSG Message;
        WNDCLASS WndClass;
        g_hInst = hInstance;
 
        WndClass.cbClsExtra      = 0;
        WndClass.cbWndExtra      = 0;
        WndClass.hbrBackground   = (HBRUSH)GetStockObject(WHITE_BRUSH);
        WndClass.hCursor         = LoadCursor(NULL, IDC_ARROW);
        WndClass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
        WndClass.hInstance       = hInstance;
        WndClass.lpfnWndProc     = (WNDPROC)WndProc;
        WndClass.lpszClassName   = lpszClass;
        WndClass.lpszMenuName    = NULL;
        WndClass.style           = CS_HREDRAW | CS_VREDRAW;
        RegisterClass (&WndClass);
 
        hWnd = CreateWindow( lpszClass
                           , lpszClass
                           , WS_CAPTION|WS_SYSMENU
                           , CW_USEDEFAULT
                           , CW_USEDEFAULT
                           , X_SIZE*X_MAX + 7
                           , Y_SIZE*Y_MAX + 33
                           , NULL
                           , (HMENU)NULL
                           , hInstance
                           , NULL);
        
        ShowWindow(hWnd, nCmdShow);
        hWndMain=hWnd;
 
        while (GetMessage (&Message,0,0,0))
        {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
        }
        
        return (int)Message.wParam;
}
 

LRESULT CALLBACK WndProc( HWND hWnd
                        , UINT iMessage
                        , WPARAM wParam
                        , LPARAM lParam)
{
    HDC		Hdc;
	HDC		MemDC;
	
	PAINTSTRUCT ps;

	HBITMAP OldBitmap;
	static HBITMAP BoxBitmap; // 상자
	static HBITMAP BaseBitmap;// 배경
	static HBITMAP RoadBitmap;// 길
	static HBITMAP ManFBitmap;// 앞
	static HBITMAP ManBBitmap;// 뒤 
	static HBITMAP ManRBitmap;// 오른쪽 
	static HBITMAP ManLBitmap;// 왼쪽
	static HBITMAP ManBitmap;
	static HBITMAP DotBitmap;

	
    switch(iMessage)
    {
        case WM_CREATE:
			hWndMain	= hWnd;
			
			uiStage = 0;
			InitMap();

			BaseBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(ID_CANCEL));
			RoadBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP3));
			ManFBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP5));
			ManBBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP6));
			ManRBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP2));
			ManLBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP4));
			BoxBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP7));
			DotBitmap	= LoadBitmap(g_hInst,
							MAKEINTRESOURCE(IDB_BITMAP8));
			ManBitmap	= ManFBitmap;


			
			return 0;

		case WM_PAINT:
			Hdc			= BeginPaint(hWnd, &ps);
			MemDC		= CreateCompatibleDC(Hdc);

			OldBitmap	= (HBITMAP)SelectObject(MemDC, BaseBitmap);

			for(uiY = 0; Y_MAX > uiY; ++uiY)
			{
				for(uiX = 0; X_MAX > uiX; ++uiX)
				{
					if('#' == Map[uiY][uiX])
					{
						SelectObject(MemDC, BaseBitmap);
					}
					else if(' ' == Map[uiY][uiX])
					{
						if('.' == Stage[uiStage][uiY][uiX])
						{
							SelectObject(MemDC, DotBitmap);
						}
						else
						{
							SelectObject(MemDC, RoadBitmap);
						}
					}
					else if('B' == Map[uiY][uiX])
					{
						SelectObject(MemDC, BoxBitmap);
					}
					else if('.' == Map[uiY][uiX])
					{
						SelectObject(MemDC, DotBitmap);
					}
					else// if('Z' == Map[uiY][uiX])
					{
						SelectObject(MemDC, ManBitmap);
					}




					BitBlt(  Hdc
						, uiX*X_SIZE	, uiY*Y_SIZE
						, X_SIZE		, Y_SIZE
						, MemDC
						, 0			, 0
						,SRCCOPY);

				}
			}			

			SelectObject(MemDC, OldBitmap);
			
			DeleteDC(MemDC);
			EndPaint(hWnd, &ps);
			return 0;

        case WM_DESTROY:
			DeleteObject(BaseBitmap);
			DeleteObject(RoadBitmap);
			DeleteObject(ManFBitmap);
			DeleteObject(ManBBitmap);
			DeleteObject(ManRBitmap);
			DeleteObject(ManLBitmap);
			DeleteObject(DotBitmap);
			DeleteObject(BoxBitmap);
            
			PostQuitMessage(0);
            return 0;

		case WM_KEYDOWN:
			Map[uiYMan][uiXMan] = ' ';
			switch(wParam)
			{
				case VK_PRIOR:
					if(0 < uiStage)
					{
						--uiStage;
						InitMap();
						InvalidateRect(hWnd, 0, FALSE);
					}
					return 0;
				case VK_NEXT:
					if(STAGE_MAX-1 > uiStage)
					{
						++uiStage;
						InitMap();
						InvalidateRect(hWnd, 0, FALSE);
					}
					return 0;
				case VK_LEFT:
					ManBitmap = ManLBitmap;
					if('#' == Map[uiYMan][uiXMan-1])
					{
						break;
					}
					if('B' == Map[uiYMan][uiXMan-1])
					{
						if('#' ==Map[uiYMan][uiXMan-2])
						{
							break;
						}
						if('B' ==Map[uiYMan][uiXMan-2])
						{
							break;
						}
						Map[uiYMan][uiXMan-2] = 'B';
					}
					--uiXMan;
					Area.left	= (uiXMan-1)*X_SIZE;
					break;

				case VK_RIGHT:
					ManBitmap = ManRBitmap;
					if('#' == Map[uiYMan][uiXMan+1])
					{
						break;
					}
					if('B' == Map[uiYMan][uiXMan+1])
					{
						if('#' == Map[uiYMan][uiXMan+2])
						{
							break;
						}
						if('B' == Map[uiYMan][uiXMan+2])
						{
							break;
						}
						Map[uiYMan][uiXMan+2] = 'B';
					}
					++uiXMan;
					Area.right	= (uiXMan+2)*X_SIZE;
					break;

				case VK_UP:
					ManBitmap = ManBBitmap;
					if('#' == Map[uiYMan-1][uiXMan])
					{
						break;
					}
					if('B' == Map[uiYMan-1][uiXMan])
					{
						if('#' ==Map[uiYMan-2][uiXMan])
						{
							break;
						}
						if('B' ==Map[uiYMan-2][uiXMan])
						{
							break;
						}
						Map[uiYMan-2][uiXMan] = 'B';
					}
					--uiYMan;
					Area.top	= (uiYMan-1)*Y_SIZE;
					break;

				case VK_DOWN:
					ManBitmap = ManFBitmap;
					if('#' == Map[uiYMan+1][uiXMan])
					{
						break;
					}
					if('B' == Map[uiYMan+1][uiXMan])
					{
						if('#' ==Map[uiYMan+2][uiXMan])
						{
							break;
						}
						if('B' ==Map[uiYMan+2][uiXMan])
						{
							break;
						}
						Map[uiYMan+2][uiXMan] = 'B';
					}
					++uiYMan;
					Area.bottom	= (uiYMan+2)*Y_SIZE;
					break;
				default:
					return 0;
			}
			Map[uiYMan][uiXMan] = 'Z';
			InvalidateRect(hWnd, &Area, FALSE); //p83
			return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
