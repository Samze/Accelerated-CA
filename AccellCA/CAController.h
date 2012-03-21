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

class ICAView;

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
       IDLE,
	   ACTIVE,
	   STOPPED
    };
	
	enum Dimension {
		TWO,
		THREE
	};

public:
	void setView(ICAView *view);
	void setRule(AbstractCellularAutomata*);

	CellularAutomata *CA;

	void setDimension(Dimension);
	void setRandomLattice(int range);

	void createNewCA(const QString& type, int latticeSize, const QString& neighbourType, 
								int numStates, const QList<int>& survNum,  const QList<int>& bornNum);
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
	QTimer timer;

	void startSCIARA();

signals:
	void newDrawElement(GLDrawer*);
	void newCA(CellularAutomata*);

private slots:
	void start();
	void stop();
	void step(); //should link to tick
	void restart();
	void tick();
	void parseDefinition(QStringList& lines);
	void setRulesToCA();
};

#endif // CACONTROLLER_H
