#ifndef CACONTROLLER_H
#define CACONTROLLER_H

#include <QObject>
#include "ICAView.h"
#include <qtimer.h>
#include "CellularAutomata_GPGPU.h"
#include "OuterTotalistic.h"
#include <qregexp.h>
#include <QStringList>
#include <lexiconparser.h>

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

public:
	void setView(ICAView *view);
	CellularAutomata *CA;


private: 
	CAController(); //private contructor, singleton pattern
	CAController(QObject *parent); 
	~CAController();
	void operator=(CAController const&); //do not implement
	Abstract2DCA* getCAClass();
	LexiconParser parser;

	State state;
		int timerTick; //in Milliseconds
		int cellularDim; //Specifies the width/height for our CA
		int initSeed; //Random "0 to initseed" chance of each cell being live to start with, lower is a higher chance.
	
private:
	ICAView *m_view;
	QTimer *timer;

private slots:
	void start();
	void stop();
	void step(); //should link to tick
	void restart();
	void tick();
	void createCAFromMCLFormat(QStringList& lines);
};

#endif // CACONTROLLER_H
