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
#include <QFileSystemModel>

#include "gl2ddrawer.h"
#include "gl3ddrawer.h"
#include "glinteroptestdrawer.h"

#include "cellularautomatafactory.h"
#include "factorymaker.h"

//typedef int (__cdecl *MYPROC)(LPWSTR); 

#define MAX_TIMER_INTERVAL 500

class WinDisplay : public QMainWindow, public ICAView
{
	Q_OBJECT
	Q_INTERFACES(ICAView)

	public:
		WinDisplay(QWidget *parent = 0, Qt::WFlags flags = 0);
		~WinDisplay();
		void updateView(CellularAutomata *ca);
		void setController(CAController& controller);

	private:
		Ui::WinDisplayClass ui;
		QFileSystemModel* dirModel;

		void LoadFile(const QString &fileName);

		QStringList fileContents;
		GLDrawer* drawer;

		bool newLatticeRequired;

	private slots:
		void newCALoaded(CellularAutomata *ca);
		
		void handleChangeState(CAController::State s);

		void on_sldSpeed_valueChanged(int);
		void on_treeView_clicked(QModelIndex index);
		void on_btnRandom_clicked();
		void on_actionLoad_triggered();
		void setDimension2D(bool);
		void setGLDrawer(GLDrawer* drawer);
	
		void createDefaultCA();

		void setupGUIElements(CellularAutomata *ca);

		void newLatticeSettingsChanged();
		void controlsChanged();

		void updateRuleComboBoxes(CellularAutomata *ca);

		void on_actionNew_triggered();

		void setupConnections(bool);


	signals:
		void setCAFromMCLFormat(QStringList&);
		void settingsChanged();

};

#endif // WINDISPLAY_H
