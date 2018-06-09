#pragma warning(disable:4996)
#undef UNICODE
#define _USE_MATH_DEFINES
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include "resource.h"
#include "KWnd.h"

using namespace std;

const int Q_ANSWERS = 5;
const int Q_OPTIONS = 5;
bool isEnded = false;
char* result_str = new char[10];

//for checked dialog
UINT currentCheckStat[3] = { BST_UNCHECKED ,BST_UNCHECKED ,BST_UNCHECKED };
//for questions
	//answers
bool answers[Q_ANSWERS][Q_OPTIONS];
bool** correctAnswers;
	//
int currentID[Q_ANSWERS][Q_OPTIONS];
//functions
	//extra functions
void read_file(bool** answers, string file_name);
int count_result();
	//main functions
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//test questions functions
BOOL CALLBACK AboutDlgProcQ1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProcQ2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProcQ3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProcQ4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProcQ5(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//winMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static HACCEL hAccel;
	MSG msg;
	KWnd mainWnd("MyWin", hInstance, nCmdShow, WndProc, MAKEINTRESOURCE(IDR_MENU1), 0, 0, 0, 0);
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
	//menu
	static HMENU hMenu;
	//dialog
	static HINSTANCE hInst;
	//answers 
	//test
	char str1[] = "PRESS RIGHT BUTTON TO START TEST";
	char str2[] = "EXIT TEST";
	static bool isStarted;
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

	switch (message)
	{
	case WM_GETMINMAXINFO:
		lpMMI->ptMinTrackSize.x = 500;
		lpMMI->ptMinTrackSize.y = 400;

		lpMMI->ptMaxTrackSize.x = 500;
		lpMMI->ptMaxTrackSize.y = 400;
	
		break;
	case WM_CREATE:
		//start test
		for (int i = 0; i < Q_ANSWERS; i++)
		{
			for (int j = 0; j < Q_OPTIONS; j++)
			{
				answers[i][j] = false;
			}
		}
		isStarted = false;
		//create dialog
		hInst = GetModuleHandle(NULL);
		//create menu
		hMenu = GetMenu(hWnd);
		//fill correctAnswers array
		correctAnswers = new bool*[Q_ANSWERS];
		for (int i = 0; i < Q_ANSWERS; i++)
			correctAnswers[i] = new bool[Q_OPTIONS];
		read_file(correctAnswers, "ANSWERS.txt");
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		SetTextAlign(hDC, TA_CENTER | VTA_CENTER);
		if (isStarted)
		{
			TextOut(hDC, 250, 100, str2, strlen(str2));
			SendMessage(hWnd, WM_CLOSE, wParam, lParam);
		}
		else
		{
			TextOut(hDC, 250, 100, str1, strlen(str1));
		}
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		if (!isStarted)
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
		else
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AboutDlgProcQ1);
		isStarted = true;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ABOUT:
			MessageBox(hWnd,"Neural networks test\nVersion 1.0\nCreated by Yauhen", "About", MB_OK);
			break;
		case ID_TEST_STARTTEST:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			break;
		case ID_TEST_EXIT:
			SendMessage(hWnd, WM_CLOSE, wParam, lParam);
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
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDSTARTTEST:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, AboutDlgProcQ1);
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

BOOL CALLBACK AboutDlgProcQ1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		if(currentID[0][0])
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, currentID[0][0]);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			currentID[0][0] = IDC_RADIO1;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[0][i] = false;
			answers[0][0] = true;
			return TRUE;
		case IDC_RADIO2:
			currentID[0][0] = IDC_RADIO2;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[0][i] = false;
			answers[0][1] = true;
			return TRUE;
		case IDC_RADIO3:
			currentID[0][0] = IDC_RADIO3;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[0][i] = false;
			answers[0][2] = true;
			return TRUE;
		case IDNEXT:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hDlg, AboutDlgProcQ2);
			return TRUE;
		case IDPREV:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProcQ2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		if (currentID[1][0])
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, currentID[1][0]);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			currentID[1][0] = IDC_RADIO1;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO1);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[1][i] = false;
			answers[1][0] = true;
			return TRUE;
		case IDC_RADIO2:
			currentID[1][0] = IDC_RADIO2;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO2);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[1][i] = false;
			answers[1][1] = true;
			return TRUE;
		case IDC_RADIO3:
			currentID[1][0] = IDC_RADIO3;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO3);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[1][i] = false;
			answers[1][2] = true;
			return TRUE;
		case IDC_RADIO4:
			currentID[1][0] = IDC_RADIO4;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO4);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[1][i] = false;
			answers[1][3] = true;
			return TRUE;
		case IDNEXT:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hDlg, AboutDlgProcQ3);
			return TRUE;
		case IDPREV:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, AboutDlgProcQ1);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProcQ3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		if (currentID[2][0])
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, currentID[2][0]);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			currentID[2][0] = IDC_RADIO1;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO1);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[2][i] = false;
			answers[2][0] = true;
			return TRUE;
		case IDC_RADIO2:
			currentID[2][0] = IDC_RADIO2;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO2);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[2][i] = false;
			answers[2][1] = true;
			return TRUE;
		case IDNEXT:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), hDlg, AboutDlgProcQ4);
			return TRUE;
		case IDPREV:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hDlg, AboutDlgProcQ2);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProcQ4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const int q_ans = 3;
	static int answer[q_ans];
	static HINSTANCE hInst;
	currentID[3][0] = IDC_CHECK1;
	currentID[3][1] = IDC_CHECK2;
	currentID[3][2] = IDC_CHECK3;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		for (int i = 0; i < q_ans; i++)
			CheckDlgButton(hDlg, currentID[3][i], currentCheckStat[i]);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CHECK1:
			if (answers[3][0])
			{
				currentCheckStat[0] = BST_UNCHECKED;
				answers[3][0] = false;
			}
			else
			{
				currentCheckStat[0] = BST_CHECKED;
				answers[3][0] = true;
			}
			return TRUE;
		case IDC_CHECK2:
			if (answers[3][1])
			{
				currentCheckStat[1] = BST_UNCHECKED;
				answers[3][1] = false;
			}
			else
			{
				currentCheckStat[1] = BST_CHECKED;
				answers[3][1] = true;
			}
			return TRUE;
		case IDC_CHECK3:
			if (answers[3][2])
			{
				currentCheckStat[2] = BST_UNCHECKED;
				answers[3][2] = false;
			}
			else
			{
				currentCheckStat[2] = BST_CHECKED;
				answers[3][2] = true;
			}
			return TRUE;
		case IDNEXT:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6), hDlg, AboutDlgProcQ5);
			return TRUE;
		case IDPREV:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hDlg, AboutDlgProcQ3);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProcQ5(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	int userReply;
	string str = "Your result is: ";
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		if (currentID[4][0])
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, currentID[4][0]);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			currentID[4][0] = IDC_RADIO1;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[4][i] = false;
			answers[4][0] = true;
			return TRUE;
		case IDC_RADIO2:
			currentID[4][0] = IDC_RADIO2;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[4][i] = false;
			answers[4][1] = true;
			return TRUE;
		case IDC_RADIO3:
			currentID[4][0] = IDC_RADIO3;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
			for (int i = 0; i < Q_OPTIONS; i++)
				answers[4][i] = false;
			answers[4][2] = true;
			return TRUE;
		case IDNEXT:
			userReply = MessageBox(hDlg, "Do you want to end the test?", "End test", MB_YESNO | MB_ICONQUESTION);
			if (IDYES == userReply)
			{
				isEnded = true;
				EndDialog(hDlg, 0);
				itoa(count_result(), result_str, 10);
				string res(result_str);
				str += res;
				MessageBox(hDlg, str.c_str(), "Result", MB_OK | MB_ICONWARNING);
				return TRUE;
			}
			return TRUE;
		case IDPREV:
			EndDialog(hDlg, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), hDlg, AboutDlgProcQ4);
			return TRUE;
		default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

void read_file(bool** answers, string file_name) {
	//Open file
	fstream ifile;
	ifile.open(file_name);
	//Read input.txt
	if (ifile.is_open()) {
		//Make temp values
		bool temp;
		//Start reading
		for (int i = 0; i < Q_ANSWERS; i++)
		{
			for (int j = 0; j < Q_OPTIONS; j++)
			{
				ifile >> temp;
				answers[i][j] = temp;
			}
		}

	}
	//End reading
	ifile.close();
}

int count_result()
{
	int result = 5;
	bool corr, temp;
	for (int i = 0; i < Q_ANSWERS; i++)
	{
		for (int j = 0; j < Q_OPTIONS; j++)
		{
			corr = correctAnswers[i][j];
			temp = answers[i][j];
			if (temp != corr)
			{
				result -= 1;
				break;
			}
		}
	}
	return result;
}