#include "propertydialog.h"

PropertyDialog::PropertyDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	editMode = false;

	createDefaultSettings();
	populateUISettings();


}

PropertyDialog::PropertyDialog(QWidget* parent,Totalistic* ca){
	ui.setupUi(this);
	editMode = true;

	//TODO Get dim
	dimension = CAController::TWO;

	//Get game type TODO
	gameType = "Generations";

	numStates =ca->getNoStates();

	//TODO
	neighbourhoodType = "Moore";

	//TODO setup maxNeighours.
	maxNeighbours = 8;
	
	//Get surv/born rules
	survNums = Util::createQListFromDynamicList(ca->surviveNo,ca->surviveSize);
	bornNums = Util::createQListFromDynamicList(ca->bornNo,ca->bornSize);

	latticeSize = ca->getLattice()->xDIM;

	populateUISettings();

}

PropertyDialog::~PropertyDialog()
{
	delete survNums;
	delete bornNums;
}

void PropertyDialog::createDefaultSettings() {
	
	//Dimension
	dimension = CAController::TWO;

	//Gametype
	gameType = "Generations";

	numStates = 2;

	//TODO
	neighbourhoodType = "Moore";

	//TODO setup maxNeighours.
	maxNeighbours = 8;
	
	//Get surv/born rules
	survNums = new QList<int>();
	bornNums = new QList<int>();

	latticeSize = 0;

}

void PropertyDialog::populateUISettings() {

	bool is2D;

	if(dimension == CAController::TWO) {
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
	
	//Set surv numbers
	ui.cboSurvStates->clear();
	ui.cboSurvStates->SetDisplayText("Survive States");
	for (int i=0; i<=maxNeighbours; i++)
    {
		ui.cboSurvStates->addItem(QString("%1").arg(i), QVariant(false));
    }

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

	//States 
	ui.cboStates->clear();
    for (int i=2; i<30; i++)
    {
		ui.cboStates->addItem(QString("%1").arg(i));
    }
	
	//Setup states
	int stateIndex = ui.cboStates->findText(QString("%1").arg(numStates));
	ui.cboStates->setCurrentIndex(stateIndex);

	//Set lattice
	ui.txtLatticeSize->setText(QString("%1").arg(latticeSize));

}


void PropertyDialog::on_okButton_clicked(){
	
	if(ui.rdo_2D->isChecked()) {
		dimension = CAController::TWO;
	}
	else {
		dimension = CAController::THREE;
	}

	gameType = ui.cboGameType->currentText();

	numStates = ui.cboStates->currentText().toInt();

	//Surv nums
	survNums->clear();
	for(int i = 0; i < ui.cboSurvStates->count(); i++) {

		if(ui.cboSurvStates->itemData(i).toBool() == true) {
			survNums->append(i);
		}
	}

	//Born nums
	bornNums->clear();
	for(int i = 0; i < ui.cboBornStates->count(); i++) {

		if(ui.cboBornStates->itemData(i).toBool() == true) {
			bornNums->append(i);
		}
	}

	 if(ui.rdoMoore->isChecked()) {
		 neighbourhoodType = "Moore";
	 }else {
 
		 neighbourhoodType = "Von Neumann";
	 }

	latticeSize = ui.txtLatticeSize->text().toInt();
	
	CAController* controller = &CAController::getInstance();

	
	//Tell our controller we're switching the dimension
	controller->setDimension(dimension);

	//Finally create our CA.
	controller->createNewCA(gameType,latticeSize,neighbourhoodType,numStates,*survNums,*bornNums);

}