#include "WinDisplay.h"

WinDisplay::WinDisplay(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setController(CAController::getInstance());
	//HINSTANCE hinstLib; 
 //   MYPROC ProcAdd; 
 //   BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 

	//hinstLib = LoadLibrary(TEXT("CellularAutomataCudaLib.dll")); 

	//if (hinstLib != NULL) 
 //   { 
 //       ProcAdd = (MYPROC) GetProcAddress(hinstLib, "myPuts"); 
 //
 //       // If the function address is valid, call the function.
 //
 //       if (NULL != ProcAdd) 
 //       {
 //           fRunTimeLinkSuccess = TRUE;
 //           (ProcAdd) (L"Message sent to the DLL function\n"); 
 //       }
 //       // Free the DLL module.
 //
 //       fFreeResult = FreeLibrary(hinstLib); 
 //   } 
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
	
	connect(ui.btnStep,SIGNAL(clicked()),ui.glWidget,SLOT());
	
}