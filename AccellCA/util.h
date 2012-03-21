#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <Abstract2DCA.h>
#include <Abstract3DCA.h>

class Util : public QObject
{
	Q_OBJECT

public:
	Util(QObject *parent);
	~Util();

	static int* createDynamicListFromQList(const QList<int>& list);
	static QList<int>* createQListFromDynamicList(int* list, int size);

	static QString getNeighbourhoodName(int maxNeighhoodsize);

private:
	
};

#endif // UTIL_H
