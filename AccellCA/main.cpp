#include "WinDisplay.h"
#include <QtGui/QApplication>
#include "CAController.h"
#include "ICAView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	WinDisplay w;

	w.show();

	return a.exec();
}