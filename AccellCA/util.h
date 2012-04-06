#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <Lattice2D.h>
#include <Lattice3D.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

//Forward declaration

class Util : public QObject
{
	Q_OBJECT

public:
	Util(QObject *parent);
	~Util();

	static int* createDynamicListFromQList(const QList<int>& list);
	static QList<int>* createQListFromDynamicList(int* list, int size);
	static QString getNeighbourhoodName(int maxNeighhoodsize);
	static int* deepArrayCopy(int* list, int size);

	static int getDimensionType(int maxNeighhoodsize);

private:
	
};

#endif // UTIL_H
