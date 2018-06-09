#include <Windows.h>
#include "KWnd.h"
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int f(int x, int k);
void fillString(string& str, double x, double y);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd("Test Class KWnd app", hInstance, nCmdShow, WndProc, NULL, 50, 100, 200, 150);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//system
	HDC hDC;
	PAINTSTRUCT ps;
	static RECT rect;
	int userReply;
		//coordinates
	//sx,sy
	static int sx, sy;
	//logic
	double xPh, yPh;
	//physical
	double xLo, yLo;
	//mouse
	int xMo, yMo;
	//with and height
	static const int width = 200;
	static const int height = 200;
	//coefficient
	int k = 10;
	//steps
	int graphStep = 1;
	int gridStep = 1;
	//string
	string coordText;
	//pens
	static HPEN hPenAxis, hPenGraphic, hPenGrid;
	//font
	static HFONT hFontCoord;

	switch (uMsg)
	{
	case WM_CREATE:
		//create pens
		hPenAxis = CreatePen(PS_SOLID, 0.5, RGB(0, 0, 0));
		hPenGraphic = CreatePen(PS_SOLID, 1.5, RGB(200, 0, 0));
		hPenGrid = CreatePen(PS_SOLID, 0.1, RGB(0, 200, 0));
		//create font
		hFontCoord = (HFONT)GetStockObject(ANSI_VAR_FONT);
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//draw graphic
			//coordinates
		xPh = sx;
		yPh = sy;
		xLo = width;
		yLo = height;
			//modes
		SetMapMode(hDC, MM_ANISOTROPIC);
		SetWindowExtEx(hDC, 2*xLo, 2*yLo, NULL);
		SetViewportExtEx(hDC, 9 * xPh / 10, -9 * yPh / 10, NULL);
		SetViewportOrgEx(hDC, 0.5*xPh, 0.5*yPh, NULL);
			//draw grid
		SelectObject(hDC, hPenGrid);
		for (int i = -xLo ; i <= xLo; i+=k * gridStep)
		{
			MoveToEx(hDC, i, -yLo, NULL);
			LineTo(hDC, i, yLo);
		}
		for (int i = -yLo; i <= yLo; i+= k * gridStep)
		{
			MoveToEx(hDC, -xLo, i, NULL);
			LineTo(hDC, xLo, i);
		}
			//draw axis
		SelectObject(hDC, hPenAxis);
		MoveToEx(hDC, -xLo, 0, NULL);
		LineTo(hDC, xLo, 0);
		MoveToEx(hDC, 0, -yLo, NULL);
		LineTo(hDC, 0, yLo);
			//draw graphic
		SelectObject(hDC, hPenGraphic);
		for (int i = -xLo; i < xLo; i += graphStep)
		{
			if ((f(i, k) / k) <= yLo)
			{
				MoveToEx(hDC, i, f(i, k)/k, NULL);
				break;
			}
		}
		for (int i = -xLo; i < xLo; i+=graphStep)
		{ 
			if ((f(i, k) / k) <= yLo)
			{
				LineTo(hDC, i, f(i, k) / k);
			}
		}
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		hDC = GetDC(hWnd);
		//get coord
		xMo = LOWORD(lParam);
		yMo = HIWORD(lParam);
		if (GetPixel(hDC, xMo, yMo) == RGB(200, 0, 0))
		{
			//physical coord
			xPh = 9 * sx / 10;
			yPh = 9 * sy / 10;
			//convert coord
			xLo = (xMo - sx / 2) / (k*xPh / (2 * width));
			yLo = ((sy / 2 - yMo) / (k*yPh / (2 * height)));
			fillString(coordText, round(xLo * 100) / 100, round(yLo * 100) / 100);
			SelectObject(hDC, hFontCoord);
			TextOut(hDC, xMo, yMo, coordText.c_str(), strlen(coordText.c_str()));
		}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_RBUTTONDOWN:
		rect.left = LOWORD(lParam);
		rect.top = HIWORD(lParam);
		break;
	case WM_RBUTTONUP:
		rect.right = LOWORD(lParam);
		rect.bottom = HIWORD(lParam);
		InvalidateRect(hWnd, &rect, TRUE);
		break;
	case WM_CLOSE:
		userReply = MessageBox(hWnd, "Do you want to exit the app?", "Notification", MB_YESNO | DT_VCENTER);
		if (IDYES == userReply)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		//delete pens
		DeleteObject(hPenAxis);
		DeleteObject(hPenGraphic);
		DeleteObject(hPenGrid);
		//delete font
		DeleteObject(hFontCoord);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int f(int x, int k) 
{
	return (0.5*x * x + 6 * k * x + k * k);
}

void fillString(string& str, double x, double y)
{
	stringstream stream;
	stream << "[" << x << "," << y << "]";
	str = stream.str();
}

