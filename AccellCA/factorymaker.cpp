#include "factorymaker.h"

//Definition
CellularAutomataFactory* FactoryMaker::factory = 0;

FactoryMaker::FactoryMaker(QObject *parent)
	: QObject(parent)
{

}

FactoryMaker::~FactoryMaker()
{

}


CellularAutomataFactory* FactoryMaker::GetFactory(int type){

	//TODO Memory leak here
	//TODO enum
	switch(type) {
		case 2 :
			factory = new CellularAutomata2DFactory();
			break;
		case 3:
			factory = new CellularAutomata3DFactory();
			break;
	};

	return factory;
}
