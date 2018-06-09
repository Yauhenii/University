#include "KWnd.h"


KWnd::KWnd(LPCWSTR windowName,
	HINSTANCE hInst,
	int cmdShow,
	LRESULT(WINAPI *pWndProc)(HWND, UINT, WPARAM, LPARAM),
	LPCTSTR menuName,
	int x,
	int y,
	int width,
	int height,
	UINT classStyle,
	DWORD windowStyle,
	HWND hParent)
{
	char szClassName[] = "KWndClass";

	wc.cbSize = sizeof(wc);
	wc.style = classStyle;
	wc.lpfnWndProc = pWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = menuName;
	wc.lpszClassName = (LPCWSTR)szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		char msg[100] = "Cannot register class: ";
		strcat(msg, szClassName);
		MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR) "Error", MB_OK);
		return;
	}

	hWnd = CreateWindow((LPCWSTR)szClassName,
		(LPCWSTR)windowName,
		windowStyle,
		x, y,
		width,
		height,
		hParent,
		(HMENU)NULL,
		hInst,
		NULL);

	if (!hWnd) {
		/*char msg[100] = "Cannot create window: ";
		strcat(msg, windowName);
		MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)"Error", MB_OK);
		return;*/
	}

	ShowWindow(hWnd, cmdShow);
}
