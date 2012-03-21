#ifndef WINDISPLAY_H
#define WINDISPLAY_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include "ui_WinDisplay.h"
#include "ICAView.h"
#include <QMessageBox>
#include <QTextStream>
#include <QTime>
#include <qdebug.h>
#include <QStringList>
#include <QStringBuilder>

#include "gl2ddrawer.h"
#include "gl3ddrawer.h"
#include "glinteroptestdrawer.h"

#include "cellularautomatafactory.h"
#include "factorymaker.h"
#include "propertydialog.h"

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

		QStringList fileContents;
		GLDrawer* drawer;

	private slots:
		void newCALoaded(CellularAutomata *ca);
		
		void on_btnRandom_clicked();
		void on_btnEditRule_clicked();
		void on_actionLoad_triggered();
		void on_btnRestart_clicked();
		void setDimension2D(bool);
		void setGLDrawer(GLDrawer* drawer);
	
		void setupGUIElements();
		void setupGUIElements(CellularAutomata *ca);

		void on_btnCreateRule_clicked();

		void on_actionNew_triggered();

	signals:
		void setCAFromMCLFormat(QStringList&);
		void settingsChanged();

	private:
		int maxNeighbours; //Used to populate the Surv/Born state combo

};

#endif // WINDISPLAY_H
