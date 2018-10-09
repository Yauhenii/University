#include <string>
//MVC
#include "Model.h"
#include "View.h"
#include "Controller.h"


using namespace std;

#define NMAXCOUNT 30
#define INT_CONST 0
#define STRING_CONST 1

using namespace std;


INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	return 0;
}
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static View view;
	view.setHWin(hWin);
	static Model model;
	static Controller controller(&model, &view);
	switch (msg)
	{
	case WM_INITDIALOG:
		return controller.actInit();
	case WM_CLOSE:
		return controller.actClose();
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			return controller.actClose();
		case IDCANCEL:
			return controller.actClose();
		case IDC_RADIO1:
			return controller.actRadio1();
		case IDC_RADIO2:
			return controller.actRadio2();
		case IDC_BUTTON1:
			return controller.actButton1();
		case IDC_BUTTON2:
			return controller.actButton2();
		case IDC_BUTTON3:
			return controller.actButton3();
		case IDC_BUTTON4:
			return controller.actButton4();
		case IDC_BUTTON5:
			return controller.actButton5();
		case IDC_BUTTON6:
			return controller.actButton6();
		case IDC_BUTTON7:
			return controller.actButton7();
		case IDC_BUTTON8:
			return controller.actButton8();
		case IDC_BUTTON9:
			return controller.actButton9();
		case IDC_BUTTON10:
			return controller.actButton10();
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

