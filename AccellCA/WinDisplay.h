#ifndef WINDISPLAY_H
#define WINDISPLAY_H

#include <QtGui/QMainWindow>
#include "ui_WinDisplay.h"
#include "ICAView.h"


//typedef int (__cdecl *MYPROC)(LPWSTR); 

class WinDisplay : public QMainWindow, public ICAView
{
	Q_OBJECT
	Q_INTERFACES(ICAView)

	public:
		WinDisplay(QWidget *parent = 0, Qt::WFlags flags = 0);
		~WinDisplay();
		void setWindowTitle1(QString &title);
		void updateView(CellularAutomata *ca);
		void setController(CAController& controller);

	private:
		Ui::WinDisplayClass ui;
		//CAController& m_controller; //this should probably be in abstract class
};

#endif // WINDISPLAY_H
