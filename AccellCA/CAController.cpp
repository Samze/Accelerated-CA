#include "CAController.h"

CAController::CAController() { }

CAController::CAController(QObject *parent)
	: QObject(parent) {}

CAController::~CAController()
{
	//do not delete m_view
}


void CAController::setView(ICAView *view) {
	m_view = view;
}

void CAController::start() {
	int timerTick = 1; //in Milliseconds
	int CellularDim = 100; //Specifies the width/height for our CA
	int initSeed = 5; //Random "0 to initseed" chance of each cell being live to start with, lower is a higher chance.
	
	//Create random CA, lower range is more concentrated
	CA = new CellularAutomata_GPGPU(CellularDim,initSeed);
	
	//setup timer
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this,SLOT(tick()));
	timer->start(timerTick);

}

void CAController::stop() {
	timer->stop();
}

void CAController::step() {
	tick();
}

void CAController::restart() {

}


//This is called every x-time based on timer settings
void CAController::tick(){
	
	
		int maxBits = 1;

		for (int i = 1; i < 4; i++) {
			maxBits = (maxBits << 1) + 1;
		}


	Abstract2DCA *gen = getCAClass();
	
	//TODO this should be drawing as the graphics card
	//is calculating new values...This would require doing an expensive
	//memory copy though...probably worth it.
	m_view->updateView(CA);

	Generations* v = dynamic_cast<Generations*>(gen);
	OuterTotalistic* v2 = dynamic_cast<OuterTotalistic*>(gen);
	if(v != 0) {
	// old was safely casted to NewType
		//qDebug("%3.3f",CA->nextTimeStep(*v));
		CA->nextTimeStep(*v);
	}
	else {
		CA->nextTimeStep(*v2);
	}
	
	//float timeTaken = 
	//qDebug("Time taken:%3.1f ms\n",timeTaken);

}


Abstract2DCA* CAController::getCAClass() {
	Abstract2DCA *outer = new OuterTotalistic();
	Abstract2DCA *gen = new Generations();
		//OuterTotalistic outer;
	int* surviveNo = new int[1];
	int* bornNo = new int[1];

	surviveNo[0] = 2;
	surviveNo[1] = 3;
	surviveNo[2] = 5;
	surviveNo[3] = 6;
	surviveNo[4] = 7;
	surviveNo[5] = 8;

	bornNo[0] = 3;
	bornNo[1] = 4;
	bornNo[2] = 6;
	bornNo[3] = 8;

	outer->setSurviveNo(surviveNo,0);
	outer->setBornNo(bornNo,1);
	outer->setStates(2);
	outer->neighbourhoodType = outer->MOORE;


	gen->setSurviveNo(surviveNo,6);
	gen->setBornNo(bornNo,4);
	gen->setStates(9);
	gen->neighbourhoodType = gen->MOORE;

	int x = gen->noBits;

	int one = 1;
	int fif = 15;

	int y = one & fif;

	return gen;
}