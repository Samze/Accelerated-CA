#include "util.h"

Util::Util(QObject *parent)
	: QObject(parent)
{

}

Util::~Util()
{

}

//Returns a dynamic array representation of a QList of ints.
int* Util::createDynamicListFromQList(const QList<int>& list){

	int size = list.size();
	int* newList = new int[size];
	int count = 0;
	//Pull this out into a seperate method
	foreach(int i, list) {
		newList[count] = i;
		++count;
	}

	return newList;
}

//Returns a dynamic array representation of a QList of ints.
QList<int>* Util::createQListFromDynamicList(int* list, int size){

	QList<int>* newList = new QList<int>();

	for(int i = 0; i < size; i++) {

		newList->append(list[i]);

	}
	return newList;
}


QString Util::getNeighbourhoodName(int maxNeighhoodsize){

	if(maxNeighhoodsize == Lattice3D::MOORE_3D || maxNeighhoodsize == Lattice2D::MOORE) {
		return QString("Moore");
	}
	else if(maxNeighhoodsize == Lattice3D::VON_NEUMANN_3D || maxNeighhoodsize == Lattice2D::VON_NEUMANN) {
		return QString("Von Neumann");
	}
	return NULL;
}


int Util::getDimensionType(int maxNeighhoodsize) {

	if(maxNeighhoodsize == Lattice3D::MOORE_3D || maxNeighhoodsize == Lattice3D::VON_NEUMANN_3D) {
		return 3;
	}
	else if(maxNeighhoodsize == Lattice2D::MOORE || maxNeighhoodsize == Lattice2D::VON_NEUMANN) {
		return 2;
	}
	qWarning("Neighbhood not detetected");
	return 0;
}


int* Util::deepArrayCopy(int* list, int size){

	int* newList = new int[size];
	for(int i =0 ; i <size;i++) {
		newList[i] = list[i];
	}
	return newList;
}