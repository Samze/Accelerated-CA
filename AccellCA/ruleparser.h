#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <QObject>
#include <QStringList>
#include "AbstractCellularAutomata.h"

class RuleParser : public QObject
{
	Q_OBJECT

public:
	RuleParser(QObject *parent);
	~RuleParser();
	
	virtual AbstractCellularAutomata* parseContent(QStringList& list) = 0;

private:
	
};

#endif // RULEPARSER_H
