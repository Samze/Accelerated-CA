#ifndef MCELLPARSER_H
#define MCELLPARSER_H

#include <QObject>
#include "CellularAutomata.h"
#include "CellularAutomata_GPGPU.h"
#include <QRegExp>
#include <QStringList>
#include <qmap.h>
#include <QStringBuilder>
#include <QDebug>
#include "ruleparser.h"

class MCellParser : public RuleParser
{
	Q_OBJECT

public:
	MCellParser(QObject *parent = 0);
	~MCellParser();

public:
	AbstractCellularAutomata* parseContent(QStringList& list);
	QList<QList<int>*> ruleData;
	unsigned int numStates;

private:
	QString getTag(const QString& line);
	QString getData(const QString& line, const QString& tag);

	QMap<QString,QString> tagDataMap;

	
	unsigned int DIM;

	int parseBoardSize(const QString& data);
	void parseRules(const QString& data);
	unsigned int* parseLattice(const QString& data);
	unsigned int* centerLattice(unsigned int* lattice, int i, int j);
	unsigned int getNumericalForLeter(const char);
	unsigned int getNumberOfStates(QList<int>*);
};

#endif // MCELLPARSER_H
