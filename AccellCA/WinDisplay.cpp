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
}

WinDisplay::~WinDisplay()
{

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

	connect(ui.btnStart,SIGNAL(clicked()),&controller,SLOT(start()));
	connect(ui.btnStop,SIGNAL(clicked()),&controller,SLOT(stop()));
	connect(ui.btnStep,SIGNAL(pressed()),&controller,SLOT(step()));
	
	connect(ui.btnRestart,SIGNAL(clicked()),&controller,SLOT(restart()));	
	connect(ui.btnLoadFile,SIGNAL(clicked()),&controller,SLOT(stop()));
	
	connect(ui.btnRandom,SIGNAL(clicked()),this,SLOT(setRandomCA()));

	connect(this,SIGNAL(setCAFromMCLFormat(QStringList&)),&controller,SLOT(parseDefinition(QStringList&)));

	connect(&controller,SIGNAL(newDrawElement(GLDrawer*)),this,SLOT(setGLDrawer(GLDrawer*)));
}

void WinDisplay::on_btnLoadFile_clicked() {
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
		drawer->setParent(ui.glWidget);
		drawer->show(); //This flickrs
		drawer->setGeometry(ui.glWidget->geometry());
	}
}


void WinDisplay::setRandomCA(){

	CAController* controller = &CAController::getInstance();

	int size = ui.txtDim->text().toInt();

	int random = ui.txtRange->text().toInt();

	controller->setRandomCA(size,random);

}