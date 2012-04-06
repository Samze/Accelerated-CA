#include "WinDisplay.h"
#include <QtGui/QApplication>
#include "CAController.h"
#include "ICAView.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int* meep = new int[500];
	QApplication a(argc, argv);
	
	WinDisplay w;

	w.show();

	delete[] meep;
	return a.exec();
}