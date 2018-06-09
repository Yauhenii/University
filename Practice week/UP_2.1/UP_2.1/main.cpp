#include <windows.h>
#include "resource.h"
#include <string>
#include <sstream>

const float d = 0.01;

using namespace std;

string fltostr(float f);
extern "C" float __cdecl findsol(float a, float b, float c, float* root, float left, float right);
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int userReply;
	static char* temp = new char[MAX_PATH];
	const int edit_am = 5;
	static float farrC[edit_am];
	const int iarrC[edit_am] = { IDC_EDIT1 ,IDC_EDIT2,IDC_EDIT3,IDC_EDIT4,IDC_EDIT5 };
	//Solutions
	static float root;

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
			for (int i = 0; i < edit_am; i++)
			{
				GetDlgItemText(hWin, iarrC[i], (LPSTR)temp, 10);
				if (temp == "")
					farrC[i] = 0.0;
				else
					farrC[i]= atof(temp);
			}
			if (findsol(farrC[2], farrC[3], farrC[4], &root, farrC[0], farrC[1]))
			{
				SetDlgItemText(hWin, IDC_EDIT6, fltostr(root).c_str());
			}
			else
				SetDlgItemText(hWin, IDC_EDIT6, "NO");
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

string fltostr(float f) {
	stringstream stream;
	stream << f;
	return stream.str();
}