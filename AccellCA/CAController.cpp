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

	Abstract3DCA* gen3d = new Generations3D();
	
	int* survNo = new int[1];
	int* bornNo = new int[1];

	survNo[0] = 25;
	//survNo[1] = 0;

	//bornNo[0] = 99;
	//bornNo[1] = 1;
	//bornNo[2] = 2;
	//bornNo[3] = 3;
	//bornNo[4] = 4;
	//bornNo[5] = 5;
	//bornNo[6] = 6;
	//bornNo[7] = 7;
	//bornNo[8] = 8;

	gen3d->setSurviveNo(survNo,1);
	gen3d->setBornNo(bornNo,1);
	gen3d->setStates(2);
	gen3d->neighbourhoodType = gen3d->THREEDMOORE;

	CA = new CellularAutomata_GPGPU(4,5);
	CA->setCARule(gen3d);
	CA->generate3DGrid(4,5);

	if (CA == NULL) {
		qWarning("Enter CA before starting");
		return;
	}
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
		tick();
	}
	if(state == STOPPED) {
		tick();
	}
	else {
		start();
		stop();
	}
	//tick
}

void CAController::restart() {
	if(state == IDLE) return;

	if (state == ACTIVE) {
		stop();	
	}
	
	m_view->updateView(CA);
	state = IDLE;
}


//This is called every x-time based on timer settings
void CAController::tick(){
	
	//TODO this should be drawing as the graphics card
	//is calculating new values...This would require doing an expensive
	//memory copy though...probably worth it.
	float timeTaken = CA->nextTimeStep();
	m_view->updateView(CA);

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