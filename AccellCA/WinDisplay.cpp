#include "WinDisplay.h"

WinDisplay::WinDisplay(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setController(CAController::getInstance());
}

WinDisplay::~WinDisplay()
{
}

void WinDisplay::setWindowTitle1(QString &title) {

}

void WinDisplay::updateView(CellularAutomata* ca) {
	ui.glWidget->CA  = ca;
	ui.glWidget->updateGL();
}

void WinDisplay::setController(CAController &controller) {

	connect(ui.btnStart,SIGNAL(clicked()),&controller,SLOT(start()));
	connect(ui.btnStop,SIGNAL(clicked()),&controller,SLOT(stop()));
	connect(ui.btnStep,SIGNAL(clicked()),&controller,SLOT(step()));
	
	connect(ui.btnRestart,SIGNAL(clicked()),&controller,SLOT(restart()));	
	connect(ui.btnLoadFile,SIGNAL(clicked()),&controller,SLOT(stop()));

	connect(this,SIGNAL(setCAFromMCLFormat(QStringList&)),&controller,SLOT(createCAFromMCLFormat(QStringList&)));
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