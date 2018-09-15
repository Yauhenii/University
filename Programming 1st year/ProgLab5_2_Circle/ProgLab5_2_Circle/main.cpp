#pragma warning(disable:4996)
#undef UNICODE
#include <windows.h>
#include "resource.h"
#include <string>
#include "KWnd.h"
#include <commdlg.h>

using namespace std;

#define THIN 1
#define MEDIUM 3
#define THICK 5

#define MOUSE_IN_1ST_RGN PtInRegion(ellipse1, mousePoint.x, mousePoint.y)
#define MOUSE_IN_2ND_RGN PtInRegion(ellipse2 mousePoint.x, mousePoint.y)
#define MOUSE_IN_3RD_RGN PtInRegion(ellipse3 mousePoint.x, mousePoint.y)
#define MOUSE_IN_ANY_RGN (MOUSE_IN_1ST_RGN | MOUSE_IN_2ND_RGN | MOUSE_IN_3RD_RGN)

//color
static CHOOSECOLOR ccs;
static COLORREF acrCustClr[16];
static HBRUSH hBrushBall[3];
static HBRUSH hBrush1;
static HBRUSH hBrush2;
static HBRUSH hBrush3;
//select ball
int currentBall;
int currentID = IDC_RADIO1;
int currentBallWidth1 = THIN;
int currentBallWidth2 = THIN;
int currentBallWidth3 = THIN;
int currentWidthID1 = IDC_RADIO3;
int currentWidthID2 = IDC_RADIO3;
int currentWidthID3 = IDC_RADIO3;
//blinking
bool isEpeleptic1, isEpeleptic2, isEpeleptic3;
//regions
HRGN ellipse1, ellipse2, ellipse3;
const int INTERVAL = 110;

COLORREF stdColor = RGB(255, 255, 255);


void updateRegion(HRGN& hRgn, int left, int top, int right, int bottom);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool isOdd(int i);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static HACCEL hAccel;
	MSG msg;
	KWnd mainWnd("MyWin", hInstance, nCmdShow, WndProc, MAKEINTRESOURCE(IDR_MENU1));
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(mainWnd.getHWnd(), hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	int userReply;
	//timer pause
	static bool paused = false;
	//sx,sy
	static int sx, sy;
	//point for mouse
	static POINT mousePoint;
	//timer t
	static int t = 0;
	//pen
	static HPEN hPenCommon;
	//brushed
	static HBRUSH hBrushBlack, hBrushWhite, hBrushRed, hBrushYellow, hBrushGreen;
	//menu
	static HMENU hMenu;
	static HMENU hMenuContext;
	static HMENU hMenuContext1, hMenuContext2, hMenuContext3;
	//dialog
	static HINSTANCE hInst;

	switch (message)
	{
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		hMenuContext = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
		hMenuContext = GetSubMenu(hMenuContext, 0);
		hMenuContext1 = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU3));
		hMenuContext1 = GetSubMenu(hMenuContext1, 0);
		hMenuContext2 = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU4));
		hMenuContext2 = GetSubMenu(hMenuContext2, 0);
		hMenuContext3 = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU5));
		hMenuContext3 = GetSubMenu(hMenuContext3, 0);
		//default menu values
		CheckMenuRadioItem(GetSubMenu(hMenu, 2), ID_FIRST_ON, ID_FIRST_OFF, ID_FIRST_ON, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuRadioItem(GetSubMenu(hMenu, 2), ID_SECOND_ON, ID_SECOND_OFF, ID_SECOND_ON, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuRadioItem(GetSubMenu(hMenu, 2), ID_THIRD_ON, ID_THIRD_OFF, ID_THIRD_ON, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuRadioItem(GetSubMenu(hMenu, 1), ID_TIMER_START, ID_TIMER_STOP, ID_TIMER_START, MF_BYCOMMAND | MF_CHECKED);
		EnableMenuItem(GetSubMenu(hMenu, 1), ID_TIMER_START, MF_GRAYED);
		if(GetSubMenu(hMenu, 1)==NULL)
			MessageBox(hWnd, "?", "ERROR", MB_OK);
		//init values
		paused = false;
		isEpeleptic1 = false;
		isEpeleptic2 = false;
		isEpeleptic3 = false;
		//create regions
		ellipse1 = CreateEllipticRgn(sx / 10, 2 * sy / 10, 3 * sx / 10, 4 * sy / 10);
		ellipse2 = CreateEllipticRgn(4 * sx / 10, 2 * sy / 10, 6 * sx / 10, 4 * sy / 10);
		ellipse3 = CreateEllipticRgn(7 * sx / 10, 2 * sy / 10, 9 * sx / 10, 4 * sy / 10);
		//create pen
		hPenCommon = (HPEN)GetStockObject(1);
		//create brushes
		hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
		hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
		hBrushYellow = CreateSolidBrush(RGB(255, 160, 0));
		hBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
		//create timer
		SetTimer(hWnd, 1, INTERVAL, NULL);
		//def color
		hBrushBall[0] = hBrushGreen;
		hBrushBall[1] = hBrushYellow;
		hBrushBall[2] = hBrushRed;
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);
		//check for point in region
		if (PtInRegion(ellipse1, mousePoint.x, mousePoint.y)) {
			if (isEpeleptic1)
			{
				isEpeleptic1 = false;
			}
			else
			{
				isEpeleptic1 = true;
			}
		}
		if (PtInRegion(ellipse2, mousePoint.x, mousePoint.y)) {
			if (isEpeleptic2)
			{
				isEpeleptic2 = false;
			}
			else
			{
				isEpeleptic2 = true;
			}
		}
		if (PtInRegion(ellipse3, mousePoint.x, mousePoint.y)) {
			if (isEpeleptic3)
			{
				isEpeleptic3 = false;
			}
			else
			{
				isEpeleptic3 = true;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);
		//check for point in region
		if (PtInRegion(ellipse1, mousePoint.x, mousePoint.y)) {
			TrackPopupMenuEx(hMenuContext1, 0, mousePoint.x, mousePoint.y, hWnd, NULL);
		}
		else if (PtInRegion(ellipse2, mousePoint.x, mousePoint.y)) {
			TrackPopupMenuEx(hMenuContext2, 0, mousePoint.x, mousePoint.y, hWnd, NULL);
		}
		else if (PtInRegion(ellipse3, mousePoint.x, mousePoint.y)) {
			TrackPopupMenuEx(hMenuContext3, 0, mousePoint.x, mousePoint.y, hWnd, NULL);
		}
		else {
			TrackPopupMenuEx(hMenuContext, 0, mousePoint.x, mousePoint.y, hWnd, NULL);
		}
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//draw circles
		SelectObject(hDC, hPenCommon);
		if (isOdd(t) && isEpeleptic1) {
			updateRegion(ellipse1, sx / 10 + sx / 20, 2 * sy / 10 + sy / 20, 3 * sx / 10 - sx / 20, 4 * sy / 10 - sy / 20);
		}
		else {
			updateRegion(ellipse1, sx / 10, 2 * sy / 10, 3 * sx / 10, 4 * sy / 10);
		}
		FillRgn(hDC, ellipse1, hBrushBall[0]);
		FrameRgn(hDC, ellipse1, hBrushBlack, currentBallWidth1, currentBallWidth1);
		if (isOdd(t) && isEpeleptic2) {
			updateRegion(ellipse2, 4 * sx / 10 + sx / 20, 2 * sy / 10 + sy / 20, 6 * sx / 10 - sx / 20, 4 * sy / 10 - sy / 20);
		}
		else {
			updateRegion(ellipse2, 4 * sx / 10, 2 * sy / 10, 6 * sx / 10, 4 * sy / 10);
		}
		FillRgn(hDC, ellipse2, hBrushBall[1]);
		FrameRgn(hDC, ellipse2, hBrushBlack, currentBallWidth2, currentBallWidth2);
		if (isOdd(t) && isEpeleptic3) {
			updateRegion(ellipse3,7 * sx / 10 + sx / 20, 2 * sy / 10 + sy / 20, 9 * sx / 10 - sx / 20, 4 * sy / 10 - sy / 20);
		}
		else {
			updateRegion(ellipse3, 7 * sx / 10, 2 * sy / 10, 9 * sx / 10, 4 * sy / 10);
		}
		FillRgn(hDC, ellipse3, hBrushBall[2]);
		FrameRgn(hDC, ellipse3, hBrushBlack, currentBallWidth3, currentBallWidth3);
		//update regions
		updateRegion(ellipse1, sx / 10, 2 * sy / 10, 3 * sx / 10, 4 * sy / 10);
		updateRegion(ellipse2, 4 * sx / 10, 2 * sy / 10, 6 * sx / 10, 4 * sy / 10);
		updateRegion(ellipse3, 7 * sx / 10, 2 * sy / 10, 9 * sx / 10, 4 * sy / 10);
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (t >= INTERVAL)
			t = 0;
		//InvalidateRgn(hWnd, NULL, true);
		if (isEpeleptic1) {
			InvalidateRgn(hWnd, ellipse1, true);
		}
		if (isEpeleptic2) {
			InvalidateRgn(hWnd, ellipse2, true);
		}
		if (isEpeleptic3) {
			InvalidateRgn(hWnd, ellipse3, true);
		}
		t++;
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
		{
			if (!paused) {
				KillTimer(hWnd, 1);
				paused = true;
			}
			else if (paused) {
				SetTimer(hWnd, 1, INTERVAL, NULL);
				paused = false;
			}
			break;
		}
		}
		break;
	}
	case WM_CONTEXTMENU:
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ABOUT:
			MessageBox(hWnd, 
				"Epeleptic circles\nVersion 1.0\nCreated by Yauhen\n\nAccelerators:\nStart timer: Ctrl+S\nStop timer : Ctrl + P\nExit : Ctrl + E\n", "About", MB_OK );
			break;
		case ID_FIRST_ON:
			isEpeleptic1 = true;
			CheckMenuRadioItem(GetSubMenu(hMenu, 2), ID_FIRST_ON, ID_FIRST_OFF, LOWORD(wParam), MF_BYCOMMAND | MF_CHECKED);
			break;
		case ID_FIRST_OFF:
			isEpeleptic1 = false;
			CheckMenuRadioItem(GetSubMenu(hMenu, 2), ID_FIRST_ON, ID_FIRST_OFF, LOWORD(wParam), MF_BYCOMMAND | MF_CHECKED);
			break;
		case ID_SECOND_ON:
			isEpeleptic2 = true;
			break;
		case ID_SECOND_OFF:
			isEpeleptic2 = false;
			break;
		case ID_THIRD_ON:
			isEpeleptic3 = true;
			break;
		case ID_THIRD_OFF:
			isEpeleptic3 = false;
			break;
		case ID_TIMER_START:
			SetTimer(hWnd, 1, INTERVAL, NULL);
			paused = false;
			break;
		case ID_TIMER_STOP:
			KillTimer(hWnd, 1);
			paused = true;
			break;
		case ID_PROGRAM_EXIT:
			SendMessage(hWnd, WM_CLOSE, wParam, lParam);
			break;
		case ID_EDIT_BALLS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		userReply = MessageBox(hWnd, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
		if (IDYES == userReply)
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		//pen
		DeleteObject(hPenCommon);
		//brushes
		DeleteObject(hBrushBlack);
		DeleteObject(hBrushWhite);
		DeleteObject(hBrushRed);
		DeleteObject(hBrushYellow);
		DeleteObject(hBrushGreen);
		//delete regions
		DeleteObject(ellipse1);
		DeleteObject(ellipse2);
		DeleteObject(ellipse3);
		//delete timer
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		//menus
		DestroyMenu(hMenu);
		DestroyMenu(hMenuContext);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool isOdd(int i){
	return i & 1;
}

void updateRegion(HRGN& hRgn, int left, int top, int right, int bottom) {
	DeleteObject(hRgn);
	hRgn = CreateEllipticRgn(left, top, right, bottom);
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG: 
		//default buttons
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, currentID);
		currentBall = 1;
		CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO6, currentWidthID1);
		//create color
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
		case IDC_RADIO1:
			currentID = IDC_RADIO1;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			currentBall = 1;
			CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO6, currentWidthID1);
			break;
		case IDC_RADIO2:
			currentID = IDC_RADIO2;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
			currentBall = 2;
			CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO6, currentWidthID2);
			break;
		case IDC_RADIO3:
			currentID = IDC_RADIO3;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
			currentBall = 3;
			CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO6, currentWidthID3);
			break;
		case IDC_RADIO4:
			if (currentBall == 1) {
				currentBallWidth1 = THIN;
				currentWidthID1 = IDC_RADIO4;
			}
			else if (currentBall == 2) {
				currentBallWidth2 = THIN;
				currentWidthID2 = IDC_RADIO4;
			}
			else if (currentBall == 3) {
				currentBallWidth3 = THIN;
				currentWidthID3 = IDC_RADIO4;
			}
			break;
		case IDC_RADIO5:
			if (currentBall == 1) {
				currentBallWidth1 = MEDIUM;
				currentWidthID1 = IDC_RADIO5;
			}
			else if (currentBall == 2) {
				currentBallWidth2 = MEDIUM;
				currentWidthID2 = IDC_RADIO5;
			}
			else if (currentBall == 3) {
				currentBallWidth3 = MEDIUM;
				currentWidthID3 = IDC_RADIO5;
			}
			break;
		case IDC_RADIO6:
			if (currentBall == 1) {
				currentBallWidth1 = THICK;
				currentWidthID1 = IDC_RADIO6;
			}
			else if (currentBall == 2) {
				currentBallWidth2 = THICK;
				currentWidthID2 = IDC_RADIO6;
			}
			else if (currentBall == 3) {
				currentBallWidth3 = THICK;
				currentWidthID3 = IDC_RADIO6;
			}
			break;
		case IDC_BUTTON1:
			if (ChooseColor(&ccs))
			{
				stdColor = ccs.rgbResult;
				if (hBrushBall[currentBall-1])
					DeleteObject(hBrushBall[currentBall-1]);
				hBrushBall[currentBall-1] = CreateSolidBrush(stdColor);
			}
			break;
		case IDOK:
		case ID_CANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}