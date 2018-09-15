#pragma warning(disable:4996)
#undef UNICODE
#include <windows.h>
#include "KWnd.h"
#include "resource.h"
#include <string>
#include <sstream>

const int n = 10;
int arr[n] = { 1,2,8,2,6,3,2,11,2,5 };
int num = 0;
using namespace std;

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern "C" int __stdcall findCloseNum(int* arr, int n, int num);
extern "C" char* __fastcall atobinom(int a);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void set_arr(char* str);
void fillString(string& str,string phrase, int num);
void fillStringArr(string& str);
void fillString(string& str, string phrase, string s);

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
	//dialog
	static HINSTANCE hInst;
	//coordinates
	static int sx, sy;
	//strings
	static string head1, head2;
	//close num
	int cl;

	switch (message) {
	case WM_CREATE:
		//create dialog
		hInst = GetModuleHandle(NULL);
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		//begin paint
		hDC = BeginPaint(hWnd, &ps);
		//TextOut
		fillString(head1,"Number = ", num);
		TextOut(hDC, 0, 0, head1.c_str(), head1.length());
		fillStringArr(head2);
		TextOut(hDC, 0, sy/10, head2.c_str(), head2.length());
		//find close num
		cl = findCloseNum(arr, n, num);
		fillString(head1,"Closest number = ", cl);
		TextOut(hDC, 0, sy / 5, head1.c_str(), head1.length());
		{
			string s(atobinom(cl));
			fillString(head1, "Closest number in binary = ", s);
			TextOut(hDC, 0, 3 * sy / 10, head1.c_str(), head1.length());
		}
		//end paint
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
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
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	//string
	const int n = 128;
	static char str[n];
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
			break;
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, (LPSTR)str, n);
			num = GetDlgItemInt(hDlg, IDC_EDIT2, FALSE, NULL);
			set_arr(str);
			InvalidateRgn(GetParent(hDlg), NULL, true);
			break;
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

void set_arr(char* str_c) {
	string str(str_c);
	str += ' ';
	string temp = "";
	int k = 0;
	int len = (int)str.length();
	for (int i = 0; i < len; i++)
	{

		if (str[i] != ' ')
		{
			temp += str[i];
		}
		else
		{
			if (k == n)
				break;
			arr[k] = atoi(temp.c_str());
			temp = "";
			k++;
		}
	}
	for (int i = k; i<n; i++)
		arr[i] = 0;

}

void fillString(string& str, string phrase, int num)
{
	stringstream stream;
	stream << phrase << num;
	str = stream.str();
}

void fillStringArr(string& str) {
	stringstream stream;
	stream << "Array: ";
	for (int i = 0; i < n; i++) {
		stream << arr[i]<<" ";
	}
	str = stream.str();
}

void fillString(string& str, string phrase, string s) {
	stringstream stream;
	stream << phrase << s.substr(0,8);
	str = stream.str();
}