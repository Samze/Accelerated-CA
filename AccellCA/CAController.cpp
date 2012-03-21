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

			int DIM = CA->getCARule()->getLattice()->xDIM;

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
	emit newCA(CA);
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

void CAController::setRandomLattice(int range){
	
	if(CA != NULL) {
		int size = CA->getCARule()->getLattice()->xDIM;
		int neigh = CA->getCARule()->getLattice()->neighbourhoodType;

		QString neighStr = Util::getNeighbourhoodName(neigh);

		AbstractLattice* newLattice = factory->createLattice(size,neighStr,range);
		
		//HOTFIX this needs fixing in the framework, this is temp fix.
		int maxBits = CA->getCARule()->getLattice()->maxBits;
		int noBits = CA->getCARule()->getLattice()->noBits;
		newLattice->maxBits = maxBits;
		newLattice->noBits = noBits;

		CA->getCARule()->setLattice(newLattice);
		m_view->updateView(CA);
		emit newCA(CA);
	}
}

void CAController::createNewCA(const QString& type, int latticeSize, const QString& neighbourType, 
								int numStates, const QList<int>& survNum,  const QList<int>& bornNum){
	
	CA = new CellularAutomata_GPGPU();

	//Get lattice
	AbstractLattice* lattice = factory->createLattice(latticeSize,neighbourType,2);


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