#include <windows.h>
#include "resource.h"
#include "KQueue.h"
#include <string>
#include <sstream>
#include "DoubleValue.h"
#include <tuple>

using namespace std;

#define NMAXCOUNT 30
#define INT 0
#define STRING 1

int TYPE = STRING;

using namespace std;

string intToString(int i);
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Reply
	int userReply;
	//ListBox
	static HWND hListBox;
	//Queue
	static tuple<KQueue<int>, KQueue<string>> queue;
	switch (msg)
	{
	case WM_INITDIALOG:
		//Init ListBox
		hListBox = GetDlgItem(hWin, IDC_LIST1);
		//Init RadioButton
		CheckRadioButton(hWin, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
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
		case IDC_RADIO1:
		{
			TYPE = 0;
			return TRUE;
		}
		case IDC_RADIO2:
		{
			TYPE = 1;
			return TRUE;
		}
		case IDC_BUTTON1:
		{
			if (TYPE == STRING)
			{
				char buff[NMAXCOUNT] = "\0";
				GetDlgItemText(hWin, IDC_EDIT1, (LPTSTR)buff, NMAXCOUNT);
				get<1>(queue).push(string(buff));
			}
			else
			{
				char buff[NMAXCOUNT] = "\0";
				GetDlgItemText(hWin, IDC_EDIT1, (LPTSTR)buff, NMAXCOUNT);
				int i = atoi(buff);
				get<0>(queue).push(i);
			}
			return TRUE;
		}
		case IDC_BUTTON2:
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			return TRUE;
		case IDC_BUTTON3:
			if (TYPE == STRING)
			{
				if (!get<1>(queue).isEmpty())
					get<1>(queue).pop();
			}
			else
			{
				if (!get<0>(queue).isEmpty())
					get<0>(queue).pop();
			}
			return TRUE;
		case IDC_BUTTON4:
			if (TYPE == STRING)
			{
				SetDlgItemInt(hWin, IDC_EDIT2, get<1>(queue).getSize(), false);
			}
			else
			{
				SetDlgItemInt(hWin, IDC_EDIT2, get<0>(queue).getSize(), false);
			}
			return TRUE;
		case IDC_BUTTON5:
			if (TYPE == STRING)
			{
				SetDlgItemInt(hWin, IDC_EDIT3, get<1>(queue).getQuantity(), false);
			}
			else
			{
				SetDlgItemInt(hWin, IDC_EDIT3, get<0>(queue).getQuantity(), false);
			}
			return TRUE;
		case IDC_BUTTON6:
		{
			if (TYPE == STRING)
			{
				SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
				if (!get<1>(queue).isEmpty())
				{
					for (KQueue<string>::iterator it = get<1>(queue).beginIt(); it != get<1>(queue).endIt(); it.next())
					{
						SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(*it).c_str());
					}
				}
			}
			else
			{
				SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
				if (!get<0>(queue).isEmpty())
				{
					for (KQueue<int>::iterator it = get<0>(queue).beginIt(); it != get<0>(queue).endIt(); it.next())
					{
						SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)intToString(*it).c_str());
					}
				}
			}
			return TRUE;
		}
		case IDC_BUTTON7:
			if (TYPE == STRING)
			{
				get<1>(queue).clear();
			}
			else
			{
				get<0>(queue).clear();
			}
			return TRUE;
		case IDC_BUTTON8:
			if (TYPE == STRING)
			{
				SetDlgItemText(hWin, IDC_EDIT4, (LPSTR)"");
				if (!get<1>(queue).isEmpty())
				{
					SetDlgItemText(hWin, IDC_EDIT4, (LPSTR)get<1>(queue).front().c_str());
				}
			}
			else
			{
				SetDlgItemText(hWin, IDC_EDIT4, (LPSTR)"");
				if (!get<0>(queue).isEmpty())
				{
					SetDlgItemInt(hWin, IDC_EDIT4, get<0>(queue).front(), TRUE);
				}
			}
			return TRUE;
		case IDC_BUTTON9:
			if (TYPE == STRING)
			{
				SetDlgItemText(hWin, IDC_EDIT5, (LPSTR)"");
				if (!get<1>(queue).isEmpty())
				{
					SetDlgItemText(hWin, IDC_EDIT5, (LPSTR)get<1>(queue).back().c_str());
				}
			}
			else
			{
				SetDlgItemText(hWin, IDC_EDIT5, (LPSTR)"");
				if (!get<0>(queue).isEmpty())
				{
					SetDlgItemInt(hWin, IDC_EDIT5, get<0>(queue).back(), TRUE);
				}
			}
			return TRUE;
		case IDC_BUTTON10:
		{
			DoubleValue visitor;
			if (TYPE == STRING)
			{
				get<1>(queue).accept(visitor);
			}
			else
			{
				get<0>(queue).accept(visitor);
			}	
			return TRUE;
		}
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

string intToString(int i)
{
	stringstream s;
	s << i;
	return s.str();
}