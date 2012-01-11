#include "CAController.h"

CAController::CAController() {
	state = IDLE;
	
	connect(&timer, SIGNAL(timeout()),this,SLOT(tick()));

	timerTick = 1; //in Milliseconds
	cellularDim = 200; //Specifies the width/height for our CA
	initSeed = 5; //Random "0 to initseed" chance of each cell being live to start with, lower is a higher chance.
	
}

CAController::CAController(QObject *parent)
	: QObject(parent) {}

CAController::~CAController()
{
	//Do not delete m_view
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
	timer.start(timerTick);

}

void CAController::stop() {
	if (state == IDLE) return;

	if (state == ACTIVE){
		timer.stop();
		state = STOPPED;
	}
}

void CAController::step() {
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
	
	//CA = new CellularAutomata_GPGPU(cellularDim,initSeed);
	m_view->updateView(CA);
	state = IDLE;
	delete parser;
}


//This is called every x-time based on timer settings
void CAController::tick(){
	
	//TODO this should be drawing as the graphics card
	//is calculating new values...This would require doing an expensive
	//memory copy though...probably worth it.
	m_view->updateView(CA);
	float timeTaken = CA->nextTimeStep();

	qDebug("Time taken:%3.1f ms\n",timeTaken);

}


Abstract2DCA* CAController::getCAClass() {
	Abstract2DCA *outer = new OuterTotalistic();
	Abstract2DCA *gen = new Generations();

	QList<int>* survNums = parser->ruleData.at(0);
	QList<int>* bornNums = parser->ruleData.at(1);

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

	int stateNum = parser->numStates;

	gen->setSurviveNo(surviveNo,survSize);
	gen->setBornNo(bornNo,bornSize);
	gen->setStates(stateNum);
	gen->neighbourhoodType = outer->MOORE;

	return gen;
}

void CAController::createCAFromMCLFormat(QStringList& lines) {
	resetForNewLoad();
	parser = new LexiconParser();
	CA = parser->parseContent(lines);

	setRulesToCA();

	m_view->updateView(CA);
}

void CAController::setRulesToCA(){
	
	Abstract2DCA* caRules = getCAClass();
	CA->setCARule(caRules);
}

void CAController::resetForNewLoad() {

	//Delete and set to null
	if(CA != NULL) {
		delete CA;
	}

	if(parser != NULL) {
		delete parser;
	}

	CA = NULL;
	parser = NULL;
}