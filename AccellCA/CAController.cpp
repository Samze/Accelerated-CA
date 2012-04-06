#include "CAController.h"

CAController::CAController() {

	CA = NULL;
	state = EMPTY;

	connect(&viewTimer, SIGNAL(timeout()),this,SLOT(viewTick()));
	viewTimer.setInterval(0); //draw as fast as possible

	viewTimer.start();

	connect(&caTimer, SIGNAL(timeout()),this,SLOT(caTick()));
	timerTick = 100; // default CA to update very 100ms.
	
	//Default to 2D.
	factory = FactoryMaker::GetFactory(TWO);
	previousLattice = NULL;
	initialLattice = NULL;

	connect(this,SIGNAL(newCA(CellularAutomata*)),this,SLOT(newCALoaded(CellularAutomata*)));

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

	if(initialLattice != NULL) {
		delete[] initialLattice;
	}

	if(previousLattice != NULL) {\
		delete[] previousLattice;
	}
}


void CAController::setRule(AbstractCellularAutomata* caRule) {
	CA->setCARule(caRule);
}


void CAController::setView(ICAView *view) {
	m_view = view;
	////Call back to the viewer giving them our optional GLwidget.
	//GLDrawer* drawer = factory->createDrawer();
	//emit newDrawElement(drawer);
}

void CAController::start(){
	
	if(state == EMPTY){
		qWarning("Enter CA before starting");
	}
	else if(state == ACTIVE) {
		stop();
	}
	else if(state == LOADED || state == STOPPED) { //Start logic here
		changeState(ACTIVE);
		caTimer.start(timerTick);
	}

}

void CAController::stop(){
	
	if(state == ACTIVE) { //stop logic here
		changeState(STOPPED);
	}
}

void CAController::back() {
	if (state == ACTIVE || state == STOPPED) {
		stop();

		int* grid = (int*) CA->getCARule()->getLattice()->pFlatGrid;

		//Delete current lattice
		delete[] grid;

		//copy new one
		CA->getCARule()->getLattice()->pFlatGrid = (void*)Util::deepArrayCopy(previousLattice, CA->getCARule()->getLattice()->getNoElements());

		CA->stepNumber -= 1;

		changeState(STOPPED);
	}
}


void CAController::forward() {
	if(state == ACTIVE) {
		stop();
	}
	else if(state == LOADED || state == STOPPED){
		//Run it one tick, then stop.
		changeState(ACTIVE);
		caTick();
		changeState(STOPPED);
	}
}


void CAController::forwardN(){
	if(state == ACTIVE) {
		stop();
	}
	else if(state == LOADED || state == STOPPED){
		//Don't render, just run!
		int N = 200;

		for(int i = 0; i < N; i++) {
			CA->nextTimeStep();
		}

		changeState(STOPPED);
	}
}

void CAController::restart() {

	if (state == ACTIVE || state == STOPPED) {
		changeState(STOPPED);

		int* grid = (int*) CA->getCARule()->getLattice()->pFlatGrid;

		//Delete current lattice
		delete[] grid;

		//copy new one
		CA->getCARule()->getLattice()->pFlatGrid = (void*)Util::deepArrayCopy(initialLattice, CA->getCARule()->getLattice()->getNoElements());

		CA->stepNumber = 0;

		changeState(LOADED);
	}
}


//This is called every x-time based on timer settings
void CAController::caTick(){
	
	if (state == ACTIVE) {

		//This could take too long..
		if(previousLattice != NULL) {
			delete[] previousLattice;
		}
		
		int* grid = (int*)CA->getCARule()->getLattice()->pFlatGrid;
		int noElements = CA->getCARule()->getLattice()->getNoElements();

		previousLattice = Util::deepArrayCopy(grid,noElements);

		float timeTaken = CA->nextTimeStep();
		qDebug("Time taken:%3.1f ms\n",timeTaken);


//	for(int i = 0; i < DIM; i++) {
		//for(int j = 0; j < DIM; j++) {
		//	for(int k = 0; k < DIM; k++) {

		//		int state = ((unsigned  int*)CA->getCARule()->getLattice()->pFlatGrid)[k * (DIM*DIM) + i * DIM + j];

		//		qDebug("cell x=%d, y=%d, z=%d, livecells=%d",i,j,k,state);

		//	}
		//}
	//}
	}
}
void CAController::viewTick(){
	
	m_view->updateView(CA);
}

void CAController::parseDefinition(QStringList& lines) {

	//resetForNewLoad();
	//parser = new LexiconParser();
	//CA = parser->parseContent(lines);

	//TEMP for now
	setDimension(CAController::TWO);

	RuleParser* parser = factory->createParser();

	AbstractCellularAutomata* caRule = parser->parseContent(lines);
	
	if(CA != NULL) {
		delete CA;
	}

	CA = new CellularAutomata_GPGPU();
	CA->setCARule(caRule);

	m_view->updateView(CA);
	emit newCA(CA);

	delete parser;
}


void CAController::setDimension(Dimension dim){

	if(factory != NULL) {
		delete factory;
	}
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

void CAController::setRandomLattice(int range){
	
	if(CA != NULL) {
		int size = CA->getCARule()->getLattice()->getXSize();
		int neigh = CA->getCARule()->getLattice()->getNeighbourhoodType();

		QString neighStr = Util::getNeighbourhoodName(neigh);

		AbstractLattice* newLattice = factory->createLattice(size,neighStr,range);
		
		//HOTFIX this needs fixing in the framework, this is temp fix.
		int maxBits = CA->getCARule()->getLattice()->getMaxBits();
		int noBits = CA->getCARule()->getLattice()->getNoBits();
		newLattice->setMaxBits(maxBits);
		newLattice->setNoBits(noBits);

		CA->getCARule()->setLattice(newLattice);
		CA->stepNumber = 0;

		m_view->updateView(CA);
		emit newCA(CA);
	}
}

void CAController::createNewCA(const QString& type, int latticeSize, const QString& neighbourType, 
	int numStates, const QList<int>& survNum,  const QList<int>& bornNum, AbstractLattice* lattice){

		

		if(lattice != NULL) {
			//Copy lattice
			int* grid = Util::deepArrayCopy((int*)lattice->pFlatGrid,lattice->getNoElements());
			lattice = factory->createLattice(latticeSize,neighbourType,grid);
		}
		else {
			lattice = factory->createLattice(latticeSize,neighbourType,1);
		}

		if(CA != NULL){
			delete CA; //this will delete the lattice too.
		}

		CA = new CellularAutomata_GPGPU();

		//Get rule using lattice
		AbstractCellularAutomata* caRule = factory->createRule(type);

		caRule->setLattice(lattice);

		Totalistic* totalistCaRule = dynamic_cast<Totalistic*>(caRule);

		//Set states
		totalistCaRule->setStates(numStates);

		int survSize = survNum.size();
		int* survNums = Util::createDynamicListFromQList(survNum);

		int bornSize = bornNum.size();
		int* bornNums = Util::createDynamicListFromQList(bornNum);

		totalistCaRule->setSurviveNo(survNums,survSize);
		totalistCaRule->setBornNo(bornNums,bornSize);

		CA->setCARule(totalistCaRule);

		m_view->updateView(CA);
		emit newCA(CA);
}

void CAController::newCALoaded(CellularAutomata* ca) {

	//When run for the first time after loaded, we want to store starting state.
	if(initialLattice != NULL) {
		delete[] initialLattice;
	}

	//Copy data.
	int noElements = ca->getCARule()->getLattice()->getNoElements();

	int* grid = (int*)ca->getCARule()->getLattice()->pFlatGrid;

	initialLattice = Util::deepArrayCopy(grid,noElements);

	changeState(LOADED);
}

void CAController::setTimerTick(int tick){
	timerTick = tick;
	caTimer.setInterval(timerTick);
}


void CAController::changeState(State newState){

	state = newState;
	emit newCAState(state);
}