#include "WinDisplay.h"

WinDisplay::WinDisplay(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	//Null this
	drawer = NULL;
	//Default to 2D

	//Maybe should store controller as a member variable using something like this..
	//CAController* controller = &CAController::getInstance();

	setController(CAController::getInstance());

	connect(ui.radioButton_2,SIGNAL(toggled(bool)),this,SLOT(setDimension2D(bool)));

	maxNeighbours = 8;

	setupGUIElements();

}

WinDisplay::~WinDisplay()
{

}

void WinDisplay::newCALoaded(CellularAutomata *ca){
	//Set our UI elements for our new CA.
	setupGUIElements(ca);
}

//Default setup
void WinDisplay::setupGUIElements(){
	
	ui.cboSurvStates->clear();
	ui.cboSurvStates->SetDisplayText("Survive States");
	for (int i=0; i<=maxNeighbours; i++)
    {
		ui.cboSurvStates->addItem(QString("%1").arg(i), QVariant(false));
    }
	
	ui.cboBornStates->clear();
	ui.cboBornStates->SetDisplayText("Born States");
    for (int i=0; i<=maxNeighbours; i++)
    {
		ui.cboBornStates->addItem(QString("%1").arg(i), QVariant(false));
    }
		
	ui.cboStates->clear();
    for (int i=2; i<30; i++)
    {
		ui.cboStates->addItem(QString("%1").arg(i));
    }
}

//Setup for a CA
void WinDisplay::setupGUIElements(CellularAutomata *ca) {
	
	Totalistic* totalisticCA = dynamic_cast<Totalistic*>(ca->getCARule());

	
	//Rule name
	QString ruleName = QString::fromStdString(ca->getRuleName());
	ui.lblGameType->setText(ruleName);

	//States
	int numStates = totalisticCA->getNoStates();
	ui.lblStates->setText(QString("%1").arg(numStates));

	//Neighbourhood
	QString neighbourhood = Util::getNeighbourhoodName(totalisticCA->getLattice()->neighbourhoodType);
	ui.lblNeighbourhood->setText(neighbourhood);

	//survNum
	QString survString;

	int count = 0;
	while(count < totalisticCA->surviveSize){

		survString.append(QString("%1").arg(totalisticCA->surviveNo[count]));

		count++;
		
		if(count < totalisticCA->surviveSize){
			survString.append(", ");
		}
	}
	ui.lblSurvNums->setText(survString);
	
	
	//bornNum
	QString bornString;

	count = 0;
	while(count < totalisticCA->bornSize){

		bornString.append(QString("%1").arg(totalisticCA->bornNo[count]));

		count++;
		
		if(count < totalisticCA->bornSize){
			bornString.append(", ");
		}
	}

	ui.lblBornNums->setText(bornString);
}

void WinDisplay::setWindowTitle1(QString &title) {

}

void WinDisplay::updateView(CellularAutomata* ca) {
	QTime myTimer;
	myTimer.start();
	drawer->CA  = ca;
	drawer->updateGL();
	qDebug("Render time = %d", myTimer.elapsed());
}

void WinDisplay::setController(CAController &controller) {

	connect(this,SIGNAL(settingsChanged()),&controller,SLOT(stop()));

	connect(ui.btnStart,SIGNAL(clicked()),&controller,SLOT(start()));
	connect(ui.btnStop,SIGNAL(clicked()),&controller,SLOT(stop()));
	connect(ui.btnStep,SIGNAL(pressed()),&controller,SLOT(step()));
	
	connect(ui.btnRestart,SIGNAL(clicked()),&controller,SLOT(restart()));	
	connect(ui.btnLoadFile,SIGNAL(clicked()),&controller,SLOT(stop()));

	connect(this,SIGNAL(setCAFromMCLFormat(QStringList&)),&controller,SLOT(parseDefinition(QStringList&)));

	connect(&controller,SIGNAL(newDrawElement(GLDrawer*)),this,SLOT(setGLDrawer(GLDrawer*)));
	connect(&controller,SIGNAL(newCA(CellularAutomata*)),this,SLOT(newCALoaded(CellularAutomata*)));
}

void WinDisplay::on_btnEditRule_clicked(){
	
	CAController* controller = &CAController::getInstance();

	Totalistic* totalistic = dynamic_cast<Totalistic*>(controller->CA->getCARule());

	PropertyDialog dialog(this,totalistic);

	dialog.setModal(true);
	dialog.exec();
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


void WinDisplay::on_btnRestart_clicked() {
	
	if (!fileContents.isEmpty()) {
		emit setCAFromMCLFormat(fileContents);
	}

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
		drawer = newDrawer;
		drawer->setParent(ui.glWidget3D);
		drawer->show(); //This flickers
		drawer->setGeometry(ui.glWidget3D->geometry());
	}
}


void WinDisplay::on_btnRandom_clicked(){

	CAController* controller = &CAController::getInstance();

	int random = ui.txtRange->text().toInt();

	controller->setRandomLattice(random);
}

//Create a new CA based on the selected rules.
void WinDisplay::on_btnCreateRule_clicked() {

	//Game type
	int gameIndex = ui.cboGameType->currentIndex();
	QString ruleType = ui.cboGameType->itemText(gameIndex);

	//Neighbourhood
	QString neighBourType;
	if(ui.rdoMoore->isChecked()) {
		neighBourType = "Moore";
	}
	else {
		neighBourType = "Von Neumann";
	}

	//Num States
	int stateIndex = ui.cboStates->currentIndex();
	int numStates = ui.cboStates->itemText(stateIndex).toInt();
	
	//Survi Nums
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

	//Lattice size
	int latticeSize = ui.txtLatticeSize->text().toInt();

	
	CAController* controller = &CAController::getInstance();
	controller->createNewCA(ruleType,latticeSize,neighBourType,numStates,survNums,bornNums);
}


void WinDisplay::on_actionNew_triggered(){
	qDebug() << "New CLicked";


	PropertyDialog dialog(this);

	dialog.setModal(true);
	dialog.exec();
}