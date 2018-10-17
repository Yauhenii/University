#pragma once
#include "Model.h"
#include <Windows.h>
#include "resource.h"

#define NMAXCOUNT 30

class View
{
private:
	HWND hWin;
	HWND hListBox;
public:
	//Constructors
	/*View(HWND& hW)
	{
	hWin = hW;
	}*/
	//Methods
	//hWin
	void setHWin(HWND& hW)
	{
		hWin = hW;
	}
	//ListBox
	void setListBox(int nIDDlgItem)
	{
		hListBox = GetDlgItem(hWin, nIDDlgItem);
	}
	void addStringListBox(LPARAM string)
	{
		SendMessage(hListBox, LB_ADDSTRING, 0, string);
	}
	void resetListBox()
	{
		SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
	}
	//DlgItem
	void setDlgItem(int nIDDlgItem, const char* string)
	{
		SetDlgItemText(hWin, nIDDlgItem, (LPSTR)string);
	}
	std::string getDlgText(int nIDDlgItem)
	{
		char buff[NMAXCOUNT] = "\0";
		GetDlgItemText(hWin, nIDDlgItem, (LPTSTR)buff, NMAXCOUNT);
		return std::string(buff);
	}
	void endView()
	{
		int userReply;
		userReply = MessageBox(hWin, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
		if (IDYES == userReply)
		{
			EndDialog(hWin, 0);
		}
	}
};