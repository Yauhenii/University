#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include "Model.h"

#define NMAXCOUNT 30
#define INT_CONST 0
#define STRING_CONST 1

//std::string intToString(int i);

class View
{
private: 
	HWND hWin;
	HWND hListBox;
public:
	//Constructors
	View()
	{
	}
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
	void resetListBox()
	{
		SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
	}
	void addStringListBox(LPARAM string)
	{
		SendMessage(hListBox, LB_ADDSTRING, 0, string);
	}
	void fillListBox(int type, Model& model)
	{
		if (type == STRING_CONST)
		{
			this->resetListBox();
			if (!model.isEmpty(STRING_CONST))
			{
				for (Model::iteratorString it = model.beginItString(); it != model.endItString(); it.next())
				{
					this->addStringListBox((LPARAM)(*it).c_str());
				}
			}
		}
		else
		{
			this->resetListBox();
			if (!model.isEmpty(INT_CONST))
			{
				for (Model::iteratorInt it = model.beginItInt(); it != model.endItInt(); it.next())
				{
					std::stringstream s;
					s << *it;
					return;
					this->addStringListBox((LPARAM)s.str().c_str());
				}
			}
		}
	}
	//RaidioButton
	void setRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton)
	{
		CheckRadioButton(hWin, nIDFirstButton, nIDLastButton, nIDCheckButton);
	}
	//DlgItem
	void setDlgItem(int nIDDlgItem, UINT uValue, bool bSigned)
	{
		SetDlgItemInt(hWin, nIDDlgItem, uValue, bSigned);
	}
	void setDlgItem(int nIDDlgItem, char* string)
	{
		SetDlgItemText(hWin, nIDDlgItem, (LPSTR)string);
	}
	void setDlgItemFront(int type, Model& model, int nIDDlgItem)
	{
		if (type == STRING_CONST)
		{
			SetDlgItemText(hWin, nIDDlgItem, (LPSTR)"");
			if (!model.isEmpty(type))
			{
				SetDlgItemText(hWin, nIDDlgItem, model.frontString().c_str());
			}
		}
		else
		{
			SetDlgItemText(hWin, nIDDlgItem, (LPSTR)"");
			if (!model.isEmpty(type))
			{
				SetDlgItemInt(hWin, nIDDlgItem,model.frontInt(), TRUE);
			}
		}
	}
	void setDlgItemBack(int type, Model& model, int nIDDlgItem)
	{
		if (type == STRING_CONST)
		{
			SetDlgItemText(hWin, nIDDlgItem, (LPSTR)"");
			if (!model.isEmpty(type))
			{
				SetDlgItemText(hWin, nIDDlgItem, model.backString().c_str());
			}
		}
		else
		{
			SetDlgItemText(hWin, nIDDlgItem, (LPSTR)"");
			if (!model.isEmpty(type))
			{
				SetDlgItemInt(hWin, nIDDlgItem, model.backInt(), TRUE);
			}
		}
	}
	int getDlgInt(int nIDDlgItem)
	{
		char buff[NMAXCOUNT] = "\0";
		GetDlgItemText(hWin, nIDDlgItem, (LPTSTR)buff, NMAXCOUNT);
		int i = atoi(buff);
		return i;
	}
	std::string getDlgText(int nIDDlgItem)
	{
		char buff[NMAXCOUNT] = "\0";
		GetDlgItemText(hWin, nIDDlgItem, (LPTSTR)buff, NMAXCOUNT);
		return std::string(buff);
	}
	//Operators

	//Destructor
	//~View();
};

//std::string intToString(int i)
//{
//	std::stringstream s;
//	s << i;
//	return s.str();
//}