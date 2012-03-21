#ifndef ICAVIEW_H
#define ICAVIEW_H

#include <qstring.h>
#include "CellularAutomata.h"
#include "CAController.h"
#include "gldrawer.h"

class CAController;

class ICAView
{

public:
	virtual ~ICAView() {}

	//This should all probably be slots
	virtual void setWindowTitle1(QString &title) = 0;
	virtual void updateView(CellularAutomata *ca) = 0;
	virtual void setController(CAController& controller) = 0;

private slots:
	virtual void newCALoaded(CellularAutomata *ca) = 0;
	virtual void setGLDrawer(GLDrawer* drawer){}	
	
};

Q_DECLARE_INTERFACE(ICAView, "ICAView/1.0");

#endif // ICAVIEW_H
