#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <qdialog.h>
#include "ui_NewCADialog.h"
#include <qdebug.h>

#include "CellularAutomata.h"
#include "util.h"
#include "CAController.h"

class PropertyDialog : public QDialog
{
	Q_OBJECT

public:
	PropertyDialog(QWidget* parent);
	PropertyDialog(QWidget* parent,Totalistic* ca);
	~PropertyDialog();


private:
	Ui::Dialog ui;
	int maxNeighbours;

	bool editMode; //We are editing! Change the rules, but not the lattice.
		
	CAController::Dimension dimension;
	QString gameType;
	int numStates;
	QList<int>* survNums;
	QList<int>* bornNums;
	int latticeSize;

	QString neighbourhoodType;
	void createDefaultSettings();
	void populateUISettings();

private slots:
	void on_okButton_clicked();
};

#endif // PROPERTYDIALOG_H
