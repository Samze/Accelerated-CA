#ifndef CACONTROLLER_H
#define CACONTROLLER_H

#include <QObject>
#include "ICAView.h"
#include <qtimer.h>
#include "CellularAutomata_GPGPU.h"
#include "OuterTotalistic.h"

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

public:
	void setView(ICAView *view);
	CellularAutomata *CA;


private: 
	CAController(); //private contructor, singleton pattern
	CAController(QObject *parent); 
	~CAController();
	void operator=(CAController const&); //do not implement
	Generations getCAClass();

private:
	ICAView *m_view;
	QTimer *timer;

	private slots:
		void start();
		void stop();
		void step(); //should link to tick
		void restart();
		void tick();

};

#endif // CACONTROLLER_H
