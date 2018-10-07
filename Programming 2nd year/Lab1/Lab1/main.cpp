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
	static Controller controller;
	return controller.start(model, view, msg, wParam, lParam);
}

