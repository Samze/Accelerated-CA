#include "cellularautomata2dfactory.h"

CellularAutomata2DFactory::CellularAutomata2DFactory(QObject *parent)
	: CellularAutomataFactory(parent)
{

}

CellularAutomata2DFactory::~CellularAutomata2DFactory()
{

}

AbstractCellularAutomata* CellularAutomata2DFactory::createRule(const QString& type){


	if(type == "Generations"){
		Generations* caRule = new Generations();
		return caRule;
	}else if(type == "Outer Totalistic"){
		OuterTotalistic* caRule = new OuterTotalistic();
		return caRule;
	}
	else {
		qWarning("Type not recognised");
	}
}

AbstractCellularAutomata* CellularAutomata2DFactory::createRule(int size, int range) {

	Generations* generations = new Generations();
	
	
	int* survNo = new int[1];
	int* bornNo = new int[1];
	
	survNo[0] = 8;
	//survNo[1] = 7;

	//Create our 2D Lattice and add it to our rule.
	Lattice2D* lattice = new Lattice2D(size,size,range);
	lattice->neighbourhoodType = Lattice2D::MOORE;

	generations->lattice = lattice;

	generations->setStates(2);
	generations->setSurviveNo(survNo,1);
	generations->setBornNo(bornNo,0);

	//Create our 2D Lattice and add it to our rule.
	generations->lattice = lattice;


	return generations;
	
}

AbstractLattice* CellularAutomata2DFactory::createLattice(int size, const QString& neighbourhoodType, int random){

	//Create our 2D Lattice and add it to our rule.
	Lattice2D* lattice = new Lattice2D(size,size,random);

	if(neighbourhoodType == "Moore"){
		lattice->neighbourhoodType = Lattice2D::MOORE;
	}
	else if(neighbourhoodType == "Von Neumann"){
		lattice->neighbourhoodType = Lattice2D::VON_NEUMANN;
	}
	else {
		qWarning("Neighbourhood type not recognised");
	}
	return lattice;
}

AbstractLattice* CellularAutomata2DFactory::createLattice(int size, const QString& neighbourhoodType, int* grid){
	
	Lattice2D* lattice = new Lattice2D(grid,size,size);

	if(neighbourhoodType == "Moore"){
		lattice->neighbourhoodType = Lattice2D::MOORE;
	}
	else if(neighbourhoodType == "Von Neumann"){
		lattice->neighbourhoodType = Lattice2D::VON_NEUMANN;
	}
	else {
		qWarning("Neighbourhood type not recognised");
	}
	return lattice;
}

GLDrawer* CellularAutomata2DFactory::createDrawer() {

	//Create our 2D drawer object.
	return new GL2DDrawer();
}


RuleParser* CellularAutomata2DFactory::createParser(){
	return new MCellParser();
}