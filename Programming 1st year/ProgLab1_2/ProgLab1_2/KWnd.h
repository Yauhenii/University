#pragma once
#include <Windows.h>
#include <tchar.h>

class KWnd
{
public:
	KWnd(LPCWSTR windowName,
		HINSTANCE hInst,
		int cmdShow,
		LRESULT(WINAPI *pWndProc)(HWND, UINT, WPARAM, LPARAM),
		LPCTSTR menuName = NULL,
		int x = CW_USEDEFAULT,
		int y = 0,
		int width = CW_USEDEFAULT,
		int height = 0,
		UINT classStyle = CS_HREDRAW | CS_VREDRAW,
		DWORD windowStyle = WS_OVERLAPPEDWINDOW,
		HWND hParent = NULL);
	HWND getHWnd()
	{
		return hWnd;
	}
protected:
	HWND hWnd;
	WNDCLASSEX wc;
};

