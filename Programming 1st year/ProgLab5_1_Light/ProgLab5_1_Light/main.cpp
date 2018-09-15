#pragma warning(disable:4996)
#undef UNICODE
#include <windows.h>
#include "KWnd.h"

#define FirstPedestrianLed Rectangle(hDC, 4.1 * sx / 9, 1.1*sy / 10, 4.9 * sx / 9, 2.9 * sy / 10)
#define SecondPedestrianLed Rectangle(hDC, 4.1 * sx / 9, 3.1*sy / 10, 4.9 * sx / 9, 4.9 * sy / 10)

#define FirstRoadLed Ellipse(hDC, 3 * sx / 18, 2 * sy / 10, 5 * sx / 18, 4 * sy / 10)
#define SecondRoadLed Ellipse(hDC, 3 * sx / 18, 4 * sy / 10, 5 * sx / 18, 6 * sy / 10)
#define ThirdRoadLed Ellipse(hDC, 3 * sx / 18, 6 * sy / 10, 5 * sx / 18, 8 * sy / 10)

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

const int INTERVAL = 110;

bool isOdd(int i);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd("MyWin", hInstance, nCmdShow, WndProc);
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
	//timer pause
	static bool paused = false;
	//sx,sy
	static int sx, sy;
	//timer t
	static int t=0;
	//pen
	static HPEN hPenCommon;
	//brushes
	static HBRUSH hBrushBlack, hBrushWhite, hBrushRed, hBrushYellow, hBrushGreen;
	//rect 
	static RECT rectFirst;
	static RECT rectSecond;
	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		SetRect(&rectFirst, sx / 9, sy / 10, 3 * sx / 9, 9 * sy / 10);
		SetRect(&rectSecond, 4 * sx / 9, sy / 10, 5 * sx / 9, 5 * sy / 10);
		break;
	case WM_CREATE:
		//pen
		hPenCommon = (HPEN)GetStockObject(1);
		//brushes
		hBrushBlack = CreateSolidBrush(RGB(0, 0, 0)); 
		hBrushWhite = CreateSolidBrush(RGB(255, 255, 255)); 
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0)); 
		hBrushYellow = CreateSolidBrush(RGB(255, 165, 0)); 
		hBrushGreen = CreateSolidBrush(RGB(0, 255, 0)); 
		SetTimer(hWnd, 1, INTERVAL, NULL);
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//draw light
		SelectObject(hDC, hPenCommon);
		//draw base
		SelectObject(hDC, hBrushBlack);
		Rectangle(hDC, sx / 9, sy / 10, 3* sx / 9, 9 * sy / 10);
		Rectangle(hDC, 4*sx / 9, sy / 10, 5 * sx / 9, 5 * sy / 10);
		//draw leds
		if (t <= 0.4* INTERVAL) {
			SelectObject(hDC, hBrushRed);
			FirstRoadLed;
			SelectObject(hDC, hBrushWhite);
			SecondRoadLed;
			ThirdRoadLed;
			FirstPedestrianLed;
			SelectObject(hDC, hBrushGreen);
			SecondPedestrianLed;
		}
		if (t <= 0.5* INTERVAL && t > 0.4* INTERVAL) {
			SelectObject(hDC, hBrushRed);
			FirstRoadLed;
			SelectObject(hDC, hBrushYellow);
			SecondRoadLed;
			SelectObject(hDC, hBrushWhite);
			ThirdRoadLed;
			FirstPedestrianLed;
			if (isOdd(t)) {
				SelectObject(hDC, hBrushGreen);
			}
			else {
				SelectObject(hDC, hBrushWhite);
			}
			SecondPedestrianLed;
		}
		if (t <= 0.8* INTERVAL && t > 0.5* INTERVAL) {
			SelectObject(hDC, hBrushRed);
			FirstPedestrianLed;
			SelectObject(hDC, hBrushWhite);
			FirstRoadLed;
			SecondRoadLed;
			SecondPedestrianLed;
			SelectObject(hDC, hBrushGreen);
			ThirdRoadLed;
		}
		if (t <= 0.9*INTERVAL && t > 0.8* INTERVAL) {
			SelectObject(hDC, hBrushRed);
			FirstPedestrianLed;
			SelectObject(hDC, hBrushWhite);
			FirstRoadLed;
			SecondRoadLed;
			SecondPedestrianLed;
			if (isOdd(t)) {
			SelectObject(hDC, hBrushGreen);
			}
			else {
				SelectObject(hDC, hBrushWhite);
			}
			ThirdRoadLed;
		}
		if (t <= INTERVAL && t > 0.9* INTERVAL) {
			SelectObject(hDC, hBrushRed);
			FirstPedestrianLed;
			SelectObject(hDC, hBrushWhite);
			FirstRoadLed;
			ThirdRoadLed;
			SecondPedestrianLed;
			SelectObject(hDC, hBrushYellow);
			SecondRoadLed;
		}
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (t >= INTERVAL)
			t = 0;
		t++;
		InvalidateRect(hWnd, &rectFirst , TRUE); 
		InvalidateRect(hWnd, &rectSecond, TRUE);
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
	case WM_CLOSE:
		userReply = MessageBox(hWnd, "Are you sure?", "", MB_YESNO | MB_ICONQUESTION);
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
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool isOdd(int i)
{
	return i & 1;
}
