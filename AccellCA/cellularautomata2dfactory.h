#ifndef CELLULARAUTOMATA2DFACTORY_H
#define CELLULARAUTOMATA2DFACTORY_H

#include "cellularautomatafactory.h"
#include "gl2ddrawer.h"

#include "Generations.h"
#include "OuterTotalistic.h"

#include "Lattice2D.h"
#include "mcellparser.h"


class CellularAutomata2DFactory : public CellularAutomataFactory
{
	Q_OBJECT

public:
	CellularAutomata2DFactory(QObject *parent = 0);
	~CellularAutomata2DFactory();
	
	virtual AbstractCellularAutomata* createRule(const QString& type);
	virtual AbstractCellularAutomata* createRule(int size, int range);
	
	virtual AbstractLattice* createLattice(int size, const QString& neighbourhoodType, int random = 0);
	virtual AbstractLattice* createLattice(int size, const QString& neighbourhoodType, int* grid);

	virtual GLDrawer* createDrawer();
	virtual RuleParser* createParser();
	
};

#endif // CELLULARAUTOMATA2DFACTORY_H
