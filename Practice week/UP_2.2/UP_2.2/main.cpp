#include <windows.h>
#include "resource.h"
#include <string>
#include <sstream>

using namespace std;

extern "C" float __cdecl compln();
extern "C" float __fastcall userln(int k);
string fltostr(float f);
string fillstr(float a, float b, int k);
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int userReply;
	//ListBox
	static HWND hListBox;
	int k;

	switch (msg)
	{
	case WM_INITDIALOG:
		//Init ListBox
		hListBox = GetDlgItem(hWin, IDC_LIST1);
		return TRUE;
	case WM_CLOSE:
		userReply = MessageBox(hWin, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
		if (IDYES == userReply)
		{
			EndDialog(hWin, 0);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			k= GetDlgItemInt(hWin, IDC_EDIT1, NULL, TRUE);
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)fillstr(compln(), userln(k), k).c_str());
			return TRUE;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	return 0;
}

string fltostr(float f) {
	stringstream stream;
	stream << f;
	return stream.str();
}

string fillstr(float a, float b, int k) {
	stringstream stream;
	stream << "Comp: " << a << ", user: " << b<<", iter: "<<k;
	return stream.str();
}