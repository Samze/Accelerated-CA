#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <QObject>
#include <QStringList>
#include "AbstractCellularAutomata.h"
#include "util.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

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
