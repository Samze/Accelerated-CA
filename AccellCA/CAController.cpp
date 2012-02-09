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

	qDebug("%d", sizeof(int));
	
	//Abstract3DCA* gen3d = new Generations3D();
	
	int* survNo = new int[1];
	int* bornNo = new int[1];

	//survNo[0] = 26;

	//bornNo[0] = 0;
	
	survNo[0] = 26;
	//survNo[1] = 3;

	bornNo[0] = 0;

	//survNo[0] = 26;
	//survNo[1] = 25;
	//survNo[2] = 24;
	//survNo[3] = 23;
	//survNo[4] = 22;
	//survNo[5] = 21;
	//survNo[6] = 20;
	//survNo[7] = 19;
	//survNo[8] = 18;
	//survNo[9] = 17;
	//survNo[10] = 16;
	//survNo[11] = 15;
	//survNo[12] = 14;
	//survNo[13] = 13;

	//bornNo[0] = 17;
	//bornNo[1] = 18;
	//bornNo[2] = 19;

	//bornNo[1] = 1;
	//bornNo[2] = 2;
	//bornNo[3] = 3;
	//bornNo[4] = 4;
	//bornNo[5] = 5;
	//bornNo[6] = 6;
	//bornNo[7] = 7;
	//bornNo[8] = 8;

	//100 , 20

	int dim = 50;

	int range = 2;
	
	CA = new CellularAutomata_GPGPU();

	//Abstract2DCA* ab2D = new Abstract2DCA(dim,range);
	//ab2D->neighbourhoodType = ab2D->MOORE;

	//Generations* gen = new Generations();
	//gen->lattice = ab2D;
	//gen->setStates(2);
	//gen->setSurviveNo(survNo,2);
	//gen->setBornNo(bornNo,1);
		
	Abstract3DCA* ab3D = new Abstract3DCA(dim,range);
	ab3D->neighbourhoodType = ab3D->MOORE_3D;

	Generations3D* gen = new Generations3D();
	gen->lattice = ab3D;
	gen->setStates(2);
	gen->setSurviveNo(survNo,1);
	gen->setBornNo(bornNo,1);

	CA->setCARule(gen);

	//CA->generate3DGrid(dim,range);

	//gen3d->neighbourCount = new unsigned int[dim * dim * dim];
	
	//CA->initCudaForGL();

	if (CA == NULL) {
		qWarning("Enter CA before starting");
		return;
	}
	if (state == IDLE) {
		//Create random CA, lower range is more concentrated
		//CA = new CellularAutomata_GPGPU(cellularDim,initSeed);
		m_view->updateView(CA);
	}
	
	//state = ACTIVE;
	//timer.start(timerTick);
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
		//stop();
	}
	
	tick();
	
		//for (int i = 0; i < CA->getDIM(); ++i) {
		//for (int j = 0; j < CA->getDIM(); ++j) {
		//	for (int k = 0; k < CA->getDIM(); ++k) {
		//		if(CA->getGrid()[(k * CA->getDIM() * CA->getDIM()) + (i * CA->getDIM()) + j] == 1) {
		//		//if(CA->getGrid()[(i * CA->getDIM()) + j] == 1) {
		//			qDebug("x = %d, y = %d, z = %d",i,j,k);
		//		}
		//	}
		//}
	
	//qDebug("%3.1f",timeTaken);
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


//Abstract2DCA* CAController::getCAClass() {
//	Abstract2DCA *outer = new OuterTotalistic();
//	Abstract2DCA *gen = new Generations();
//
//	QList<int>* survNums = parser->ruleData.at(0);
//	QList<int>* bornNums = parser->ruleData.at(1);
//
//	int survSize = (*survNums).size();
//	int* surviveNo = new int[survSize];
//	int count = 0;
//	//Pull this out into a seperate method
//	foreach(int i, *survNums) {
//		surviveNo[count] = i;
//		++count;
//	}
//	
//	int bornSize = (*bornNums).size();
//	int* bornNo = new int[bornSize];
//	count = 0;
//	//Pull this out into a seperate method
//	foreach(int i, *bornNums) {
//		bornNo[count] = i;
//		++count;
//	}
//
//	int stateNum = parser->numStates;
//
//	gen->setSurviveNo(surviveNo,survSize);
//	gen->setBornNo(bornNo,bornSize);
//	gen->setStates(stateNum);
//	gen->neighbourhoodType = outer->MOORE;
//
//	return gen;
//}

void CAController::createCAFromMCLFormat(QStringList& lines) {
	resetForNewLoad();
	parser = new LexiconParser();
	CA = parser->parseContent(lines);

	setRulesToCA();

	m_view->updateView(CA);
}

void CAController::setRulesToCA(){
	
	/*Abstract2DCA* caRules = getCAClass();
	CA->setCARule(caRules);*/
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