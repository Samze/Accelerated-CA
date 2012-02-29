#include "CAController.h"

CAController::CAController() {

	state = IDLE;
	connect(&timer, SIGNAL(timeout()),this,SLOT(tick()));
	timerTick = 1; //in Milliseconds
	
	//Default to 2D.
	factory = FactoryMaker::GetFactory(TWO);
}

CAController::CAController(QObject *parent)
	: QObject(parent) 
{

}

CAController::~CAController()
{
	//Do not delete m_view
	delete CA;
	delete factory;
}


void CAController::setRule(AbstractCellularAutomata* caRule) {
	CA->setCARule(caRule);
}


void CAController::setView(ICAView *view) {
	m_view = view;
	//Call back to the viewer giving them our optional GLwidget.
	GLDrawer* drawer = factory->createDrawer();
	
	emit newDrawElement(drawer);
}

void CAController::start() {
	
	if (CA == NULL) {
		qWarning("Enter CA before starting");
	}
	else {
		state = ACTIVE;
		timer.start(timerTick);
	}
}

void CAController::stop() {
	if (state == ACTIVE){
		state = STOPPED;
	}
}

void CAController::step() {
	if(state == ACTIVE) {
		stop();
	}
	else { //TODO This will blow up if nothing is set
		//Run it one tick, then stop.
		state = ACTIVE;
		tick();
		state = STOPPED;
	}
}

void CAController::restart() {
	if (state == ACTIVE) {
		stop();	
	}
	
	m_view->updateView(CA);
	state = IDLE;
}


//This is called every x-time based on timer settings
void CAController::tick(){
	
	if (state == ACTIVE) {
		float timeTaken = CA->nextTimeStep();
		qDebug("Time taken:%3.1f ms\n",timeTaken);

			int DIM = CA->getCARule()->getLattice()->DIM;

//	for(int i = 0; i < DIM; i++) {
		//for(int j = 0; j < DIM; j++) {
		//	for(int k = 0; k < DIM; k++) {

		//		int state = ((unsigned  int*)CA->getCARule()->getLattice()->pFlatGrid)[k * (DIM*DIM) + i * DIM + j];

		//		qDebug("cell x=%d, y=%d, z=%d, livecells=%d",i,j,k,state);

		//	}
		//}
	//}
	}
	


	//Draw constantly.
	m_view->updateView(CA);
}


void CAController::parseDefinition(QStringList& lines) {

	//resetForNewLoad();
	//parser = new LexiconParser();
	//CA = parser->parseContent(lines);

	RuleParser* parser = factory->createParser();

	AbstractCellularAutomata* caRule = parser->parseContent(lines);
	
	CA = new CellularAutomata_GPGPU();

	CA->setCARule(caRule);

	m_view->updateView(CA);
}


void CAController::setDimension(Dimension dim){
	//Set our factory to our type of CA.
	factory = FactoryMaker::GetFactory(dim);

	//update GUI	

	GLDrawer* drawer = factory->createDrawer();
	emit newDrawElement(drawer);
	
}

void CAController::setRulesToCA(){
	
	/*bstract2DCA* caRules = getCAClass();
	CA->setCARule(caRules);*/
}

void CAController::resetForNewLoad() {

	//Delete and set to null
	if(CA != NULL) {
		delete CA;
	}

	CA = NULL;
}

void CAController::setRandomCA(int size, int range){

	CA = new CellularAutomata_GPGPU();

	AbstractCellularAutomata* caRule = factory->createRule(size,range);

	CA->setCARule(caRule);
	m_view->updateView(CA);

}


void CAController::tempCreate3D() {

	//
	//
	//FactoryMaker::Dimension type = FactoryMaker::TWO_D;
	//
	//CellularAutomataFactory* factory = FactoryMaker::GetFactory(type);


	//AbstractCellularAutomata* ca = factory->createRule();
	//GLDrawer* drawer = factory->createDrawer();
	//
	//qDebug("%d", sizeof(int));
	//
	////Abstract3DCA* gen3d = new Generations3D();
	//
	//int* survNo = new int[16];
	//int* bornNo = new int[3];

	////survNo[0] = 26;

	////bornNo[0] = 0;
	////
	////survNo[0] = 5;
	////survNo[1] = 4;

	////bornNo[0] = 4;

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
	//survNo[14] = 12;
	//survNo[15] = 11;

	//bornNo[0] = 17;
	//bornNo[1] = 18;
	//bornNo[2] = 19;

	////bornNo[1] = 1;
	////bornNo[2] = 2;
	////bornNo[3] = 3;
	////bornNo[4] = 4;
	////bornNo[5] = 5;
	////bornNo[6] = 6;
	////bornNo[7] = 7;
	////bornNo[8] = 8;

	////100 , 20

	//int dim = 24;

	//int range = 2;
	//
	//CA = new CellularAutomata_GPGPU();

	////Abstract2DCA* ab2D = new Abstract2DCA(dim,range);
	////ab2D->neighbourhoodType = ab2D->MOORE;

	////Generations* gen = new Generations();
	////gen->lattice = ab2D;
	////gen->setStates(2);
	////gen->setSurviveNo(survNo,2);
	////gen->setBornNo(bornNo,1);
	//	
	//Abstract3DCA* ab3D = new Abstract3DCA(dim,range);
	//ab3D->neighbourhoodType = ab3D->MOORE_3D;
	//
	//unsigned int* cells = new unsigned int[dim * dim * dim];

	//for (int i = 0; i < dim; ++i) {
	//	for (int j = 0; j < dim; ++j) {
	//		for (int k = 0; k < dim; ++k) {

	//		//get random state value bettwen 0 & 1;
	//		int random = std::rand() % range;
	//		//assign
	//		cells[(i * dim) + j + (k * (dim * dim))] = random == range - 1 ? 1 : 0;
	//		//pFlatGrid[(k * DIM * DIM) + (i * DIM) + j] = 1;
	//		}
	//	}
	//}

	//ab3D->pFlatGrid = cells;

	//Generations3D* gen = new Generations3D();
	//gen->lattice = ab3D;
	//gen->setStates(2);
	//gen->setSurviveNo(survNo,16);
	//gen->setBornNo(bornNo,3);

	//CA->setCARule(gen);
}