#ifndef CELLULARAUTOMATA2DFACTORY_H
#define CELLULARAUTOMATA2DFACTORY_H

#include "cellularautomatafactory.h"
#include "gl2ddrawer.h"

#include "Generations.h"
#include "OuterTotalistic.h"

#include "Abstract2DCA.h"
#include "mcellparser.h"


class CellularAutomata2DFactory : public CellularAutomataFactory
{
	Q_OBJECT

public:
	CellularAutomata2DFactory(QObject *parent = 0);
	~CellularAutomata2DFactory();

	virtual AbstractCellularAutomata* createRule(int size, int range);
	virtual GLDrawer* createDrawer();
	virtual RuleParser* createParser();
	
};

#endif // CELLULARAUTOMATA2DFACTORY_H
