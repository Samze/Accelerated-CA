#ifndef CELLULARAUTOMATAFACTORY_H
#define CELLULARAUTOMATAFACTORY_H

#include <QObject>
#include "AbstractCellularAutomata.h"
#include "gldrawer.h"
#include "ruleparser.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

//TODO make this a singleton.
class CellularAutomataFactory : public QObject
{
	Q_OBJECT

public:
	CellularAutomataFactory(QObject *parent = 0);
	virtual ~CellularAutomataFactory();
	
	virtual AbstractCellularAutomata* createRule(const QString& type) = 0;
	
	virtual AbstractLattice* createLattice(int size, const QString& neighbourhoodType, int* grid) = 0;
	virtual AbstractLattice* createLattice(int size, const QString& neighbourhoodType, int random = 0) = 0;

	virtual AbstractCellularAutomata* createRule(int size, int range) = 0;
	virtual GLDrawer* createDrawer() = 0;
	
	virtual RuleParser* createParser() = 0;
	//virtual RuleSaver* createSaver() = 0;

};

#endif // CELLULARAUTOMATAFACTORY_H
