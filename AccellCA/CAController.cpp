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
	int CellularDim = 300; //Specifies the width/height for our CA
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
	
	Generations gen = getCAClass();

	float timeTaken = CA->nextTimeStep(gen);
	qDebug("Time taken:%3.1f ms\n",timeTaken);

	m_view->updateView(CA);
}


Generations CAController::getCAClass() {
	OuterTotalistic outer;
	int* surviveNo = new int[1];
	int* bornNo = new int[1];
	surviveNo[0] = 0;
	//surviveNo[1] = 3;
	bornNo[0] = 2;
	outer.setSurviveNo(surviveNo,2);
	outer.setBornNo(bornNo,1);
	outer.setStates(2);
	outer.neighbourhoodType = outer.MOORE;


	Generations gen;
	gen.setSurviveNo(surviveNo,0);
	gen.setBornNo(bornNo,1);
	gen.setStates(3);

	int x = gen.noBits;

	gen.neighbourhoodType = outer.MOORE;

	return gen;
}