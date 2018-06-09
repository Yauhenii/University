#include <Windows.h>
#include "KWnd.h"
#include "Lesson.h"
#include <string>
#include <vector>
#include <fstream>
#include <tchar.h>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void read_file(vector<Lesson>& lessons, string file_name);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd((LPCWSTR)"Test Class KWnd app", hInstance, nCmdShow, WndProc, NULL, 50, 100, 200, 150);

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
	RECT rect;
	int userReply;
	//const char[]
	char cHeader[] = "Schedule\0";
	char cDays[6][12] = { "Monday\0","Tuesday\0","Wednesday\0","Thursday\0","Friday\0","Saturday\0" };
	//sx,sy
	static int sx, sy;
	//pen
	static HPEN hPenCommon;
	//brushes
	static HBRUSH hBrushDay, hBrushLect, hBrushPract, hBrushTrain;
	//fonts
	static HFONT hFontHeader, hFontDay, hFontLesson;
	//vector for lessons
	static vector<Lesson> lessons;
	//temp values

	switch (uMsg) {
	case WM_CREATE:
		//read file
		read_file(lessons, "lessons_a.txt");
		//create pen
		hPenCommon = (HPEN)GetStockObject(BLACK_PEN);
		//create brushes
		hBrushDay = CreateSolidBrush(RGB(200, 100, 50));
		hBrushLect = CreateSolidBrush(RGB(100, 50, 200));
		hBrushPract = CreateSolidBrush(RGB(200, 50, 100));
		hBrushTrain = CreateSolidBrush(RGB(100, 100, 100));
		//create fonts
		hFontHeader = (HFONT)GetStockObject(ANSI_VAR_FONT);
		hFontDay = (HFONT)GetStockObject(ANSI_VAR_FONT);
		hFontLesson = (HFONT)GetStockObject(ANSI_VAR_FONT);
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		//start paint
		hDC = BeginPaint(hWnd, &ps);
		SetBkMode(hDC, TRANSPARENT);
		//header
		SelectObject(hDC, hFontHeader);
		SetTextAlign(hDC, TA_CENTER | VTA_CENTER);
		TextOut(hDC, sx / 2, 0, (LPCWSTR)cHeader, strlen(cHeader));
		//draw days
		SelectObject(hDC, hPenCommon);
		SelectObject(hDC, hBrushDay);
		SetTextAlign(hDC, TA_LEFT | TA_TOP);
		SelectObject(hDC, hFontDay);
		for (int i = 0; i < 6; i++)
		{
			Rectangle(hDC, 0, (i + 1)*sy / 7, sx / 6, (i + 2)*sy / 7);
			SetRect(&rect, 0, (i + 1)*sy / 7, sx / 6, (i + 2)*sy / 7);
			DrawText(hDC, (LPCWSTR)cDays[i], strlen(cDays[i]), &rect, DT_VCENTER | DT_CENTER);
		}
		//draw lessons
		for (int i = 0; i < lessons.size(); i++)
		{
			if (lessons[i].get_type()=="lecture") 
				SelectObject(hDC, hBrushLect);
			else if (lessons[i].get_type() == "practice")
				SelectObject(hDC, hBrushPract);
			else
				SelectObject(hDC, hBrushTrain);

			Rectangle(hDC, (atoi(lessons[i].get_less_num().c_str()))*sx / 6,
				(atoi(lessons[i].get_day().c_str()))*sy / 7,
				(atoi(lessons[i].get_less_num().c_str()) + 1)*sx / 6,
				(atoi(lessons[i].get_day().c_str()) + 1)*sy / 7);
			SetRect(&rect, (atoi(lessons[i].get_less_num().c_str()))*sx / 6,
				(atoi(lessons[i].get_day().c_str()))*sy / 7,
				(atoi(lessons[i].get_less_num().c_str()) + 1)*sx / 6,
				(atoi(lessons[i].get_day().c_str()) + 1)*sy / 7);
			DrawText(hDC, (LPCWSTR)lessons[i].get_less().c_str(), strlen(cDays[i]), &rect, DT_LEFT | DT_BOTTOM);
		}
		//end paint
		EndPaint(hWnd, 0);
		break;
	case WM_CLOSE:
		userReply = MessageBox(hWnd, (LPCWSTR)"Do you want to exit the app?", (LPCWSTR)"Notification", MB_YESNO | DT_VCENTER);
		if (IDYES == userReply)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		//delete pen
		DeleteObject(hPenCommon);
		//delete brushes
		DeleteObject(hBrushDay);
		DeleteObject(hBrushLect);
		DeleteObject(hBrushPract);
		DeleteObject(hBrushTrain);
		//delete fonts
		DeleteObject(hFontHeader);
		DeleteObject(hFontDay);
		DeleteObject(hFontLesson);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void read_file(vector<Lesson>& lessons, string file_name)
{
	//Open file
	fstream ifile;
	ifile.open(file_name);
	//Read input.txt
	if (ifile.is_open())
	{
		//Make temp values
		string t_day = "";
		string t_less_num = "";
		string t_less = "";
		string t_type = "";
		string t_room_num = "";
		//Start reading
		while (!ifile.eof())
		{
			ifile >> t_day >> t_less_num >> t_less >> t_type >> t_room_num;
			Lesson t_kless(t_day, t_less_num, t_less, t_type, t_room_num);
			lessons.push_back(t_kless);
		}
	}
	//End reading
	ifile.close();
}