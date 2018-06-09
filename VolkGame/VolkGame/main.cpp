#pragma warning(disable:4996)
//#pragma comment (lib, "winmm.lib")

#undef UNICODE
#define _USE_MATH_DEFINES
#include <windows.h>
#include "KWnd.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "resource.h"
#include <cmath>
#include <ctime>
#include <mmsystem.h>
#include <map>
#include <algorithm>


#define BALL_SML 5
#define BALL_MED 10
#define BALL_BIG 15

#define X_IS_OUT iCurrentOffsetX - 60 >= LeftUpL.first

#define WM_GAME_OVER (WM_USER + 0x0001)

using namespace std;

//consts
const int LEVELS = 5;
const int INTERVAL[LEVELS] = { 250,200,150,50,20 };
const int WOLF_CONST_OFFSET = 90;
//wolf directions
const int WOLF_D = 4; //wolf directions 
					  //shelves
const pair<int, int> LeftUpH(0, 340);
const pair<int, int> LeftUpL(300, 420);
const pair<int, int> LeftDownH(0, 540);
const pair<int, int> LeftDownL(300, 620);
const pair<int, int> RightUpH(1000, 340);
const pair<int, int> RightUpL(700, 420);
const pair<int, int> RightDownH(1000, 540);
const pair<int, int> RightDownL(700, 620);

pair<int, int> ShelvesH[WOLF_D] = { LeftUpH ,LeftDownH ,RightUpH,RightDownH };
pair<int, int> ShelvesL[WOLF_D] = { LeftUpL ,LeftDownL ,RightUpL,RightDownL };

//global var
//pens
HPEN hPenShelves;
//brushes
static HBRUSH hBrushBall;
//color
static CHOOSECOLOR ccs;
static COLORREF acrCustClr[16];
COLORREF stdColor = RGB(255, 255, 255);
COLORREF stdColorPen = RGB(255, 0, 0);
//current size
int currentBallSize = 1; // 0 - small, 1 - medum, 2 - big
int ballHeight = BALL_MED; // 5 - small, 10 - medum, 15 - big
int currentID[3] = { IDC_RADIO1,IDC_RADIO2,IDC_RADIO3 };
//current level
int currentLevel = 0;
int prevLevel = 0;
//game stat
bool gameIsOver = false;
bool wantToExit = true;
//score and health
int score;
int health;
//temp name
char* name = new char[40];
//pause game
bool paused = 0;
//start game 
bool gameIsStarted = 0;
//sound
bool soundIsOn = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int setCurrentWolf(bool left, bool up); //choose wolf type
void DrawShelves(HDC &hDC, HPEN hPen);
HRGN& CreateBallRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime);
HRGN& CreateBallBodyRgn(HDC hDC, int x, int y, int width, int height, double angle);
HRGN& CreateBallSpaceRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime);
double RotateX(int x, int y, double angle);
double RotateY(int x, int y, double angle);
int setLevel(int score, int currentLevel);
bool setGameOver(int health);
void writeFile(string file_name, int score, string player_name);
void read_file(string file_name, multimap<int, string>& mmap);
void fillString(string& str, double x, double y);
void fillString(string& str, string x, int y);
void fillString(string& str, int y, string x);

BOOL CALLBACK AboutDlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd("MyWin", hInstance, nCmdShow, WndProc, MAKEINTRESOURCE(IDR_MENU1), 0, 0, 1000, 800);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	int userReply;
	//sx,sy
	static int sx, sy;
	//timer 
	static int t = 0;
	//ball
	static HRGN hBall;
	//offset and speed
	static int iCurrentOffsetX;
	static int iCurrentOffsetY;
	static int iCurrentSpeedX;
	static int iCurrentSpeedY;
	//0 1 2 3 <=> LeftUp LeftDown RightUp RightDown
	//wolf directions
	static bool left, up;
	static int currentWolf;
	//shelves options
	static int currentShelf;
	int k; //direction of shelf offset
	//wolf bitmaps
	HBITMAP hWolfBitmap[WOLF_D];
	static HDC memBit[WOLF_D];
	static BITMAP bm[WOLF_D];
	string WolfBMP[WOLF_D] = { "WolfLeftUp450-420.bmp" ,"WlofLeftDown450-420.bmp" ,"WolfRightUp450-420.bmp" ,"WolfRightDown450-420.bmp" };
	//life bmp
	HBITMAP hLifeBmp;
	static HDC memBitLife;
	static BITMAP bmLife;
	//flag bmp
	HBITMAP hFlagBmp;
	static HDC memBitFlag;
	static BITMAP bmFlag;
	//heads bmp
	HBITMAP hHeadBmp[2];
	static HDC memBitHead[2];
	static BITMAP bmHead[2];
	string HeadsBMP[2] = { "lenin.bmp" ,"stalin.bmp" };
	//menu
	static HMENU hMenu;
	static HMENU hMenuContext;
	//dialog
	static HINSTANCE hInst;
	//strings
	char lifes[] = "Lifes:";
	char startGame[] = "Start game";
	char exitGame[] = "Exit";
	static string stat_str;
	//fonts
	static HFONT hFontLife;
	static LOGFONT lfLife;
	static HFONT hFontMainMenu;
	static LOGFONT lfMainMenu;
	//point for mouse
	static POINT mousePoint;

	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_CREATE:
	{
		//create dialog
		hInst = GetModuleHandle(NULL);
		//create menu
		hMenu = GetMenu(hWnd);
		hMenuContext = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
		hMenuContext = GetSubMenu(hMenuContext, 0);
		//load wolf images
		for (int i = 0; i < WOLF_D; i++)
		{
			hWolfBitmap[i] = (HBITMAP)LoadImage(NULL, WolfBMP[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hWolfBitmap[i], sizeof(bm[i]), &bm[i]);
			hDC = GetDC(hWnd);
			memBit[i] = CreateCompatibleDC(hDC);
			SelectObject(memBit[i], hWolfBitmap[i]);
			ReleaseDC(hWnd, hDC);
		}
		//load life image
		hLifeBmp = (HBITMAP)LoadImage(NULL, "life.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetObject(hLifeBmp, sizeof(bmLife), &bmLife);
		hDC = GetDC(hWnd);
		memBitLife = CreateCompatibleDC(hDC);
		SelectObject(memBitLife, hLifeBmp);
		ReleaseDC(hWnd, hDC);
		//load flag image
		hFlagBmp = (HBITMAP)LoadImage(NULL, "flag.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetObject(hFlagBmp, sizeof(bmFlag), &bmFlag);
		hDC = GetDC(hWnd);
		memBitFlag = CreateCompatibleDC(hDC);
		SelectObject(memBitFlag, hFlagBmp);
		ReleaseDC(hWnd, hDC);
		//load heads image
		for (int i = 0; i < 2; i++)
		{
			hHeadBmp[i] = (HBITMAP)LoadImage(NULL, HeadsBMP[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hHeadBmp[i], sizeof(bmHead[i]), &bmHead[i]);
			hDC = GetDC(hWnd);
			memBitHead[i] = CreateCompatibleDC(hDC);
			SelectObject(memBitHead[i], hHeadBmp[i]);
			ReleaseDC(hWnd, hDC);
		}
		//init logfont
		lfLife.lfCharSet = DEFAULT_CHARSET;
		lfLife.lfPitchAndFamily = DEFAULT_PITCH;
		strcpy_s(lfLife.lfFaceName, "My Font");
		lfLife.lfWeight = FW_NORMAL;
		lfLife.lfEscapement = 0;
		lfLife.lfHeight = 30;
		//init logfont
		lfMainMenu.lfCharSet = DEFAULT_CHARSET;
		lfMainMenu.lfPitchAndFamily = DEFAULT_PITCH;
		strcpy_s(lfMainMenu.lfFaceName, "My Font");
		lfMainMenu.lfWeight = FW_NORMAL;
		lfMainMenu.lfEscapement = 0;
		lfMainMenu.lfHeight = 60;
		//create brushes
		hBrushBall = CreateSolidBrush(RGB(255, 255, 255));
		//create pen
		hPenShelves = CreatePen(PS_SOLID, 8, stdColorPen);
		//set default wolf directions
		left = true;
		up = true;
		currentWolf = 0;
		//offset and speed
		iCurrentOffsetX = 0;
		iCurrentSpeedX = 15;
		iCurrentOffsetY = 0;
		iCurrentSpeedY = 4;
		//set default score and health 
		score = 0;
		health = 3;
		//set default shelve
		currentShelf = 0;
		//set timer
		SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
		break;
	}
	case WM_GAME_OVER:
	{
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, AboutDlgProc3);
		if (wantToExit)
		{
			SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
			health = 3;
			currentLevel = 0;
			prevLevel = 3;
			gameIsOver = false;
			string player_name(name);
			if (player_name=="")
				player_name = "DefaultPlayer";
			writeFile("Statistics.txt", score, player_name);
			score = 0;
		}
		else if (!wantToExit)
		{
			SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
			string player_name(name);
			DestroyWindow(hWnd);
		}
		break;
	}
	case WM_PAINT:
	{
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//check for game started
		if (!gameIsStarted)
		{
			//stop time
			KillTimer(hWnd, 1);
			//create font
			hFontMainMenu = CreateFontIndirect(&lfMainMenu);
			//select font
			SelectObject(hDC, hFontMainMenu);
			//set background 
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(47, 79, 79));
			SetTextAlign(hDC, TA_CENTER | VTA_CENTER);
			TextOut(hDC, sx / 2, sy / 2+50, startGame, strlen(startGame));
			BitBlt(hDC, (sx- bmFlag.bmWidth )/2, (sy)/2 - bmFlag.bmHeight +50, bmFlag.bmWidth, bmFlag.bmHeight, memBitFlag, 0, 0, SRCCOPY);
			//delete font
			DeleteObject(hFontMainMenu);
			break;
		}
		//check for game over
		gameIsOver = setGameOver(health);
		if (gameIsOver)
		{
			KillTimer(hWnd, 1);
			SendMessage(hWnd, WM_GAME_OVER, wParam, lParam);
			break;
		}
		//draw
		//set background 
		SetBkMode(hDC, TRANSPARENT);
		//create font
		hFontLife = CreateFontIndirect(&lfLife);
		//draw lenin
		BitBlt(hDC, sx / 2 - 30, 30, bmHead[0].bmWidth, bmHead[0].bmHeight, memBitHead[0], 0, 0, SRCCOPY);
		//draw lenin
		BitBlt(hDC, sx / 2 - bmHead[0].bmWidth - 40, 30, bmHead[0].bmWidth, bmHead[0].bmHeight, memBitHead[1], 0, 0, SRCCOPY);
		//draw lifes
		SetTextColor(hDC, RGB(47, 79, 79));
		SelectObject(hDC, hFontLife);
		TextOut(hDC, sx - 3 * bmLife.bmWidth, 0, lifes, strlen(lifes));
		fillString(stat_str, "Your score:", score);
		TextOut(hDC, sx - 3 * bmLife.bmWidth, bmLife.bmWidth + 30, stat_str.c_str(), strlen(stat_str.c_str()));
		fillString(stat_str, "Current level:", (currentLevel + 1));
		TextOut(hDC, sx - 3 * bmLife.bmWidth, bmLife.bmWidth + 60, stat_str.c_str(), strlen(stat_str.c_str()));
		DeleteObject(hFontLife);
		for (int i = 0; i < health; i++)
		{
			BitBlt(hDC, sx - 3 * bmLife.bmWidth + i * bmLife.bmWidth - 10, 30, bmLife.bmWidth, bmLife.bmHeight, memBitLife, 0, 0, SRCCOPY);
		}
		//draw wolf
		if (currentWolf == 0 || currentWolf == 1)
		{
			BitBlt(hDC, (sx - bm[currentWolf].bmWidth) / 2, sy - bm[currentWolf].bmHeight, bm[currentWolf].bmWidth, bm[currentWolf].bmHeight, memBit[currentWolf], 0, 0, SRCCOPY);
		}
		else if (currentWolf == 2 || currentWolf == 3)
		{
			BitBlt(hDC, (sx + WOLF_CONST_OFFSET - bm[currentWolf].bmWidth) / 2, sy - bm[currentWolf].bmHeight, bm[currentWolf].bmWidth, bm[currentWolf].bmHeight, memBit[currentWolf], 0, 0, SRCCOPY);
		}

		//draw shelves
		DrawShelves(hDC, hPenShelves);
		//draw ball
		//change level
		currentLevel = setLevel(score, currentLevel);
		if (currentLevel != prevLevel)
		{
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
			prevLevel = currentLevel;
		}
		//choose shelf
		if (currentShelf == 0 || currentShelf == 1)
			k = 1;
		else
			k = -1;
		hBall = CreateBallRgn(hDC, ShelvesH[currentShelf].first, ShelvesH[currentShelf].second - ballHeight, 0, ballHeight, 0, 0, 0);
		OffsetRgn(hBall, k * iCurrentOffsetX, iCurrentOffsetY);
		FillRgn(hDC, hBall, hBrushBall);
		if (X_IS_OUT)
		{
			if (currentWolf == currentShelf)
			{
				score++;
				if (soundIsOn)
					PlaySound(TEXT("Sound_11086-2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else
			{
				health--;
				if (soundIsOn)
					PlaySound(TEXT("Sound_11104-2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			iCurrentOffsetX = 0;
			iCurrentOffsetY = 0;
			srand(time(0));
			currentShelf = rand() % 4;
		}
		//end paint
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
			if (!paused)
			{
				KillTimer(hWnd, 1);
				paused = true;
			}
			else if (paused)
			{
				SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
				paused = false;
			}
			break;
		case VK_LEFT:
			if (!left) {
				left = true;
				currentWolf = setCurrentWolf(left, up);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_RIGHT:
			if (left) {
				left = false;
				currentWolf = setCurrentWolf(left, up);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_UP:
			if (!up) {
				up = true;
				currentWolf = setCurrentWolf(left, up);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_DOWN:
			if (up) {
				up = false;
				currentWolf = setCurrentWolf(left, up);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
		if (!gameIsStarted)
		{
			gameIsStarted = true;
			SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
		}
		break;
	case WM_RBUTTONDOWN:
	{
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);
		//check for point in region
		if (PtInRegion(hBall, mousePoint.x, mousePoint.y))
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc1);
		else
		{
			TrackPopupMenuEx(hMenuContext, 0, mousePoint.x, mousePoint.y, hWnd, NULL);
		}
		if (GetPixel(hDC, LOWORD(lParam), HIWORD(lParam)) == stdColorPen) //RGB(255,0,0))
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AboutDlgProc2);
		}
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_GAME_STARTNEWGAME:
			SetTimer(hWnd, 1, INTERVAL[currentLevel], NULL);
			health = 3;
			currentLevel = 0;
			prevLevel = 0;
			gameIsOver = false;
			iCurrentOffsetX = 0;
			iCurrentOffsetY = 0;
			score = 0;
			break;
		case ID_GAME_EXIT:
			userReply = MessageBox(hWnd, "Are you sure?", "", MB_YESNO | MB_ICONQUESTION);
			if (IDYES == userReply)
			{
				DestroyWindow(hWnd);
			}
			break;
		case ID_ABOUT:
			MessageBox(hWnd, "True USSR wolf\nVersion 1.0\nCreated by Yauhen", "About", MB_OK);
			break;
		case ID_EDIT_CHANGEBALL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc1);
			break;
		case ID_EDIT_CHANGESHELVES:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AboutDlgProc2);
			break;
		case ID_STATICTICS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, AboutDlgProc4);
		case ID_SOUND_ON:
			soundIsOn = 1;
			break;
		case ID_SOUND_OFF:
			soundIsOn = 0;
			break;
		case ID_HELP:
			MessageBox(hWnd, "1)Move wolf using arrow keys\n2)Left click on the ball\nwill call change menu\n3)Press space to stop the game", "Help", MB_OK);
			break;
		}
		break;
	case WM_CLOSE:
		userReply = MessageBox(hWnd, "Are you sure?", "", MB_YESNO | MB_ICONQUESTION);
		if (IDYES == userReply)
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		//delete brushes
		DeleteObject(hBrushBall);
		//delete pen
		DeleteObject(hPenShelves);
		//timer
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	case WM_TIMER:
	{
		//Offset
		iCurrentOffsetX += iCurrentSpeedX;
		iCurrentOffsetY += iCurrentSpeedY;
		t++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int setCurrentWolf(bool left, bool up)
{
	if (left && up)
		return 0;
	if (left && !up)
		return 1;
	if (!left && up)
		return 2;
	if (!left && !up)
		return 3;
	/*if (left) {
	if (up)
	return 0;
	else
	return 1;
	}
	else {
	if (up)
	return 3;
	else
	return 4;
	}*/
	return -1;
}

double RotateX(int x, int y, double angle)
{
	return (x * cos(angle) + y * sin(angle));
}
double RotateY(int x, int y, double angle)
{
	return (-x * sin(angle) + y * cos(angle));
}

void DrawShelves(HDC &hDC, HPEN hPen) {
	SelectObject(hDC, hPen);
	//left up 
	MoveToEx(hDC, LeftUpH.first, LeftUpH.second, NULL);
	LineTo(hDC, LeftUpL.first, LeftUpL.second);
	//left down
	MoveToEx(hDC, LeftDownH.first, LeftDownH.second, NULL);
	LineTo(hDC, LeftDownL.first, LeftDownL.second);
	//right up
	MoveToEx(hDC, RightUpH.first, RightUpH.second, NULL);
	LineTo(hDC, RightUpL.first, RightUpL.second);
	//right down
	MoveToEx(hDC, RightDownH.first, RightDownH.second, NULL);
	LineTo(hDC, RightDownL.first, RightDownL.second);
}

//HRGN& CreateBallRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime)
//{
//	HRGN hBallBody, hBallSpace;
//	hBallBody = CreateBallBodyRgn(hDC, x, y, width, height, angle);
//	hBallSpace = CreateBallSpaceRgn(hDC, x, y, width, height, angle, iSpeed, iTime);
//	//make finite region	
//	CombineRgn(hBallBody, hBallBody, hBallSpace, RGN_DIFF);
//	return hBallBody;
//}

HRGN& CreateBallRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime)
{
	HRGN hBall;
	//begin
	BeginPath(hDC);
	//draw wheels
	//first
	AngleArc(hDC, x, y, height, 0, 360);
	CloseFigure(hDC);
	//end
	EndPath(hDC);
	//make region
	hBall = PathToRegion(hDC);
	return hBall;
}

HRGN& CreateBallBodyRgn(HDC hDC, int x, int y, int width, int height, double angle)
{
	HRGN hBall;
	//begin
	BeginPath(hDC);
	//draw wheels
	//first
	AngleArc(hDC, x, y, height, 0, 360);
	CloseFigure(hDC);
	//end
	EndPath(hDC);
	//make region
	hBall = PathToRegion(hDC);
	return hBall;
}

HRGN& CreateBallSpaceRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime)
{
	HRGN hBall;
	//begin
	BeginPath(hDC);
	//draw wheels
	MoveToEx(hDC, x, y, NULL);
	//AngleArc(hDC, x, y, height, r_angle, 15);
	CloseFigure(hDC);
	//end
	EndPath(hDC);
	//make region
	hBall = PathToRegion(hDC);
	return hBall;
}

void fillString(string& str, double x, double y)
{
	stringstream stream;
	stream << "[" << x << "," << y << "]";
	str = stream.str();
}

void fillString(string& str, string x, int y)
{
	stringstream stream;
	stream << x << " " << y;
	str = stream.str();
}

void fillString(string& str, int y, string x)
{
	stringstream stream;
	stream << y << " " << x;
	str = stream.str();
}

BOOL CALLBACK AboutDlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	switch (uMsg) {
	case WM_INITDIALOG:
		//def radio
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, currentID[currentBallSize]);
		//color
		ccs.lStructSize = sizeof(CHOOSECOLOR);
		ccs.hwndOwner = GetParent(hDlg);
		ccs.rgbResult = stdColor;
		ccs.Flags = CC_RGBINIT | CC_FULLOPEN; ccs.lpCustColors = (LPDWORD)acrCustClr;
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			if (ChooseColor(&ccs))
			{
				stdColor = ccs.rgbResult;
				if (hBrushBall)
					DeleteObject(hBrushBall);
				hBrushBall = CreateSolidBrush(stdColor);
			}
			return TRUE;
		case IDC_RADIO1:
			currentBallSize = 0;
			ballHeight = BALL_SML;
			return TRUE;
		case IDC_RADIO2:
			currentBallSize = 1;
			ballHeight = BALL_MED;
			return TRUE;
		case IDC_RADIO3:
			currentBallSize = 2;
			ballHeight = BALL_BIG;
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	switch (uMsg) {
	case WM_INITDIALOG:
		//color
		ccs.lStructSize = sizeof(CHOOSECOLOR);
		ccs.hwndOwner = GetParent(hDlg);
		ccs.rgbResult = stdColor;
		ccs.Flags = CC_RGBINIT | CC_FULLOPEN; ccs.lpCustColors = (LPDWORD)acrCustClr;
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			if (ChooseColor(&ccs))
			{
				stdColorPen = ccs.rgbResult;
				if (hPenShelves)
					DeleteObject(hPenShelves);
				hPenShelves = CreatePen(PS_SOLID, 8, stdColorPen);
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	switch (uMsg) {
	case WM_INITDIALOG:
		//edit
		hEdit = GetDlgItem(hDlg, IDC_EDIT1);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			wantToExit = false;
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_BUTTON1:
			GetDlgItemText(hDlg, IDC_EDIT1, name, 40);
			wantToExit = true;
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProc4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//mmap of players
	static multimap<int, string> PlayersStat;
	static HWND hListBox;

	switch (uMsg) {
	case WM_INITDIALOG:
	{
		read_file("Statistics.txt", PlayersStat);
		hListBox = GetDlgItem(hDlg, IDC_LIST1);
		for_each(PlayersStat.rbegin(), PlayersStat.rend(), [](pair<int, string> player) {
			string str;
			fillString(str, player.first, player.second);
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
		});
		return TRUE;
	}
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

int setLevel(int score, int currentLevel) 
{
	if (score == 5)
		return 1;
	else if (score == 10)
		return 2;
	else if (score == 15)
		return 3;
	else if (score == 20)
		return 4;
	else return currentLevel;
}

bool setGameOver(int health) 
{
	if (health < 1)
		return true;
	else
		return false;
}

void writeFile(string file_name, int score, string player_name) 
{
	//Open file
	ofstream ofile;
	ofile.open(file_name , std::ios::app);
	//Write
	ofile << "\n" << score << " " << player_name;
	//End reading
	ofile.close();
}

void read_file(string file_name, multimap<int, string>& mmap)
{
	//mmap.insert(pair<int,string>(1,"null"));
	mmap.clear();
	//Open file
	fstream ifile;
	ifile.open(file_name);
	//Read input.txt
	while (!ifile.eof())
	{
		int frst;
		string scnd;
		ifile >> frst >> scnd;
		pair<int, string> temp(frst, scnd);
		mmap.insert(temp);
	}
	//End reading
	ifile.close();
}

