#include "CAController.h"

CAController::CAController() {
	state = IDLE;
	
	//setup timer
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this,SLOT(tick()));

	timerTick = 1; //in Milliseconds
	cellularDim = 200; //Specifies the width/height for our CA
	initSeed = 5; //Random "0 to initseed" chance of each cell being live to start with, lower is a higher chance.
	
}

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

	if (state == IDLE) {
		//Create random CA, lower range is more concentrated
		//CA = new CellularAutomata_GPGPU(cellularDim,initSeed);
		m_view->updateView(CA);
	}
	
	state = ACTIVE;
	timer->start(timerTick);

}

void CAController::stop() {
	if (state == IDLE) return;

	if (state == ACTIVE){
		timer->stop();
		state = STOPPED;
	}
}

void CAController::step() {
	if (state == IDLE) return;

	if(state == ACTIVE) {
		stop();
	}
	tick();
}

void CAController::restart() {
	if(state == IDLE) return;

	if (state == ACTIVE) {
		stop();	
	}
	
	CA = new CellularAutomata_GPGPU(cellularDim,initSeed);
	m_view->updateView(CA);
	state = IDLE;
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

	
	float timeTaken;

	if(v != 0) {
	// old was safely casted to NewType
		//qDebug("%3.3f",CA->nextTimeStep(*v));
		timeTaken = CA->nextTimeStep(*v);
	}
	else {
		timeTaken = CA->nextTimeStep(*v2);
	}

	qDebug("Time taken:%3.1f ms\n",timeTaken);

}


Abstract2DCA* CAController::getCAClass() {
	Abstract2DCA *outer = new OuterTotalistic();
	Abstract2DCA *gen = new Generations();
		//OuterTotalistic outer;
	//int* bornNo = new int[1];

	//surviveNo[0] = 2;
	//surviveNo[1] = 3;
	//surviveNo[2] = 5;
	//surviveNo[3] = 6;
	//surviveNo[4] = 7;
	//surviveNo[5] = 8;

	//bornNo[0] = 3;
	//bornNo[1] = 4;
	//bornNo[2] = 6;
	//bornNo[3] = 8;

	QList<int>* survNums = parser.ruleData.at(0);
	QList<int>* bornNums = parser.ruleData.at(1);
	QList<int>* states = parser.ruleData.at(2);

	int survSize = (*survNums).size();
	int* surviveNo = new int[survSize];
	int count = 0;
	//Pull this out into a seperate method
	foreach(int i, *survNums) {
		surviveNo[count] = i;
		++count;
	}
	
	int bornSize = (*bornNums).size();
	int* bornNo = new int[bornSize];
	count = 0;
	//Pull this out into a seperate method
	foreach(int i, *bornNums) {
		bornNo[count] = i;
		++count;
	}

	int state = (*states).at(0);
	//surviveNo[0] = 2;
	//surviveNo[1] = 3;
	//bornNo[0] = 3;

	gen->setSurviveNo(surviveNo,survSize);
	gen->setBornNo(bornNo,bornSize);
	gen->setStates(25);
	gen->neighbourhoodType = outer->MOORE;


	//gen->setSurviveNo(surviveNo,6);
	//gen->setBornNo(bornNo,4);
	
	
	int x = gen->noBits;

	int one = 1;
	int fif = 15;

	int y = one & fif;

	return gen;
}

void CAController::createCAFromMCLFormat(QStringList& lines) {
	CA = parser.parseContent(lines);
	m_view->updateView(CA);
}