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

	if(maxNeighhoodsize == Abstract3DCA::MOORE_3D || maxNeighhoodsize == Abstract2DCA::MOORE) {
		return QString("Moore");
	}
	else if(maxNeighhoodsize == Abstract3DCA::VON_NEUMANN_3D || maxNeighhoodsize == Abstract2DCA::VON_NEUMANN) {
		return QString("Von Neumann");
	}
	return NULL;
}