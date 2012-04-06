#include "cellularautomata3dfactory.h"

CellularAutomata3DFactory::CellularAutomata3DFactory(QObject *parent)
	: CellularAutomataFactory(parent)
{

}

CellularAutomata3DFactory::~CellularAutomata3DFactory()
{

}

AbstractCellularAutomata* CellularAutomata3DFactory::createRule(const QString& type){
	

	if(type == "Generations"){
		Generations3D* caRule = new Generations3D();
		return caRule;
	}else if(type == "Outer Totalistic"){
		OuterTotalistic3D* caRule = new OuterTotalistic3D();
		return caRule;
	}
	else {
		qWarning("Type not recognised");
	}

}

AbstractLattice* CellularAutomata3DFactory::createLattice(int size, const QString& neighbourhoodType, int random){

	//Create our 2D Lattice and add it to our rule.
	Lattice3D* lattice = new Lattice3D(size,size,size,random);

	if(neighbourhoodType == "Moore"){
		lattice->setNeighbourhoodType(Lattice3D::MOORE_3D);
	}
	else if(neighbourhoodType == "Von Neumann"){
		lattice->setNeighbourhoodType(Lattice3D::VON_NEUMANN_3D);
	}
	else {
		qWarning("Neighbourhood type not recognised");
	}
	return lattice;
}

AbstractLattice* CellularAutomata3DFactory::createLattice(int size, const QString& neighbourhoodType, int* grid){

	Lattice3D* lattice = new Lattice3D(grid,size,size,size);

	if(neighbourhoodType == "Moore"){
		lattice->setNeighbourhoodType(Lattice3D::MOORE_3D);
	}
	else if(neighbourhoodType == "Von Neumann"){
		lattice->setNeighbourhoodType(Lattice3D::VON_NEUMANN_3D);
	}
	else {
		qWarning("Neighbourhood type not recognised");
	}
	return lattice;
}


AbstractCellularAutomata* CellularAutomata3DFactory::createRule(int size, int range) {
	
	Generations3D* generations = new Generations3D();
	
		
	int* survNo = new int[16];
	int* bornNo = new int[3];


	bornNo[0] = 17;
	bornNo[1] = 18;
	bornNo[2] = 19;

	survNo[0] = 26;
	survNo[1] = 25;
	survNo[2] = 24;
	survNo[3] = 23;
	survNo[4] = 22;
	survNo[5] = 21;
	survNo[6] = 20;
	survNo[7] = 19;
	survNo[8] = 18;
	survNo[9] = 17;
	survNo[10] = 16;
	survNo[11] = 15;
	survNo[12] = 14;
	survNo[13] = 13;
	survNo[14] = 12;
	survNo[15] = 11;
		
	//int* survNo = new int[1];
	//int* bornNo = new int[1];

	//survNo[0] = 26;

	//Create our 2D Lattice and add it to our rule.
	Lattice3D* lattice = new Lattice3D(size,size,size,range);
	lattice->setNeighbourhoodType(Lattice3D::MOORE_3D);

	generations->lattice = lattice;

	generations->setStates(2);
	generations->setSurviveNo(survNo,16);
	generations->setBornNo(bornNo,3);


	return generations;
	
}


GLDrawer* CellularAutomata3DFactory::createDrawer() {

	//Create our 2D drawer object.
	GLDrawer* drawer = new GL3DDrawer();
	return drawer;
}