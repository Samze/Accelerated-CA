#include "WinDisplay.h"
#include <QtGui/QApplication>
#include "CAController.h"
#include "ICAView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	WinDisplay w;
	CAController& controller = CAController::getInstance();
	
	controller.setView(&w);

	w.show();

	return a.exec();
}