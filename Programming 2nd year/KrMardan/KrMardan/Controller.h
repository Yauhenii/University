#include "View.h"

#pragma once
class Controller
{
private:
	Model * model;
	View* view;
public:
	//Constructor
	Controller(Model* m, View* v)
	{
		model = m;
		view = v;
	}
	//Methods
	bool actInit()
	{
		view->setListBox(IDC_LIST1);
		return TRUE;
	}
	bool actButton1()
	{
		std::string temp = view->getDlgText(IDC_EDIT3);
		model->push_back(temp);
		view->addStringListBox((LPARAM)temp.c_str());
		return TRUE;
	}
	bool actButton2()
	{
		if (!model->isEmpty())
		{
			std::string temp = model->pop_back();
			view->setDlgItem(IDC_EDIT4, temp.c_str());
			view->resetListBox();
			for (Model::iterator it = model->begin(); it != model->end(); it.next())
			{
				view->addStringListBox((LPARAM)(*it).c_str());
			}
		}
		else
		{
			view->setDlgItem(IDC_EDIT4, "Queue is empty");
		}
		return TRUE;
	}
	bool actButton3()
	{
		if (!model->isEmpty())
		{
			std::string temp = model->pop_front();
			view->setDlgItem(IDC_EDIT5, temp.c_str());
			view->resetListBox();
			for (Model::iterator it = model->begin(); it != model->end(); it.next())
			{
				view->addStringListBox((LPARAM)(*it).c_str());
			}
		}
		else
		{
			view->setDlgItem(IDC_EDIT5, "Queue is empty");
		}
		return TRUE;
	}
	bool actButton4()
	{
		if (!model->isEmpty())
		{
			Min min;
			model->accept(min);
			view->setDlgItem(IDC_EDIT1, min.getMin().c_str());
		}
		else
		{
			view->setDlgItem(IDC_EDIT1, "Queue is empty");
		}
		return TRUE;
	}
	bool actButton5()
	{
		if (!model->isEmpty())
		{
			Max max;
			model->accept(max);
			view->setDlgItem(IDC_EDIT2, max.getMax().c_str());
		}
		else
		{
			view->setDlgItem(IDC_EDIT2, "Queue is empty");
		}
		return TRUE;
	}
	bool actClose()
	{
		view->endView();
		return TRUE;
	}
};
