#ifndef CELLULARAUTOMATAFACTORY_H
#define CELLULARAUTOMATAFACTORY_H

#include <QObject>
#include "AbstractCellularAutomata.h"
#include "gldrawer.h"
#include "ruleparser.h"

//TODO make this a singleton.
class CellularAutomataFactory : public QObject
{
	Q_OBJECT

public:
	CellularAutomataFactory(QObject *parent = 0);
	virtual ~CellularAutomataFactory(); //Virutal contstructor this abstract factory?

	virtual AbstractCellularAutomata* createRule(int size, int range) = 0;
	virtual GLDrawer* createDrawer() = 0;
	
	virtual RuleParser* createParser() = 0;
	//virtual RuleSaver* createSaver() = 0;

};

#endif // CELLULARAUTOMATAFACTORY_H
