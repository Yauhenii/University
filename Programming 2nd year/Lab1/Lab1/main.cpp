#include "resource.h"
#include "KQueue.h"
#include <string>
#include "DoubleValue.h"
#include <tuple>
//MVC
#include "Model.h"
#include "View.h"


using namespace std;

#define NMAXCOUNT 30
#define INT_CONST 0
#define STRING_CONST 1

int TYPE = STRING_CONST;

using namespace std;

INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Reply
	int userReply;
	static View view;
	//Queue
	//static tuple<KQueue<int>, KQueue<string>> queue;
	static Model model;
	switch (msg)
	{
	case WM_INITDIALOG:
		view.setHWin(hWin);
		view.setListBox(IDC_LIST1); //Init ListBox
		view.setRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2); //Init RadioButton
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
			if (TYPE == STRING_CONST)
				model.push(view.getDlgText(IDC_EDIT1));
			else
				model.push(view.getDlgInt(IDC_EDIT1));
			return TRUE;
		}
		case IDC_BUTTON2:
			view.resetListBox();
			return TRUE;
		case IDC_BUTTON3:
			model.pop(TYPE);
			return TRUE;
		case IDC_BUTTON4:
			view.setDlgItem(IDC_EDIT2, model.getSize(TYPE), false);
			return TRUE;
		case IDC_BUTTON5:
			view.setDlgItem(IDC_EDIT3, model.getQuantity(TYPE), false);
			return TRUE;
		case IDC_BUTTON6:
		{
			view.fillListBox(TYPE, model);
			return TRUE;
		}
		case IDC_BUTTON7:
			model.clear(TYPE);
			return TRUE;
		case IDC_BUTTON8:
			view.setDlgItemFront(TYPE, model, IDC_EDIT4);
			return TRUE;
		case IDC_BUTTON9:
			view.setDlgItemBack(TYPE, model, IDC_EDIT5);
			return TRUE;
		case IDC_BUTTON10:
		{
			DoubleValue visitor;
			model.accept(TYPE, visitor);
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

