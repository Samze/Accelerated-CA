#ifndef WINDISPLAY_H
#define WINDISPLAY_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include "ui_WinDisplay.h"
#include "ICAView.h"
#include <QMessageBox>
#include <QTextStream>
#include <qdebug.h>
#include <QStringList>


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
		void LoadFile(const QString &fileName);
		//CAController& m_controller; //this should probably be in abstract class

		QStringList fileContents;

	private slots:
		void on_btnLoadFile_clicked();

	signals:
		void setCAFromMCLFormat(QStringList&);

};

#endif // WINDISPLAY_H
