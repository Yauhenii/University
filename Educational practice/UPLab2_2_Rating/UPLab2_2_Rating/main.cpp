#pragma warning(disable:4996)
#undef UNICODE
#define _USE_MATH_DEFINES
#include <windows.h>
#include "KWnd.h"
#include <cmath>
#include <string>
#include <sstream>
#include <commdlg.h>
#include "resource.h"

using namespace std;
//consts 
const int Q_PARTICIPANT = 6;
const int PIE_CHART_WIDTH = 314;
const int PIE_CHART_HEIGHT = 314;
const int BAR_CHART_WIDTH = 24;
const int BAR_CHART_HEIGHT = 100;
//color
CHOOSECOLOR ccs;
COLORREF acrCustClr[16];
COLORREF stdColor = RGB(255, 255, 255);
HBRUSH hBrushArr[Q_PARTICIPANT];

int currentPart = 0;
int currentID = IDC_RADIO1;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawBarChart(HDC hDC);
void DrawPieChart(HDC hDC);
void fillString(string& str, string name, int rating);
string fromIntToString(int i);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd("MyWin", hInstance, nCmdShow, WndProc, MAKEINTRESOURCE(IDR_MENU1));
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
	//
	//coordinates
	static int sx, sy;
	static int x_scr1, y_scr1, x_scr2, y_scr2;
	//pens
	static HPEN hPenAxis, hPenGraphic;
	//font
	static HFONT hFontName;
	static LOGFONT lf;
	//data
	string sNames[Q_PARTICIPANT] = { "Smith","Alzheimer","Freud","Tesla","Black","Parkinson" };
	static int iRating[Q_PARTICIPANT] = { 76,30,22,54,70,95 };
	static int iRatingSum = 0;
	string info;
	//values for pie
	double angle;
	double r; //radius
	static double start;
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
		hPenAxis = CreatePen(PS_SOLID, 0.5, RGB(0, 0, 0));
		hPenGraphic = CreatePen(PS_SOLID, 0.5, RGB(50, 200, 80));
		//create brush
		hBrushArr[0] = CreateSolidBrush(RGB(255, 165, 0));
		hBrushArr[1] = CreateSolidBrush(RGB(0, 255, 165));
		hBrushArr[2] = CreateSolidBrush(RGB(165, 0, 255));
		hBrushArr[3] = CreateSolidBrush(RGB(165, 255, 0));
		hBrushArr[4] = CreateSolidBrush(RGB(0, 165, 255));
		hBrushArr[5] = CreateSolidBrush(RGB(255, 0, 165));
		//init logfont
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfPitchAndFamily = DEFAULT_PITCH;
		strcpy_s(lf.lfFaceName, "My Font");
		lf.lfWeight = FW_NORMAL;
		lf.lfEscapement = 900;
		//count rating sum
		for (int i = 0; i < Q_PARTICIPANT; i++)
		{
			iRatingSum +=iRating[i];
		}
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		//init logfont
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//create font
		lf.lfHeight = sx / 400;
		hFontName = CreateFontIndirect(&lf);
		//devide space
		MoveToEx(hDC, sx / 2, 0, NULL);
		LineTo(hDC, sx / 2, sy);
		//
		//Draw bar chart
		//
		//set modes
		SetMapMode(hDC, MM_ANISOTROPIC);
		SetWindowExtEx(hDC, BAR_CHART_WIDTH, BAR_CHART_HEIGHT, NULL);
		SetViewportExtEx(hDC, (sx / 2 - 90), -(sy - 90), NULL);
		SetViewportOrgEx(hDC, 0+15, sy-15, NULL);
		//draw axis
		SelectObject(hDC, hPenAxis);
		MoveToEx(hDC, 0, BAR_CHART_HEIGHT, NULL);
		LineTo(hDC, 0, 0);
		LineTo(hDC, BAR_CHART_WIDTH, 0);
		//draw graphic
		SelectObject(hDC, hPenGraphic);
		SelectObject(hDC, hFontName);
		SetTextAlign(hDC, TA_RIGHT | TA_TOP);
		SetBkMode(hDC, TRANSPARENT);
		for (int i = 0; i < Q_PARTICIPANT; i++) 
		{
			SelectObject(hDC, hBrushArr[i]);
			Rectangle(hDC, 4*i+1, iRating[i], 4*i+3, 0);
			TextOut(hDC, 4 * i + 2, iRating[i], fromIntToString(iRating[i]).c_str(), strlen(fromIntToString(iRating[i]).c_str()));
			TextOut(hDC, 4 * i + 3, iRating[i], sNames[i].c_str(), strlen(sNames[i].c_str()));
		}
		DeleteObject(hFontName);
		//
		//Draw pie chart
		//
		//create font
		lf.lfHeight = sx / 80;
		hFontName = CreateFontIndirect(&lf);
		//set modes
		SetMapMode(hDC, MM_ISOTROPIC);
		SetWindowExtEx(hDC, PIE_CHART_WIDTH, PIE_CHART_WIDTH, NULL);
		SetViewportExtEx(hDC, sx / 2, -(sy / 2), NULL);
		SetViewportOrgEx(hDC, 3 * sx / 4, sy / 2, NULL);
		//draw graphic
		SelectObject(hDC, hFontName);
		r = PIE_CHART_WIDTH / 2;
		start = 0;
		for (int i = 0; i < Q_PARTICIPANT; i++)
		{
			SelectObject(hDC, hBrushArr[i]);
			x_scr1 = r * cos(2 * M_PI* start / iRatingSum);
			y_scr1 = r * sin(2 * M_PI* start / iRatingSum);
			x_scr2 = r * cos(2 * M_PI* (iRating[i] + start) / iRatingSum);
			y_scr2 = r * sin(2 * M_PI* (iRating[i] + start) / iRatingSum);
			start += iRating[i];
			Pie(hDC, -PIE_CHART_WIDTH / 2, -PIE_CHART_WIDTH / 2, PIE_CHART_WIDTH / 2, PIE_CHART_WIDTH / 2, x_scr1, y_scr1, x_scr2, y_scr2);
			fillString(info, sNames[i], iRating[i]);
			TextOut(hDC, x_scr2, y_scr2, info.c_str(), strlen(info.c_str()));
		}
		//delete font
		DeleteObject(hFontName);
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_CHANGE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AboutDlgProc);
			break;
		}
		break;
		
	case WM_DESTROY:
		//delete pens
		DeleteObject(hPenAxis);
		DeleteObject(hPenGraphic);
		//delete brush
		for (int i = 0; i < Q_PARTICIPANT; i++)
		{
			DeleteObject(hBrushArr[i]);
		}
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void fillString(string& str, string name, int rating)
{
	stringstream stream;
	stream << name << " " << rating << "%";
	str = stream.str();
}
string fromIntToString(int i)
{
	stringstream stream;
	stream << i << "%";
	return stream.str();
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	string str = "";
	static HWND hEdit;
	TCHAR text[256];
	int i, k;
	switch (uMsg) {
	case WM_INITDIALOG:
		//default buttons
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, currentID);
		//create changable color
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
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, IDC_RADIO1);
			currentPart = 0;
			break;
		case IDC_RADIO2:
			currentID = IDC_RADIO1;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, IDC_RADIO2);
			currentPart = 1;
			break;
		case IDC_RADIO3:
			currentID = IDC_RADIO3;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, IDC_RADIO3);
			currentPart = 2;
			break;
		case IDC_RADIO4:
			currentID = IDC_RADIO4;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, IDC_RADIO4);
			currentPart = 3;
			break;
		case IDC_RADIO5:
			currentID = IDC_RADIO5;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, IDC_RADIO5);
			currentPart = 4;
			break;
		case IDC_RADIO6:
			currentID = IDC_RADIO6;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO6, IDC_RADIO6);
			currentPart = 5;
			break;
		case IDC_BUTTON1:
			if (ChooseColor(&ccs))
			{
				stdColor = ccs.rgbResult;
				if (hBrushArr[currentPart])
					DeleteObject(hBrushArr[currentPart]);
				hBrushArr[currentPart] = CreateSolidBrush(stdColor);
			}
			break;
		case IDOK:
			InvalidateRgn(GetParent(hDlg), NULL, true);
			return true;
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

