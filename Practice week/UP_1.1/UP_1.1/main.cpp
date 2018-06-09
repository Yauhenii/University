#include <windows.h>
#include "resource.h"
#include <commdlg.h>
#include <string>
#include <cstring>
using namespace std;

extern "C" char* __fastcall strtoken(char* s, char*& w);
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Reply
	int userReply;
	//Open
	static OPENFILENAME ofn;
	static char szFile[MAX_PATH];
	bool success;
	//ListBox
	static HWND hListBox;
	//Words
	char* str1 = new char[MAX_PATH];
	char* str2 = new char[MAX_PATH];
	char* word = new char[MAX_PATH];

	switch (msg)
	{
	case WM_INITDIALOG:
		//Init open
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWin;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
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
			EndDialog(hWin, 0);
			return TRUE;
		case IDCANCEL:
			userReply = MessageBox(hWin, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
			if (IDYES == userReply)
			{
				EndDialog(hWin, 0);
			}
			return TRUE;
		case IDC_BUTTON1:
			strcpy_s(szFile, "");
			success = GetOpenFileName(&ofn);
			if (success)
			{
				strcpy_s(str1, strlen(str1), ofn.lpstrFile);
				str2 = strcat(str1, "\\\0");
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)str2);
				while (strlen(str2)) {
					str2 = strtoken(str2, word);
					SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)word);
				}
			}
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