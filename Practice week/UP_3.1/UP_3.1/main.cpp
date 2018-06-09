#include <windows.h>
#include "resource.h"
#include "Chocolate.h"
#include "Jelly.h"
#include "Muesli.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

const int _SIZE = 30;

using namespace std;

int read_file(vector<Sugar*>& arr, string filename);
INT_PTR CALLBACK DlgProc(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int userReply;
	//
	static vector<Sugar*> arr;
	int sz = read_file(arr, "source.txt");
	//ListBox
	static HWND hListBox;
	int sum = 0;

	switch (msg)
	{
	case WM_INITDIALOG:
		//Init ListBox
		hListBox = GetDlgItem(hWin, IDC_LIST1);
		return TRUE;
	case WM_CLOSE:
		userReply = MessageBox(hWin, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
		if (IDYES == userReply)
		{
			EndDialog(hWin, 0);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: 
			for_each(arr.begin(), arr.end(), [&sum](Sugar* sptr1) {sum += sptr1->getWeight(); });
			SetDlgItemInt(hWin, IDC_EDIT1, sum, true);
			return TRUE;
		case IDC_BUTTON1:
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			sort(arr.begin(), arr.end(), [](Sugar* sptr1,Sugar* sptr2) {
				return sptr1->getWeight() < sptr2->getWeight();
			});
			for (int i = 0; i<sz; i++)
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(arr[i]->toStr()).c_str());
			return TRUE;
		case IDC_BUTTON2:
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			for (int i = 0; i<sz; i++)
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(arr[i]->toStr()).c_str());
			return TRUE;
		case IDC_BUTTON3:
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			return TRUE;
		case IDC_BUTTON4:
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			sort(arr.begin(), arr.end(), [](Sugar* sptr1, Sugar* sptr2) {
				return sptr1->getSugar() < sptr2->getSugar();
			});
			for (int i = 0; i<sz; i++)
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(arr[i]->toStr()).c_str());
			return TRUE;
		case IDCANCEL:
			userReply = MessageBox(hWin, "Are you sure?", "Close", MB_YESNO | MB_ICONQUESTION);
			if (IDYES == userReply)
			{
				EndDialog(hWin, 0);
			}
			return TRUE;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	return 0;
}

int read_file(vector<Sugar*>& arr, string filename)
{
	//Start read
	ifstream ifile;
	ifile.open(filename);
	char buff;
	string buff1;
	arr.resize(_SIZE);
	int m = 0; //size

	while (!ifile.eof())
	{
		ifile >> buff;
		switch (buff)
		{
		case '1':
		{
			arr[m] = new Chocolate;
			getline(ifile, buff1);
			arr[m]->setStr(buff1);
			m++;
			break;
		}
		case '2':
		{
			arr[m] = new Jelly;
			getline(ifile, buff1);
			arr[m]->setStr(buff1);
			m++;
			break;
		}
		case '3':
		{
			arr[m] = new Muesli;
			getline(ifile, buff1);
			arr[m]->setStr(buff1);
			m++;
			break;
		}
		}

	}
	//End read
	ifile.close();
	arr.resize(m);
	return m;
}
