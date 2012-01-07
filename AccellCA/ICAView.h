#ifndef ICAVIEW_H
#define ICAVIEW_H

#include <qstring.h>
#include "CellularAutomata.h"
#include "CAController.h"

class CAController;

class ICAView
{
public:
	virtual ~ICAView() {}
	virtual void setWindowTitle1(QString &title) = 0;
	virtual void updateView(CellularAutomata *ca) = 0;
	virtual void setController(CAController& controller) = 0;
	
};

Q_DECLARE_INTERFACE(ICAView, "ICAView/1.0");

#endif // ICAVIEW_H
