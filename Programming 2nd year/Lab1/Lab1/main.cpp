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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	View view;
	Model model;
	Controller controller(model,view);
	controller.act(hInstance, model, view);
	return 0;
}


