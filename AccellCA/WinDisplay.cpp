#include "WinDisplay.h"

WinDisplay::WinDisplay(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	CAController& controller = CAController::getInstance();
	controller.setView(this);

	//Null this
	drawer = NULL;

	//Maybe should store controller as a member variable using something like this..
	//CAController* controller = &CAController::getInstance();

	setController(controller);

	createDefaultCA();

	//States 
	ui.cboStates->clear();
    for (int i=2; i<30; i++)
    {
		ui.cboStates->addItem(QString("%1").arg(i));
    }
	
	setupConnections(true);

	//file system model
	qDebug() << QDir::currentPath();

	dirModel = new QFileSystemModel(this);
	dirModel->setRootPath(QDir::currentPath());

	dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
	
	dirModel->setNameFilterDisables(false);
	QStringList nameFilters("*.mcl");
 	dirModel->setNameFilters(nameFilters);

	ui.treeView->setModel(dirModel);
	ui.treeView->setRootIndex(dirModel->index(QDir::currentPath()));

	ui.treeView->hideColumn(1); //size
	ui.treeView->hideColumn(2); //type
	ui.treeView->hideColumn(3); //modified

	//Name window
	setWindowTitle(QString("Accelerated Cellular Automata"));

}

WinDisplay::~WinDisplay()
{

}

void WinDisplay::setupConnections(bool enableConnections) {

	if(enableConnections) {
		//Dimensions
		connect(ui.rdo_2D,SIGNAL(clicked()),this,SLOT(newLatticeSettingsChanged()));
		connect(ui.rdo_3D,SIGNAL(clicked()),this,SLOT(newLatticeSettingsChanged()));

		//GameType
		connect(ui.cboGameType,SIGNAL(currentIndexChanged(int)),this,SLOT(controlsChanged()));
	
		//States
		connect(ui.cboStates,SIGNAL(editTextChanged(QString)),this,SLOT(controlsChanged()));
		
		//Game Rule states
		connect(ui.cboBornStates,SIGNAL(itemChanged()),this,SLOT(controlsChanged()));
		connect(ui.cboSurvStates,SIGNAL(itemChanged()),this,SLOT(controlsChanged()));

		//Neighbours
		connect(ui.rdoMoore,SIGNAL(clicked()),this,SLOT(controlsChanged()));
		connect(ui.rdoVonNeumann,SIGNAL(clicked()),this,SLOT(controlsChanged()));

		//Lattice
		connect(ui.txtLatticeSize,SIGNAL(textEdited(QString)),this,SLOT(newLatticeSettingsChanged()));
	}
	else {
				//Dimensions
		disconnect(ui.rdo_2D,SIGNAL(clicked()),this,SLOT(newLatticeSettingsChanged()));
		disconnect(ui.rdo_3D,SIGNAL(clicked()),this,SLOT(newLatticeSettingsChanged()));

		//GameType
		disconnect(ui.cboGameType,SIGNAL(currentIndexChanged(int)),this,SLOT(controlsChanged()));
	
		//States
		disconnect(ui.cboStates,SIGNAL(editTextChanged(QString)),this,SLOT(controlsChanged()));
		
		//Game Rule states
		disconnect(ui.cboBornStates,SIGNAL(itemChanged()),this,SLOT(controlsChanged()));
		disconnect(ui.cboSurvStates,SIGNAL(itemChanged()),this,SLOT(controlsChanged()));

		//Neighbours
		disconnect(ui.rdoMoore,SIGNAL(clicked()),this,SLOT(controlsChanged()));
		disconnect(ui.rdoVonNeumann,SIGNAL(clicked()),this,SLOT(controlsChanged()));

		//Lattice
		disconnect(ui.txtLatticeSize,SIGNAL(textEdited(QString)),this,SLOT(newLatticeSettingsChanged()));
	}
}

void WinDisplay::createDefaultCA() {
	
	CAController* controller = &CAController::getInstance();

	//Tell our controller we're switching the dimension
	controller->setDimension(CAController::TWO);

	//Finally create our CA.
	QList<int> survNums;
	//survNums.append(1);
	//survNums.append(2);

	QList<int> bornNums;
	bornNums.append(0);
	//bornNums.append(4);

	controller->createNewCA("Generations",20,"Moore",2,survNums,bornNums);
}

void WinDisplay::newCALoaded(CellularAutomata *ca){
	//Because we're re populating the gui we dont want to fire signals.
	setupConnections(false);

	//Set our UI elements for our new CA.
	setupGUIElements(ca);

	//ok done, re-enable signals for user intput
	setupConnections(true);
}

//Setup for a CA
void WinDisplay::setupGUIElements(CellularAutomata *ca) {
	
	QString gameType = QString::fromStdString(ca->getRuleName());
	Totalistic* totalisticCA = dynamic_cast<Totalistic*>(ca->getCARule());
	
	int maxNeighbourhood = totalisticCA->getLattice()->neighbourhoodType;

	int dim = Util::getDimensionType(maxNeighbourhood);

	QString neighbourhoodType= Util::getNeighbourhoodName(maxNeighbourhood);

	bool is2D;
	if(dim == CAController::TWO) {
		is2D = true;
	}
	else {
		is2D = false;
	}

	ui.rdo_2D->setChecked(is2D);
	ui.rdo_3D->setChecked(!is2D);
	

	//Setup game type
	int typeIndex = ui.cboGameType->findText(gameType);
	ui.cboGameType->setCurrentIndex(typeIndex);

	//Set radio buttons up
	bool isMoore;
	if(neighbourhoodType == "Moore") {
		isMoore = true;
	}else {
		isMoore = false;
	}

	ui.rdoMoore->setChecked(isMoore);
	ui.rdoVonNeumann->setChecked(!isMoore);

	//Setup states
	int stateIndex = ui.cboStates->findText(QString("%1").arg(totalisticCA->getNoStates()));
	
	if(gameType == "Outer Totalistic"){
		ui.cboStates->setCurrentIndex(0); //Max of 2 states.
		ui.cboStates->setEnabled(false);
	}
	else {
		ui.cboStates->setEnabled(true);
		ui.cboStates->setCurrentIndex(stateIndex);
	}

	//Set lattice
	ui.txtLatticeSize->setText(QString("%1").arg(totalisticCA->getLattice()->xDIM));

	updateRuleComboBoxes(ca);

}

void WinDisplay::updateRuleComboBoxes(CellularAutomata *ca) {

	Totalistic* totalisticCA = dynamic_cast<Totalistic*>(ca->getCARule());
	int maxNeighbours = totalisticCA->getLattice()->neighbourhoodType;

	//Set surv numbers
	ui.cboSurvStates->clear();
	ui.cboSurvStates->SetDisplayText("Survive States");
	for (int i=0; i<=maxNeighbours; i++)
    {
		ui.cboSurvStates->addItem(QString("%1").arg(i), QVariant(false));
    }

	QList<int>* survNums = Util::createQListFromDynamicList(totalisticCA->surviveNo,totalisticCA->surviveSize);
	QList<int>* bornNums = Util::createQListFromDynamicList(totalisticCA->bornNo,totalisticCA->bornSize);
	//Add surv data, if we have any
	foreach(int item, *survNums) {

		QString strItem = QString("%1").arg(item);

		int survIndex = ui.cboSurvStates->findText(strItem);
		ui.cboSurvStates->setItemData(survIndex,QVariant(true));
	}

	//Set born numbers
	ui.cboBornStates->clear();
	ui.cboBornStates->SetDisplayText("Born States");
    for (int i=0; i<=maxNeighbours; i++)
    {
		ui.cboBornStates->addItem(QString("%1").arg(i), QVariant(false));
    }

	//Add surv data, if we have any
	foreach(int item, *bornNums) {
		
		QString strItem = QString("%1").arg(item);

		int bornIndex = ui.cboBornStates->findText(strItem);
		ui.cboBornStates->setItemData(bornIndex,QVariant(true));
	}

	delete survNums;
	delete bornNums;
}

void WinDisplay::controlsChanged(){

	CAController::Dimension dimension;

	if(ui.rdo_2D->isChecked()) {
		dimension = CAController::TWO;
	}
	else {
		dimension = CAController::THREE;
	}

	QString gameType = ui.cboGameType->currentText();

	int numStates = ui.cboStates->currentText().toInt();

	//Surv nums
	QList<int> survNums;
	for(int i = 0; i < ui.cboSurvStates->count(); i++) {

		if(ui.cboSurvStates->itemData(i).toBool() == true) {
			survNums.append(i);
		}
	}
	

	//Born nums
	QList<int> bornNums;
	for(int i = 0; i < ui.cboBornStates->count(); i++) {

		if(ui.cboBornStates->itemData(i).toBool() == true) {
			bornNums.append(i);
		}
	}
	
	QString neighbourhoodType;

	 if(ui.rdoMoore->isChecked()) {
		 neighbourhoodType = "Moore";
	 }else {
 
		 neighbourhoodType = "Von Neumann";
	 }

	int latticeSize = ui.txtLatticeSize->text().toInt();
	
	CAController* controller = &CAController::getInstance();
	//Tell our controller we're switching the dimension
	controller->setDimension(dimension);

	AbstractLattice* lattice = controller->CA->getCARule()->getLattice();
	if(lattice != NULL && newLatticeRequired == false) {
		//Finally create our CA.
		controller->createNewCA(gameType,latticeSize,neighbourhoodType,numStates,survNums,bornNums,lattice);
	}
	else {
		controller->createNewCA(gameType,latticeSize,neighbourhoodType,numStates,survNums,bornNums);
	}
	
	newLatticeRequired = false;
}

void WinDisplay::newLatticeSettingsChanged(){
	
	newLatticeRequired = true;
	controlsChanged();
}

void WinDisplay::updateView(CellularAutomata* ca) {
	QTime myTimer;
	myTimer.start();
	drawer->CA  = ca;
	drawer->updateGL();

	//ui.glWidget->CA = ca;
	//ui.glWidget->updateGL();

	qDebug("Render time = %d", myTimer.elapsed());

	//set step number
	ui.lblStepNo->setText(QString("%1").arg(ca->stepNumber));
}

void WinDisplay::setController(CAController &controller) {

	connect(this,SIGNAL(settingsChanged()),&controller,SLOT(stop()));

	//StartStop
	connect(ui.btnStartStop,SIGNAL(clicked()),&controller,SLOT(start()));

	//Back
	connect(ui.btnBack,SIGNAL(clicked()),&controller,SLOT(back()));

	//Restart
	connect(ui.btnRestart,SIGNAL(pressed()),&controller,SLOT(restart()));
	
	//Forward
	connect(ui.btnForward,SIGNAL(clicked()),&controller,SLOT(forward()));	

	//ForwardN
	connect(ui.btnForwardN,SIGNAL(clicked()),&controller,SLOT(forwardN()));	

	connect(this,SIGNAL(setCAFromMCLFormat(QStringList&)),&controller,SLOT(parseDefinition(QStringList&)));

	connect(&controller,SIGNAL(newDrawElement(GLDrawer*)),this,SLOT(setGLDrawer(GLDrawer*)));
	connect(&controller,SIGNAL(newCA(CellularAutomata*)),this,SLOT(newCALoaded(CellularAutomata*)));

	//Inform ui on change of state
	connect(&controller,SIGNAL(newCAState(CAController::State)),this,SLOT(handleChangeState(CAController::State)));
}


void WinDisplay::on_actionLoad_triggered() {
	 QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "/home",
                                                 tr("MCell documents (*.mcl)"));

	 if (!fileName.isNull()) {
		LoadFile(fileName);
	 }
}

void  WinDisplay::LoadFile(const QString &fileName) {
	fileContents.clear();

	QFile file(fileName);

	 if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,"Error opening file","File could not be opened.",QMessageBox::Close);
        return;
    }
	 
	 QTextStream inStream(&file);

	 while(!inStream.atEnd()) {
		 fileContents.append(inStream.readLine());
	 }

	 emit setCAFromMCLFormat(fileContents);

	 file.close();

}

void WinDisplay::setDimension2D(bool twoDim){

	CAController* controller = &CAController::getInstance();

	if(twoDim == true) {
		controller->setDimension(CAController::TWO);
	}
	else {
		controller->setDimension(CAController::THREE);
	}
}

void WinDisplay::setGLDrawer(GLDrawer* newDrawer){
	
	if(drawer == NULL || drawer != newDrawer) {

		//delete drawer
		delete drawer;
		drawer = NULL;

		//Add a new one.
		drawer = newDrawer;
		drawer->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);

		ui.glLayout->addWidget(drawer);
		//ui.glLayout->removeWidget(&temp);

	}
}


void WinDisplay::on_btnRandom_clicked(){

	CAController* controller = &CAController::getInstance();

	int random = ui.txtRange->text().toInt();

	controller->setRandomLattice(random);
}

void WinDisplay::on_actionNew_triggered(){
	qDebug() << "New CLicked";

}

void WinDisplay::on_treeView_clicked(QModelIndex index) {

	QString sPath = dirModel->fileInfo(index).absoluteFilePath();

	qDebug() << sPath;	
	if (!sPath.isNull()) {
		LoadFile(sPath);
	 }
}

void WinDisplay::on_sldSpeed_valueChanged(int value) {



	//Dynamically calc speed, with max value being 1, max as 500;

	
	int max = ui.sldSpeed->maximum();

	float val = value * ((float)MAX_TIMER_INTERVAL/max);


	int newTimerValue = MAX_TIMER_INTERVAL - val;



	CAController::getInstance().setTimerTick(newTimerValue);

}

void WinDisplay::handleChangeState(CAController::State newState) {

	switch(newState) {
		case CAController::ACTIVE:
			ui.btnStartStop->setText("Pause");
			break;
		case CAController::STOPPED:
			ui.btnStartStop->setText("Start");
			break;
		case CAController::LOADED:
			ui.btnStartStop->setText("Start");
			break;
		case CAController::EMPTY:
			break;
	}

}