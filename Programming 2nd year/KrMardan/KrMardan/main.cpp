//MVC
#include "Controller.h"

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
		case IDCANCEL:
			return controller.actClose();
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
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
