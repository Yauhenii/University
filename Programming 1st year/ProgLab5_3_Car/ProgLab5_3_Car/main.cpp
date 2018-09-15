#pragma warning(disable:4996)
#undef UNICODE
#define _USE_MATH_DEFINES
#include <windows.h>
#include <cmath>
#include "KWnd.h"

#define CAR_IS_OUT_LEFT (iCurrentOffsetX-sx/5)>sx
#define CAR_IS_OUT_RIGHT iCurrentOffsetX < 0

const int INTERVAL = 50;
const int COEFFICIENT = 600;
static double r_angle = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRGN& CreateCarRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime);
HRGN& CreateCarBodyRgn(HDC hDC, int x, int y, int width, int height, double angle);
HRGN& CreateCarWheelsRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime);
double RotateX(int x, int y, double angle);
double RotateY(int x, int y, double angle);

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
	static int t = 0;
	//pen
	static HPEN hPenCommon;
	//brushed
	static HBRUSH hBrushCurrent;
	//region
	static HRGN hCar;
	//car
	static POINT* pCarCoord = new POINT[4];
	static int iCurrentOffsetX;
	static int iCurrentOffsetY;
	static int iCurrentSpeedX;
	static int iCurrentSpeedY;
	static double dCarAngle;

	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_CREATE:
		//pen
		hPenCommon = (HPEN)GetStockObject(1);
		//car
		iCurrentOffsetX = 0;
		iCurrentSpeedX = 1;
		iCurrentOffsetY = 0;
		iCurrentSpeedY = 0;
		dCarAngle = 0;
		//set timer
		SetTimer(hWnd, 1, INTERVAL, NULL);
		break;
	case WM_PAINT:
		//check for stop
		if (iCurrentSpeedX == 0)
		{
			iCurrentSpeedY = 0;
		}
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//
		SelectObject(hDC,hPenCommon);
		//set modes
		SetMapMode(hDC, MM_ANISOTROPIC);
		SetWindowExtEx(hDC, sx, sy, NULL);
		SetViewportExtEx(hDC, sx, -sy, NULL);
		SetViewportOrgEx(hDC, 0, sy, NULL);
		//draw
		hCar = CreateCarRgn(hDC, 0, 0, sx / 5, sy / 6, dCarAngle, iCurrentSpeedX, t);
		if (CAR_IS_OUT_LEFT)
		{
			iCurrentOffsetX = -sx / 5;
			iCurrentOffsetY = sy / 6;
		}
		OffsetRgn(hCar, iCurrentOffsetX, iCurrentOffsetY - sy);
		//color
		if (CAR_IS_OUT_RIGHT)
		{
			hBrushCurrent = CreateSolidBrush(RGB(255, 0, 255));
		}
		else
		{
			hBrushCurrent = CreateSolidBrush(RGB(255, (255 * iCurrentOffsetX) / sx, 255));
		}
		FillRgn(hDC, hCar, hBrushCurrent);
		DeleteObject(hBrushCurrent);
		//detete region
		DeleteObject(hCar);
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		//Offset
		iCurrentOffsetX += iCurrentSpeedX;
		iCurrentOffsetY += iCurrentSpeedY;
		t++;
		if (iCurrentSpeedX != 0)
		{
			dCarAngle = atan((0.1*iCurrentSpeedY) / (0.1*iCurrentSpeedX));
		}
		else
		{
			dCarAngle = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
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
				SetTimer(hWnd, 1, INTERVAL, NULL);
				paused = false;
			}
			break;
		case VK_RIGHT:
			iCurrentSpeedX++;
			break;
		case VK_LEFT:
			if (iCurrentSpeedX != 0)
			{
				iCurrentSpeedX--;
			}
			break;
		case VK_UP:
			iCurrentSpeedY++;
			break;
		case VK_DOWN:
			iCurrentSpeedY--;
			break;
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
		//timer
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HRGN& CreateCarRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime)
{
	HRGN hRgn, hWheels;
	hRgn = CreateCarBodyRgn(hDC, x, y, width, height, angle);
	hWheels = CreateCarWheelsRgn(hDC, x, y, width, height, angle, iSpeed, iTime);
	//make finite region	
	CombineRgn(hRgn, hRgn, hWheels, RGN_DIFF);
	return hRgn;
}
double RotateX(int x, int y, double angle)
{
	return (x * cos(angle) + y * sin(angle)); 
}
double RotateY(int x, int y, double angle)
{
	return (-x * sin(angle) + y * cos(angle)); 
}

#define LEFT_DOWN x,y-0.4*height,angle
#define LEFT_UP x,y-0.8*height,angle
#define WINDOW_UP x+0.6*width,y-0.8*height,angle
#define WINDOW_DOWN x+0.7*width,y-0.6*height,angle
#define WINDOW_RIGHT x+width,y-0.6*height,angle
#define RIGHT_DOWN x+width,y-0.4*height,angle 

#define FIRST_BODY_WHEEL x+0.1*width,y-0.2*height,angle
#define SECOND_BODY_WHEEL x+0.9*width,y-0.2*height,angle

HRGN& CreateCarBodyRgn(HDC hDC, int x, int y, int width, int height, double angle) 
{
	HRGN hBody;
	//begin
	BeginPath(hDC);
	//draw body
	MoveToEx(hDC, (int)RotateX(LEFT_DOWN), (int)RotateY(LEFT_DOWN), NULL);
	LineTo(hDC, (int)RotateX(LEFT_UP), (int)RotateY(LEFT_UP));
	LineTo(hDC, (int)RotateX(WINDOW_UP), (int)RotateY(WINDOW_UP));
	LineTo(hDC, (int)RotateX(WINDOW_DOWN), (int)RotateY(WINDOW_DOWN));
	LineTo(hDC, (int)RotateX(WINDOW_RIGHT), (int)RotateY(WINDOW_RIGHT));
	LineTo(hDC, (int)RotateX(RIGHT_DOWN), (int)RotateY(RIGHT_DOWN));
	LineTo(hDC, (int)RotateX(LEFT_DOWN), (int)RotateY(LEFT_DOWN));
	//draw wheels
	MoveToEx(hDC, (int)RotateX(FIRST_BODY_WHEEL), (int)RotateY(FIRST_BODY_WHEEL), NULL);
	AngleArc(hDC, (int)RotateX(FIRST_BODY_WHEEL), (int)RotateY(FIRST_BODY_WHEEL), 0.2*height, 0, 360);
	MoveToEx(hDC, (int)RotateX(SECOND_BODY_WHEEL), (int)RotateY(SECOND_BODY_WHEEL), NULL);
	AngleArc(hDC, (int)RotateX(SECOND_BODY_WHEEL), (int)RotateY(SECOND_BODY_WHEEL), 0.2*height, 0, 360);
	CloseFigure(hDC);
	//end
	EndPath(hDC);
	//make region
	hBody = PathToRegion(hDC);
	return hBody;
}
HRGN& CreateCarWheelsRgn(HDC hDC, int x, int y, int width, int height, double angle, int iSpeed, int iTime)
{
	//init r_angle
	if (iSpeed != 0) 
	{
		r_angle += COEFFICIENT * (iSpeed) / 360; //rotation angle
	}
	HRGN hWheels;
	//begin
	BeginPath(hDC);
	//draw wheels
		//first
	MoveToEx(hDC, (int)RotateX(FIRST_BODY_WHEEL), (int)RotateY(FIRST_BODY_WHEEL), NULL);
	AngleArc(hDC, (int)RotateX(FIRST_BODY_WHEEL), (int)RotateY(FIRST_BODY_WHEEL), 0.15*height, r_angle, 15);
	MoveToEx(hDC, (int)RotateX(SECOND_BODY_WHEEL), (int)RotateY(SECOND_BODY_WHEEL), NULL);
	AngleArc(hDC, (int)RotateX(SECOND_BODY_WHEEL), (int)RotateY(SECOND_BODY_WHEEL), 0.15*height, r_angle, (15));
		//second
	CloseFigure(hDC);
	//end
	EndPath(hDC);
	//make region
	hWheels = PathToRegion(hDC);
	return hWheels;
}
