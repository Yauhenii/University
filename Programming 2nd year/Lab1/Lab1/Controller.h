#pragma once
#include "View.h"
#include "resource.h"
//Visitor
#include "DoubleValue.h"

class Controller
{
private:
	Model model;
	View view;
public:
	//Constructors
	Controller(Model& m, View& v)
	{
		model = m;
		view = v;
	}
	void act(HINSTANCE hInstance,Model&model, View& view)
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, &DlgProc);
	}
	INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		view.setHWin(hWin);
		return start(model, view, msg, wParam, lParam);
	}
	bool start(Model& model, View& view, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_INITDIALOG:
			return actInit(model, view);
		case WM_CLOSE:
			return actClose(view);
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDOK:
				return actClose(view);
			case IDCANCEL:
				return actClose(view);
			case IDC_RADIO1:
				return actRadio1(model);
			case IDC_RADIO2:
				return actRadio2(model);
			case IDC_BUTTON1:
				return actButton1(model, view);
			case IDC_BUTTON2:
				return actButton2(view);
			case IDC_BUTTON3:
				return actButton3(model, view);
			case IDC_BUTTON4:
				return actButton4(model, view);
			case IDC_BUTTON5:
				return actButton5(model, view);
			case IDC_BUTTON6:
				return actButton6(model, view);
			case IDC_BUTTON7:
				return actButton7(model);
			case IDC_BUTTON8:
				return actButton8(model,view);
			case IDC_BUTTON9:
				return actButton9(model,view);
			case IDC_BUTTON10:
				return actButton10(model);
			default:
				return FALSE;
			}
			break;
		default:
			return FALSE;
		}
	}
private:
	bool actInit(Model& model, View& view)
	{
		view.setListBox(IDC_LIST1);
		view.setRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		model.setType(STRING_CONST);
		return TRUE;
	}
	bool actRadio1(Model& model)
	{
		model.setType(INT_CONST);
		return TRUE;
	}
	bool actRadio2(Model& model)
	{
		model.setType(STRING_CONST);
		return TRUE;
	}
	bool actClose(View& view)
	{
		view.endView();
		return TRUE;
	}
	bool actButton1(Model& model, View&view)
	{
		if (model.isStringType())
			model.push(view.getDlgText(IDC_EDIT1));
		else
			model.push(view.getDlgInt(IDC_EDIT1));
		return TRUE;
	}
	bool actButton2(View& view)
	{
		view.resetListBox();
		return TRUE;
	}
	bool actButton3(Model& model, View&view)
	{
		model.pop(model.getType());
		return TRUE;
	}
	bool actButton4(Model& model, View&view)
	{
		view.setDlgItem(IDC_EDIT2, model.getSize(model.getType()), false);
		return TRUE;
	}
	bool actButton5(Model& model, View&view)
	{
		view.setDlgItem(IDC_EDIT3, model.getQuantity(model.getType()), false);
		return TRUE;
	}
	bool actButton6(Model& model, View&view)
	{
		view.fillListBox(model.getType(), model);
		return TRUE;
	}
	bool actButton7(Model& model)
	{
		model.clear(model.getType());
		return TRUE;
	}
	bool actButton8(Model& model, View& view)
	{
		view.setDlgItemFront(model.getType(), model, IDC_EDIT4);
		return TRUE;
	}
	bool actButton9(Model& model, View& view)
	{
		view.setDlgItemBack(model.getType(), model, IDC_EDIT5);
		return TRUE;
	}
	bool actButton10(Model& model)
	{
		DoubleValue visitor;
		model.accept(model.getType(), visitor);
		return TRUE;
	}
	//Methods
	//Destructor
	//~Controller();
};

