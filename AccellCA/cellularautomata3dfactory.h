#ifndef CELLULARAUTOMATA3DFACTORY_H
#define CELLULARAUTOMATA3DFACTORY_H


#include "cellularautomatafactory.h"
#include "gl3ddrawer.h"

#include "Generations3D.h"

#include "Abstract3DCA.h"


class CellularAutomata3DFactory : public CellularAutomataFactory
{
	Q_OBJECT

public:
	CellularAutomata3DFactory(QObject *parent = 0);
	~CellularAutomata3DFactory();
	
	virtual AbstractCellularAutomata* createRule(const QString& type);
	virtual AbstractCellularAutomata* createRule(int size, int range);
	
	virtual AbstractLattice* createLattice(int size, const QString& neighbourhoodType, int random = 0);


	virtual GLDrawer* createDrawer();

	virtual RuleParser* createParser() {
		return 0;
	}
	
};

#endif // CELLULARAUTOMATA3DFACTORY_H
