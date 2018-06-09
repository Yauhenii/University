#pragma warning(disable:4996)
#undef UNICODE
#define _USE_MATH_DEFINES
#include <windows.h>
#include <string>
#include "resource.h"
#include "KWnd.h"
#include <commdlg.h>
#include <cmath>

using namespace std;

const int WIDTH = 314;
const int HEIGHT = 200;
//brush
HBRUSH hBrush;
//color
static CHOOSECOLOR ccs;
static COLORREF acrCustClr[16];
COLORREF stdColor = RGB(255, 255, 255);
double i = 15;
int k = 3;

void updateRegion(HRGN& hRgn, int left, int top, int right, int bottom);
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
const int d_INTERVAL = 50;
int INTERVAL = d_INTERVAL;
int t_INTERVAL;

//bool isOdd(int i);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static HACCEL hAccel;
	MSG msg;
	KWnd mainWnd("MyWin", hInstance, nCmdShow, WndProc, MAKEINTRESOURCE(IDR_MENU3));
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
	//coordinates
	static int sx, sy;
	static int x_scr, y_scr;
	double x;
	//ball coord 
	//timer t
	static bool paused = false;
	static int t = 0;
	//pens
	static HPEN hPenAxis, hPenGraphic;
	//regions
	static HRGN ellipse;
	//angle for ball
	static double ballAngle = -M_PI;
	//direction
	static bool forward = true;
	//menu
	static HMENU hMenu;
	//dialog
	static HINSTANCE hInst;

	switch (message) {
	case WM_CREATE:
		//create dialog
		hInst = GetModuleHandle(NULL);
		//create menu
		hMenu = GetMenu(hWnd);
		//create pens
		hPenAxis = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		hPenGraphic = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		//create brush
		hBrush = CreateSolidBrush(RGB(255, 165, 0));
		//create timer
		SetTimer(hWnd, 1, INTERVAL, NULL);
		//create rgn
		ellipse = CreateEllipticRgn(sx, sy, sx, sy);
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//set modes
		SetMapMode(hDC, MM_ANISOTROPIC);
		SetWindowExtEx(hDC, 2 * WIDTH, -2 * HEIGHT, NULL);
		SetViewportExtEx(hDC, 3 * sx / 4, 3 * sy / 4, NULL);
		SetViewportOrgEx(hDC, sx / 2, sy / 2, NULL);
		////draw axis
		//SelectObject(hDC, hPenAxis);
		//MoveToEx(hDC, -WIDTH, 0, NULL);
		//LineTo(hDC, WIDTH, 0);
		//MoveToEx(hDC, 0, HEIGHT, NULL);
		//LineTo(hDC, 0, -HEIGHT);
		////draw graphic
		//SelectObject(hDC, hPenGraphic);
		//MoveToEx(hDC, -WIDTH, 0, NULL);
		//for (x = -M_PI, x_scr = -WIDTH; x < M_PI; x += 0.02, x_scr += 2)
		//{
		//	y_scr = HEIGHT * sin(x);
		//	LineTo(hDC, x_scr, y_scr);
		//}
		//draw ball
		SelectObject(hDC, hBrush);
		x_scr = t - WIDTH;
		y_scr = HEIGHT * sin(ballAngle);
		updateRegion(ellipse, x_scr - i, y_scr - i, x_scr + i, y_scr + i);
		FillRgn(hDC, ellipse, hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		paused = !paused;
		break;
	case WM_TIMER:
		if (INTERVAL != t_INTERVAL && (INTERVAL <= d_INTERVAL) && (INTERVAL >= 0)) {
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, INTERVAL, NULL);
		}
		if (!paused)
		{
			if (forward)
			{
				t += 3;
				ballAngle += 0.03;
			}
			else if (!forward)
			{
				t -= 3;
				ballAngle -= 0.03;
			}
			if (t >= 2 * WIDTH) {
				forward = false;
			}
			else if (t <= 0) {
				forward = true;
			}
			InvalidateRgn(hWnd, NULL, true);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ABOUT:
			MessageBox(hWnd,"Sinus ball\nVersion 1.0\nCreated by Yauhen", "About", MB_OK);
			break;
		case ID_EDIT_COLOR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
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
		//delete pens
		DeleteObject(hPenAxis);
		DeleteObject(hPenGraphic);
		//delete brush
		DeleteObject(hBrush);
		//delete timer
		KillTimer(hWnd, 1);
		//delete rgn
		DeleteObject(ellipse);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void updateRegion(HRGN& hRgn, int left, int top, int right, int bottom) {
	DeleteObject(hRgn);
	hRgn = CreateEllipticRgn(left, top, right, bottom);
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit; 
	switch (uMsg) {
	case WM_INITDIALOG:
		//color
		ccs.lStructSize = sizeof(CHOOSECOLOR);
		ccs.hwndOwner = GetParent(hDlg);
		ccs.rgbResult = stdColor;
		ccs.Flags = CC_RGBINIT | CC_FULLOPEN; ccs.lpCustColors = (LPDWORD)acrCustClr;
		//edit
		hEdit = GetDlgItem(hDlg, IDC_EDIT1);
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
				if (hBrush)
					DeleteObject(hBrush);
				hBrush = CreateSolidBrush(stdColor);
			}
			break;
		case IDC_BUTTON2:
			i = GetDlgItemInt(hDlg, IDC_EDIT1, FALSE, NULL);
			return TRUE;
		case IDC_BUTTON3:
			t_INTERVAL = INTERVAL;
			INTERVAL = d_INTERVAL - GetDlgItemInt(hDlg, IDC_EDIT2, FALSE, NULL);
			return TRUE;
		case IDOK:
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

