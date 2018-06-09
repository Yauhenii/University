#include <windows.h>
#include "resource.h"

extern "C" char* __fastcall atohex(int a);
extern "C" void __cdecl MemoryToHex(void* addr, int size, char* buffer);
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Reply
	int userReply;
	//EditBox
	static int num;
	char* buff = new char[MAX_PATH];
	static float fl = 62.487;

	switch (msg)
	{
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
			num = GetDlgItemInt(hWin, IDC_EDIT1, NULL, TRUE);
			SetDlgItemText(hWin, IDC_EDIT2, atohex(num));
			MemoryToHex(&fl, sizeof(fl), buff);
			SetDlgItemText(hWin, IDC_EDIT3, buff);
			return TRUE;
		case IDCANCEL:
			userReply = MessageBox(hWin, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
			if (IDYES == userReply)
			{
				EndDialog(hWin, 0);
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
