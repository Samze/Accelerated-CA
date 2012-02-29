#ifndef FACTORYMAKER_H
#define FACTORYMAKER_H

#include <QObject>
#include "cellularautomatafactory.h"
#include "cellularautomata2dfactory.h"
#include "cellularautomata3dfactory.h"

//This class provides a way of instansiating objects and
//gives us the singleton pattern.

class FactoryMaker : public QObject
{
	Q_OBJECT

public:
	FactoryMaker(QObject *parent);
	~FactoryMaker();


	static CellularAutomataFactory* GetFactory(int dim);

private:
	//Declaration
	static CellularAutomataFactory* factory;
};

#endif // FACTORYMAKER_H
