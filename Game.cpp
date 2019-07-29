#include <Windows.h>
#include <ObjIdl.h>
//#include <gdiplus.h>
#include <thread>
#include <time.h>
#include <ctime>

//#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib") 

#define err_ex(a) MessageBox(NULL,a,NULL,NULL),ExitProcess(NULL);

LRESULT CALLBACK GameWndProc(HWND, UINT, UINT, LONG);

HINSTANCE mainHinstace;
HWND hWnd = NULL, hwndMenu = NULL;
HDC winDC,
	fonDC,
	cwinDC,
	steveDC,
	steveRDC,
	groundDC,
	treeDC,	
	stoneDC,
	houseDC,
	houseWallDC,
	houseFloorDC,
	enemyDC[5],
	hitDC,
	hpDC,
	fireBallDC;

RECT rClient, 
 	 rWindow, 
	 rFon, 
	 rSam, 
	 rcurSam;
HANDLE hThread;
HANDLE hEnemyThread;
HBITMAP hBitmap, 
		hSteve, 
		hSteveR,
		bmpFon,
		bmpMem, 
		bmpTemp,
		bmpTree,
		bmpStone,
		bmpHouse,
		bmpHouseWall,
		bmpFloor,
		bmpEnemy[5],
		bmpHit,
		bmpHpBar,
		bmpFireBall;

INT xPl = 1, yPl = 32 * 3;
INT xEn = 500, yEn = 32 * 3;
INT lvl = 0;

INT plHP = 100, enHP = 150;

BOOL jump = FALSE, onGround = TRUE, menu = FALSE, attack = FALSE;
BOOL mRight = TRUE, mLeft = FALSE, inFight = FALSE;
BOOL levels[10] = {1,0,0,0,0,0,0,0,0,0}, inHouse = FALSE;

HFONT hfont;
LOGFONT font;

wchar_t enemyName[] = L"res/rEn/0.bmp";

VOID LoadResuorce() {
	HDC			tempDC = GetDC(hWnd);
	
	cwinDC = CreateCompatibleDC(tempDC);
	groundDC = CreateCompatibleDC(tempDC);
	steveDC = CreateCompatibleDC(tempDC);
	steveRDC = CreateCompatibleDC(tempDC);
	fonDC = CreateCompatibleDC(tempDC);
	treeDC = CreateCompatibleDC(tempDC);
	stoneDC = CreateCompatibleDC(tempDC);
	houseDC = CreateCompatibleDC(tempDC);
	houseWallDC = CreateCompatibleDC(tempDC);
	houseFloorDC = CreateCompatibleDC(tempDC);
	hitDC = CreateCompatibleDC(tempDC);
	hpDC = CreateCompatibleDC(tempDC);
	fireBallDC = CreateCompatibleDC(tempDC);

	for (int i = 0; i < 5; i++) {
		enemyDC[i] = CreateCompatibleDC(tempDC);
	}

	bmpFon = (HBITMAP)LoadImage(NULL, L"res/fon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hBitmap = (HBITMAP)LoadImage(NULL, L"res/Grass.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hSteve = (HBITMAP)LoadImage(NULL, L"res/Steve.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hSteveR = (HBITMAP)LoadImage(NULL, L"res/SteveR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpTree = (HBITMAP)LoadImage(NULL, L"res/TreeBig.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpStone = (HBITMAP)LoadImage(NULL, L"res/stoneGrassed.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpHouse = (HBITMAP)LoadImage(NULL, L"res/House.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpHouseWall = (HBITMAP)LoadImage(NULL, L"res/houseWall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpHit = (HBITMAP)LoadImage(NULL, L"res/hit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpFloor = (HBITMAP)LoadImage(NULL, L"res/floor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpHpBar = (HBITMAP)LoadImage(NULL, L"res/hpBar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpFireBall = (HBITMAP)LoadImage(NULL, L"res/fireBallDark.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	for (int i = 0; i < 5; i++) {
		wsprintf(enemyName, L"%s%d%s", L"res/rEn/", i, L".bmp");
		bmpEnemy[i] = (HBITMAP)LoadImage(NULL, enemyName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	bmpMem = CreateCompatibleBitmap(tempDC, rClient.right, rClient.bottom);

	ReleaseDC(hWnd, tempDC);

	bmpTemp = (HBITMAP)SelectObject(fonDC, bmpFon);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(groundDC, hBitmap);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(steveDC, hSteve);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(steveRDC, hSteveR);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(treeDC, bmpTree);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(houseDC, bmpHouse);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(stoneDC, bmpStone);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(houseWallDC, bmpHouseWall);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(houseFloorDC, bmpFloor);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(hitDC, bmpHit);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(hpDC, bmpHpBar);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(fireBallDC, bmpFireBall);
	if (bmpTemp) DeleteObject(bmpTemp);

	bmpTemp = (HBITMAP)SelectObject(cwinDC, bmpMem);
	if (bmpTemp) DeleteObject(bmpTemp);

	for (int i = 0; i < 5; i++) {
		bmpTemp = (HBITMAP)SelectObject(enemyDC[i], bmpEnemy[i]);
		if (bmpTemp) DeleteObject(bmpTemp);
	}
}

DWORD WINAPI threadDraw(LPVOID A) {
	static int w = 64, h = 64;
	srand(time(NULL));

	BITMAP		bm, bmSteve, bmBg, bmSun, bmTree, bmStone,
				bmHouse, bmHouseWall, bmHouseFloor, bmHpBar,
				bmFireBall;

	BITMAP		bmEnemy, bmHit;
	BOOL		enemyAlive = TRUE;
	RECT		rSteveUp;

	LoadResuorce();

	GetObject(hBitmap, sizeof(BITMAP), &bm);
	GetObject(hSteve, sizeof(BITMAP), &bmSteve);
	GetObject(bmpFon, sizeof(BITMAP), &bmBg);
	GetObject(bmpTree, sizeof(BITMAP), &bmTree);
	GetObject(bmpStone, sizeof(BITMAP), &bmStone);
	GetObject(bmpHouse, sizeof(BITMAP), &bmHouse);
	GetObject(bmpHouseWall, sizeof(BITMAP), &bmHouseWall);
	GetObject(bmpEnemy[0], sizeof(BITMAP), &bmEnemy);
	GetObject(bmpHit, sizeof(BITMAP), &bmHit);
	GetObject(bmpFloor, sizeof(BITMAP), &bmHouseFloor);
	GetObject(bmpHpBar, sizeof(BITMAP), &bmHpBar);
	GetObject(bmpFireBall, sizeof(BITMAP), &bmFireBall);

	wchar_t buff[256];

	int treePos = 400, treePos1 = treePos - 200 ;
	int t = timeGetTime(), t1 = t;
	int enF = 0;
	int maxHpPl = 100, maxHpEn = 150;
	BOOL enR = TRUE,  hitDraw = FALSE;
	xEn = treePos + 50;
	
	while (true) {
		float fx = -50, fy = -50;

		if (jump == FALSE && onGround != TRUE) {
			if (rWindow.bottom - yPl <= rWindow.bottom - (bm.bmHeight * 3)) {
				yPl = bm.bmHeight * 3;
				onGround = TRUE;
			}
			else
				yPl -= 10;
		}
		if (jump == TRUE) {
			if (rWindow.bottom - yPl <= (rWindow.bottom - (bm.bmHeight * 3)) - 60) {
				onGround = FALSE;
				jump = FALSE;
			}
			else {
				yPl += 20;
			}
		}

		if (levels[lvl]) {
			fx += xPl / 40;
			fy -= yPl / 100;
			int x = 0;
			if (xPl >= xEn - 50 && xPl <= xEn + 50) {
				inFight = TRUE;
			}
			if (!inFight) {
				if (timeGetTime() - t >= 500) {
					if (xEn >= treePos1 - bmTree.bmWidth && xEn <= treePos + bmTree.bmWidth)
						enR = rand() % 2;
					else {
						if (xEn <= treePos + bmTree.bmWidth)
							enR = rand() % 2;
					}
					t = timeGetTime();
				}
				if (!enR) {
					if (xEn - 10 <= treePos1)
						xEn = treePos1;
					else
						xEn -= 10;
				}
				else {
					if (xEn + 10 >= treePos + bmTree.bmWidth)
						xEn = treePos + bmTree.bmWidth;
					else
						xEn += 10;
				}
			}
			else {
				if (xEn <= xPl - 25) {
					enR = TRUE;
					xEn += 10;
					
				}
				else
					if (xEn >= xPl + 45) {
						enR = FALSE;
						xEn -= 10;
					}
			}

			StretchBlt(cwinDC, fx, fy, rWindow.right + 100, rWindow.bottom + 80, fonDC, 0, 0, 1920, 1080, SRCCOPY);
			while (x <= rWindow.right) {
				StretchBlt(cwinDC, x, rWindow.bottom - bm.bmHeight * 3,
					bm.bmWidth * 3, bm.bmHeight * 3, groundDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				x += bm.bmWidth * 3;
			}
			TransparentBlt(cwinDC, treePos, (rWindow.bottom - bm.bmHeight * 3) - (bmTree.bmHeight - 8), bmTree.bmWidth, bmTree.bmHeight, treeDC, 0, 0, bmTree.bmWidth, bmTree.bmHeight, RGB(255, 255, 255));
			TransparentBlt(cwinDC, treePos - 20, (rWindow.bottom - bm.bmHeight * 3) - (bmStone.bmHeight - 10), bmStone.bmWidth, bmStone.bmHeight, stoneDC, 0, 0, bmStone.bmWidth, bmStone.bmHeight, RGB(255, 255, 255));
			//house
			TransparentBlt(cwinDC, rWindow.right - (bmHouse.bmWidth * 1.5), (rWindow.bottom - bm.bmHeight * 3) - (bmHouse.bmHeight * 1.5 - 20), bmHouse.bmWidth * 1.5, bmHouse.bmHeight * 1.5, houseDC, 0, 0, bmHouse.bmWidth, bmHouse.bmHeight, RGB(255, 255, 255));
			if (xPl >= rWindow.right - bmHouse.bmWidth * 1.5) {
				xPl = rWindow.right - bmHouse.bmWidth * 1.5;
				xPl = 0;
				inHouse = TRUE;
				levels[lvl] = FALSE;
				lvl++;
			}
			if (mRight)
				TransparentBlt(cwinDC, rWindow.left + xPl, (rWindow.bottom - bm.bmHeight * 2) - (yPl - 12), bmSteve.bmWidth, bmSteve.bmHeight, steveDC, 0, 0, bmSteve.bmWidth, bmSteve.bmHeight, RGB(255, 255, 255));
			if (mLeft)
				TransparentBlt(cwinDC, rWindow.left + xPl, (rWindow.bottom - bm.bmHeight * 2) - (yPl - 12), bmSteve.bmWidth, bmSteve.bmHeight, steveRDC, 0, 0, bmSteve.bmWidth, bmSteve.bmHeight, RGB(255, 255, 255));
				
			TransparentBlt(cwinDC, treePos1, (rWindow.bottom - bm.bmHeight * 3) - (bmTree.bmHeight - 20), bmTree.bmWidth, bmTree.bmHeight, treeDC, 0, 0, bmTree.bmWidth, bmTree.bmHeight, RGB(255, 255, 255));
			
			if (enemyAlive == TRUE) {
				if (enHP <= 0) enemyAlive = FALSE;
				if (enR) {
					wsprintf(enemyName, L"%s%d%s", L"res/rEn/", enF, L".bmp");
					bmpEnemy[enF] = (HBITMAP)LoadImage(NULL, enemyName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					bmpTemp = (HBITMAP)SelectObject(enemyDC[enF], bmpEnemy[enF]);
					if (bmpTemp) DeleteObject(bmpTemp);
					TransparentBlt(cwinDC, xEn, (rWindow.bottom - bm.bmHeight * 2) - yEn, bmEnemy.bmWidth / 10, bmEnemy.bmHeight / 10, (HDC)enemyDC[enF], 0, 0, bmEnemy.bmWidth, bmEnemy.bmHeight, RGB(255, 255, 255));
					if (xPl >= xEn && xPl <= xEn + 100) {
						if(timeGetTime() - t > 1000) {
							plHP -= 10;
							t = timeGetTime();
						}
					}
				}
				if (!enR) {
					wsprintf(enemyName, L"%s%d%s", L"res/lEn/", enF, L".bmp");
					bmpEnemy[enF] = (HBITMAP)LoadImage(NULL, enemyName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					bmpTemp = (HBITMAP)SelectObject(enemyDC[enF], bmpEnemy[enF]);
					if (bmpTemp) DeleteObject(bmpTemp);
					TransparentBlt(cwinDC, xEn, (rWindow.bottom - bm.bmHeight * 2) - yEn, bmEnemy.bmWidth / 10, bmEnemy.bmHeight / 10, (HDC)enemyDC[enF], 0, 0, bmEnemy.bmWidth, bmEnemy.bmHeight, RGB(255, 255, 255));
					if (xPl >= xEn - 100 && xPl <= xEn) {
						if (timeGetTime() - t > 1000){
							plHP -= 10;
							t = timeGetTime();
						}
					}
				}
			if (timeGetTime() - t1 > 100) {
				enF++;
				t1 = timeGetTime();
			}
			if (enF == 5)
				enF = 0;
			TransparentBlt(cwinDC, rWindow.left + xEn, (rWindow.bottom - bm.bmHeight * 2) - (bmEnemy.bmHeight/10) - yEn / 2, bmHpBar.bmWidth, bmHpBar.bmHeight,
							(HDC)hpDC, 0, 0, bmHpBar.bmWidth, bmHpBar.bmHeight, RGB(255, 255, 255));
			int c = wsprintf(buff, L"%d%s%d", enHP, L" / ", maxHpEn);
			SetBkMode(cwinDC, TRANSPARENT);
			hfont = ::CreateFontIndirect(&font);
			SelectObject(cwinDC, hfont);
			TextOut(cwinDC, rWindow.left + xEn + bmHpBar.bmWidth / 2 - 25, (rWindow.bottom - bm.bmHeight * 2) - (bmEnemy.bmHeight / 10) - yEn / 2 + bmHpBar.bmHeight / 3, buff, c);
			}
			if (attack) {
				if (mRight) {
					bmpHit = (HBITMAP)LoadImage(NULL, L"res/hit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					bmpTemp = (HBITMAP)SelectObject(hitDC, bmpHit);
					if (bmpTemp) DeleteObject(bmpTemp);
					TransparentBlt(cwinDC, xPl + bmSteve.bmWidth - 30, (rWindow.bottom - bm.bmHeight * 3) - 50, bmHit.bmWidth / 2, bmHit.bmHeight / 2, hitDC, 0, 0, bmHit.bmWidth, bmHit.bmHeight, RGB(255, 255, 255) | RGB(255, 0, 0));
					if (xEn >= xPl && xEn <= xPl + bmSteve.bmWidth + bmHit.bmWidth) {
						enHP -= 5;
					}
				}
				if (mLeft) {
					bmpHit = (HBITMAP)LoadImage(NULL, L"res/hitL.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					bmpTemp = (HBITMAP)SelectObject(hitDC, bmpHit);
					if (bmpTemp) DeleteObject(bmpTemp);
					TransparentBlt(cwinDC, xPl, (rWindow.bottom - bm.bmHeight * 3) - 50, bmHit.bmWidth / 2, bmHit.bmHeight / 2, hitDC, 0, 0, bmHit.bmWidth, bmHit.bmHeight, RGB(255, 255, 255) | RGB(255, 0, 0));
					if (xEn >= xPl - bmHit.bmWidth && xEn <= xPl) {
						enHP -= 5;
					}
				}
				hitDraw = TRUE;
				rSteveUp.bottom = rWindow.bottom - bm.bmHeight * 3;
				rSteveUp.left = xPl;
				rSteveUp.right = rWindow.right - (xPl + bmSteve.bmWidth);
				rSteveUp.top = rWindow.top + (yPl - bmSteve.bmHeight);
				
			}
			else
				if (hitDraw == TRUE && timeGetTime() - t >= 120) {
					InvalidateRect(hWnd, &rSteveUp, FALSE);
					t = timeGetTime();
					hitDraw = FALSE;
				}
			
			TransparentBlt(cwinDC, rWindow.left + 10, rWindow.top + 10, bmHpBar.bmWidth * 4, bmHpBar.bmHeight * 2,
				(HDC)hpDC, 0, 0, bmHpBar.bmWidth, bmHpBar.bmHeight, RGB(255, 255, 255));
			int c = wsprintf(buff, L"%d%s%d", plHP, L" / ", maxHpPl);
			SetBkMode(cwinDC, TRANSPARENT);
			hfont = CreateFont(0, 0, 0, 0, 800, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			SelectObject(cwinDC, hfont);
			TextOut(cwinDC, rWindow.left + bmHpBar.bmWidth * 2 - 20, rWindow.top  + bmHpBar.bmHeight, buff, c);
			}
		if (inHouse) {
			fx += xPl / 20;
			fy -= yPl / 50;
			StretchBlt(cwinDC, fx, fy, rWindow.right + 100, rWindow.bottom + 80, fonDC, 0, 0, 1920, 1080, SRCCOPY);
			TransparentBlt(cwinDC, -20, -20, rWindow.right + 50, rWindow.bottom - 200, houseWallDC, 0, 0, bmHouseWall.bmWidth, bmHouseWall.bmHeight, RGB(255,255,255));
			TransparentBlt(cwinDC, -20, rWindow.bottom - 240, rWindow.right + 50, rWindow.bottom, houseFloorDC, 0, 0, bmHouseFloor.bmWidth, bmHouseFloor.bmHeight, RGB(255, 255, 255));

			if (mRight)
				TransparentBlt(cwinDC, rWindow.left + xPl, (rWindow.bottom - bm.bmHeight * 20) - (yPl - 12), bmSteve.bmWidth * 10, bmSteve.bmHeight * 10, steveDC, 0, 0, bmSteve.bmWidth, bmSteve.bmHeight, RGB(255, 255, 255));
			if (mLeft)
				TransparentBlt(cwinDC, rWindow.left + xPl, (rWindow.bottom - bm.bmHeight * 20) - (yPl - 12), bmSteve.bmWidth * 10, bmSteve.bmHeight * 10, steveRDC, 0, 0, bmSteve.bmWidth, bmSteve.bmHeight, RGB(255, 255, 255));

		}
		
		InvalidateRect(hWnd, &rWindow, FALSE);
		Sleep(60);
	}
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrewInstance, LPSTR lpszCmdParam, int nCmdShow) 
{
	WNDCLASS			WndClass;
	MSG					Msg;
	LPCWSTR szClassName = L"Game";

	WndClass.style			= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= (WNDPROC)GameWndProc;
	WndClass.hInstance		= hInstance;
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName	= szClassName;
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpszMenuName	= NULL;
	WndClass.cbClsExtra		= 0;
	WndClass.cbWndExtra		= 0;


	if (!RegisterClass(&WndClass)) {
		MessageBox(NULL, (LPCWSTR)L"Cannot register class", (LPCWSTR)L"Error", MB_OK);
		return 0;
	}
	
	hWnd = CreateWindowEx(WS_EX_TOPMOST, szClassName, (LPCWSTR)L"Game",
		WS_MAXIMIZE | WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX) | WS_BORDER,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, (LPCWSTR)L"Cannot create window", (LPCWSTR)L"Error", MB_OK);
		return 0;
	}

	GetClientRect(hWnd, &rClient);
	rWindow = rClient;
	AdjustWindowRectEx(&rWindow, WS_OVERLAPPED | WS_MAXIMIZE | WS_EX_STATICEDGE, FALSE, 0);

	mainHinstace = hInstance;
	hThread = CreateThread(NULL, 0, threadDraw, NULL, NULL, NULL);

	font.lfHeight = 10;// Устанавливает высоту шрифта или символа
	font.lfWidth = 0;// Устанавливает среднюю ширину символов в шрифте
	font.lfEscapement = 0;// Устанавливает угол, между вектором наклона и осью X устройства
	font.lfOrientation = 0;// Устанавливает угол, между основной линией каждого символа и осью X устройства
	font.lfWeight = 800;// Устанавливает толщину шрифта в диапазоне от 0 до 1000
	font.lfItalic = 0;// Устанавливает курсивный шрифт
	font.lfUnderline = 0;// Устанавливает подчеркнутый шрифт
	font.lfStrikeOut = 0;// Устанавливает зачеркнутый шрифт
	font.lfCharSet = RUSSIAN_CHARSET;// Устанавливает набор символов
	font.lfOutPrecision = 0;// Устанавливает точность вывода
	font.lfClipPrecision = 0;// Устанавливает точность отсечения
	font.lfQuality = 0;// Устанавливает качество вывода
	font.lfPitchAndFamily = 0;// Устанавливает ширину символов и семейство шрифта
	wsprintf(font.lfFaceName, L"VERDANA");//  устанавливает название шрифта

	hfont = ::CreateFontIndirect(&font);

	ShowWindow(hWnd, 3);
	UpdateWindow(hWnd);

	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	ExitProcess(0);
}


LRESULT CALLBACK GameWndProc(HWND funcHWND, UINT Message, UINT wParam, LONG lParam) {
	PAINTSTRUCT PaintStruct;
	BITMAP		bm;
	
	int msgResult;

	GetObject(hBitmap, sizeof(bm), &bm);
	switch (Message)
	{
	case WM_DESTROY: {
		if (hThread) {
			TerminateThread(hThread, 0);
			if (hBitmap) DeleteObject(hBitmap);
			if (hSteve) DeleteObject(hSteve);
			if (bmpMem) DeleteObject(bmpMem);
			if (bmpTree) DeleteObject(bmpTree);
			if (bmpStone) DeleteObject(bmpStone);
			if (bmpFon) DeleteObject(bmpFon);
			if (bmpHpBar) DeleteObject(bmpHpBar);
			if (bmpFireBall) DeleteObject(bmpFireBall);
			for (int i = 0; i < 5; i++)
				if (bmpEnemy[i]) DeleteObject(bmpEnemy[i]);

			if (cwinDC) DeleteDC(cwinDC);
			if (groundDC) DeleteDC(groundDC);
			if (steveDC) DeleteDC(steveDC);
			if (treeDC) DeleteDC(treeDC);
			if (stoneDC) DeleteDC(stoneDC);
			if (fonDC) DeleteDC(fonDC);
			if (hpDC) DeleteDC(hpDC);
			if (fireBallDC) DeleteDC(fireBallDC);
			for (int i = 0; i < 5; i++)
				if (enemyDC) DeleteDC(enemyDC[i]);
			ExitProcess(0);
		}
	}break;
	case WM_PAINT:
		GetClientRect(hWnd, &rWindow);
		winDC = BeginPaint(hWnd, &PaintStruct);
		StretchBlt(
			winDC,
			rWindow.left,
			rWindow.top,
			rWindow.right,
			rWindow.bottom,
			cwinDC,
			rWindow.left,
			rWindow.top,
			rWindow.right,
			rWindow.bottom,
			SRCCOPY
		);
		EndPaint(hWnd, &PaintStruct);
		return 0;
		break;
	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_UP) < 0 && onGround == TRUE || GetKeyState(VK_SPACE) && onGround == TRUE || LOWORD(wParam)=='w' && onGround == TRUE || LOWORD(wParam) == 'W' && onGround == TRUE) {
			jump = TRUE;
			onGround = FALSE;
		}
		if (GetAsyncKeyState(VK_RIGHT) < 0 || LOWORD(wParam) == 'd' || LOWORD(wParam) == 'D') {
			!inHouse ? (xPl + 64 >= rWindow.right) ? xPl = rWindow.right - 64 : xPl += 10 : (xPl + 190 >= rWindow.right) ? xPl = rWindow.right - 190 : xPl += 10;
			mRight = TRUE;
			mLeft = FALSE;
		}
		if (GetAsyncKeyState(VK_LEFT) < 0 || LOWORD(wParam) == 'a' || LOWORD(wParam) == 'A') {
			!inHouse ? (xPl - 24 <= rWindow.left) ? xPl = rWindow.left : xPl -= 10 : (xPl + 190 <= rWindow.left) ? xPl = rWindow.left - 190 : xPl -= 10;
			mRight = FALSE;
			mLeft = TRUE;
		}
		if (LOWORD(wParam) == 'f' || LOWORD(wParam) == 'F') {
			attack = TRUE;
		}

		if (LOWORD(wParam) == 'r' || LOWORD(wParam) == 'R') {
			msgResult = MessageBox(hWnd, L"Do you really wan't restart game ?", L"Caution, game restart !", MB_YESNO);
			if (msgResult == 6) {
				xPl = 0;
				yPl = 32 * 3;
				if (inFight) inFight = FALSE;
				if (inHouse) {
					inHouse = FALSE; 
					lvl--;
					levels[lvl] = TRUE;
				}
				enHP = 150;
			}
		}

		if (GetKeyState(VK_ESCAPE) < 0) {
			MessageBox(hWnd, L"Menu body", L"Main menu", NULL);
		}

		return 0;
		break;
	case WM_KEYUP:
		if (LOWORD(wParam) == 'f' || LOWORD(wParam) == 'F') {
			attack = FALSE;
		}
		return 0;
		break;
	default: return DefWindowProc(funcHWND, Message, wParam, lParam);
	}
}

int main(HINSTANCE hInstance, HINSTANCE hPrewInstance,
	LPSTR lpszCmdParam, int nCmdShow) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	WinMain(hInstance, hPrewInstance,
			lpszCmdParam, nCmdShow);
	return 0;
}