#ifndef WINDISPLAY_H
#define WINDISPLAY_H

#include <QtGui/QMainWindow>
#include "ui_WinDisplay.h"
#include <qtimer.h>
#include "CellularAutomata_GPGPU.h"
#include "OuterTotalistic.h"

typedef int (__cdecl *MYPROC)(LPWSTR); 

class WinDisplay : public QMainWindow
{
	Q_OBJECT

	public:
		WinDisplay(QWidget *parent = 0, Qt::WFlags flags = 0);
		~WinDisplay();

	private:
		Ui::WinDisplayClass ui;
		QTimer *timer;
		CellularAutomata *CA;

	private slots:
		void tick();
};

#endif // WINDISPLAY_H
