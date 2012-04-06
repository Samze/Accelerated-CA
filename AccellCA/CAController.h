#ifndef CACONTROLLER_H
#define CACONTROLLER_H

#include <QObject>
#include "ICAView.h"
#include <qtimer.h>
#include "CellularAutomata_GPGPU.h"
#include "OuterTotalistic.h"
#include <qregexp.h>
#include <QStringList>
#include "ruleparser.h"
#include "util.h"

#include "cellularautomatafactory.h"
#include "factorymaker.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

class ICAView;

class Util;

class CAController : public QObject
{
	Q_OBJECT

public: //singleton pattern
	static CAController& getInstance()
	{
		static CAController instance; //Allocated on stack, will be destroyed
		return instance;
	}

	enum State {
	   EMPTY,	
       LOADED,
	   ACTIVE,
	   STOPPED
    };
	
	enum Dimension {
		TWO = 2,
		THREE = 3
	};

public:
	void setView(ICAView *view);
	void setRule(AbstractCellularAutomata*);

	CellularAutomata *CA;

	void setDimension(Dimension);
	void setRandomLattice(int range);

	void createNewCA(const QString& type, int latticeSize, const QString& neighbourType, 
								int numStates, const QList<int>& survNum,  const QList<int>& bornNum, AbstractLattice* lattice = 0);

	void setTimerTick(int timerTick);

private: 
	CAController(); //private contructor, singleton pattern
	CAController(QObject *parent); 
	~CAController();
	void operator=(CAController const&); //do not implement
	

	CellularAutomataFactory* factory;

	void resetForNewLoad();

	State state;
	int timerTick; //in Milliseconds
	int initSeed; //Random "0 to initseed" chance of each cell being live to start with, lower is a higher chance.
	
private:
	ICAView *m_view; //Weak assosiation, do not release.

	QTimer viewTimer;
	QTimer caTimer;

	
	int* initialLattice;
	int* previousLattice;

	void startSCIARA();

signals:
	void newDrawElement(GLDrawer*);
	void newCA(CellularAutomata*);
	void newCAState(CAController::State);

private slots:

	void restart();
	void back();
	void start();
	void stop();
	void forward();
	void forwardN();

	void caTick();
	void viewTick();
	void parseDefinition(QStringList& lines);
	void setRulesToCA();

	void newCALoaded(CellularAutomata*);

	void changeState(State);
};

#endif // CACONTROLLER_H
