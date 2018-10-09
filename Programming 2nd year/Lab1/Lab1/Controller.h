#pragma once
#include "View.h"
#include "resource.h"
//Visitor
#include "DoubleValue.h"

class Controller
{
private:
	Model* model;
	View* view;
public:
	Controller(Model* m, View* v)
	{
		model = m;
		view = v;
	}
	//Methods
	bool start(Model& model, View& view, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		
	}
	bool actInit()
	{
		view->setListBox(IDC_LIST1);
		view->setRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		model->setType(STRING_CONST);
		return TRUE;
	}
	bool actRadio1()
	{
		model->setType(INT_CONST);
		return TRUE;
	}
	bool actRadio2()
	{
		model->setType(STRING_CONST);
		return TRUE;
	}
	bool actClose()
	{
		view->endView();
		return TRUE;
	}
	bool actButton1()
	{
		if (model->isStringType())
			model->push(view->getDlgText(IDC_EDIT1));
		else
			model->push(view->getDlgInt(IDC_EDIT1));
		view->fillListBox(model->getType(), *model);
		return TRUE;
	}
	bool actButton2()
	{
		view->resetListBox();
		return TRUE;
	}
	bool actButton3()
	{
		model->pop(model->getType());
		return TRUE;
	}
	bool actButton4()
	{
		view->setDlgItem(IDC_EDIT2, model->getSize(model->getType()), false);
		return TRUE;
	}
	bool actButton5()
	{
		view->setDlgItem(IDC_EDIT3, model->getQuantity(model->getType()), false);
		return TRUE;
	}
	bool actButton6()
	{
		view->fillListBox(model->getType(), *model);
		return TRUE;
	}
	bool actButton7()
	{
		model->clear(model->getType());
		return TRUE;
	}
	bool actButton8()
	{
		view->setDlgItemFront(model->getType(), *model, IDC_EDIT4);
		return TRUE;
	}
	bool actButton9()
	{
		view->setDlgItemBack(model->getType(), *model, IDC_EDIT5);
		return TRUE;
	}
	bool actButton10()
	{
		DoubleValue visitor;
		model->accept(visitor);
		return TRUE;
	}
	//Methods
	//Destructor
	//~Controller();
};

