#include "WinDisplay.h"

WinDisplay::WinDisplay(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	int timerTick = 100; //in Milliseconds
	int CellularDim = 800; //Specifies the width/height for our CA
	int initSeed = 5; //Random "0 to initseed" chance of each cell being live to start with, lower is a higher chance.

	//setup timer
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this,SLOT(tick()));
	timer->start(timerTick);

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

	//Create random CA, lower range is more concentrated
	CA = new CellularAutomata_GPGPU(CellularDim,initSeed);
	 
	//Give pointer to openGL class
	ui.glWidget->CA = CA;
}

WinDisplay::~WinDisplay()
{
	delete timer;
	delete CA;
}

//This is called every x-time based on timer settings
void WinDisplay::tick(){

	OuterTotalistic outer;
	int* surviveNo = new int[1];
	int* bornNo = new int[1];
	surviveNo[0] = 0;
	//surviveNo[1] = 3;
	bornNo[0] = 2;
	outer.setSurviveNo(surviveNo,2);
	outer.setBornNo(bornNo,1);
	outer.setStates(2);
	outer.neighbourhoodType = outer.MOORE;


	Generations gen;
	gen.setSurviveNo(surviveNo,0);
	gen.setBornNo(bornNo,1);
	gen.setStates(3);

	int x = gen.noBits;

	gen.neighbourhoodType = outer.MOORE;

	float timeTaken = CA->nextTimeStep(gen);
	qDebug("Time taken:%3.1f ms\n",timeTaken);
	ui.glWidget->updateGL();
}

