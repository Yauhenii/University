#include <windows.h>
#include "resource.h"
#include "KQueue.h"
#include <string>

using namespace std;

#define NMAXCOUNT 30

using namespace std;

INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Reply
	int userReply;
	//ListBox
	static HWND hListBox;
	//Queue
	static KQueue<string> queue;
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
		{
			char buff[NMAXCOUNT]="\0";
			GetDlgItemText(hWin, IDC_EDIT1, (LPTSTR)buff, NMAXCOUNT);
			queue.push(string(buff));
			//queue.emplace(buff);
			return TRUE;
		}
		case IDC_BUTTON2:
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			return TRUE;
		case IDC_BUTTON3:
			if (!queue.isEmpty())
				queue.pop();
			return TRUE;
		case IDC_BUTTON4:
			SetDlgItemInt(hWin, IDC_EDIT2, queue.getSize(), false);
			return TRUE;
		case IDC_BUTTON5:
			SetDlgItemInt(hWin, IDC_EDIT3, queue.getQuantity(), false);
			return TRUE;
		case IDC_BUTTON6:
		{
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			for (KQueue<string>::iterator it= queue.beginIt(); it != queue.endIt(); it++)
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(*it).c_str());
			return TRUE;
		}
		case IDC_BUTTON7:
			queue.clear();
			return TRUE;
		case IDC_BUTTON8:
			SetDlgItemText(hWin, IDC_EDIT4, (LPSTR)"");
			if (!queue.isEmpty())
			{
				SetDlgItemText(hWin, IDC_EDIT4, (LPSTR)queue.front().c_str());
			}
			return TRUE;
		case IDC_BUTTON9:
			SetDlgItemText(hWin, IDC_EDIT5, (LPSTR)"");
			if (!queue.isEmpty())
			{
				SetDlgItemText(hWin, IDC_EDIT5, (LPSTR)queue.back().c_str());
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