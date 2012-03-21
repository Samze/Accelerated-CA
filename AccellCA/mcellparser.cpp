#include "mcellparser.h"

MCellParser::MCellParser(QObject *parent)
	: RuleParser(parent)
{

}

MCellParser::~MCellParser()
{
	foreach(QList<int>* list, ruleData) {
		delete list;
		list = NULL;
	}
}


AbstractCellularAutomata* MCellParser::parseContent(QStringList& lines)
{
	//Create a map with our key/values
	foreach (QString line, lines) {

		QString tag = getTag(line);
		QString data = getData(line,tag);

		if (tagDataMap.contains(tag)) {
			QString existingData = tagDataMap.value(tag);
			existingData.append(data);
			tagDataMap.insert(tag,existingData);
		}
		else {
			tagDataMap.insert(tag,data);
		}
	}


	DIM =  parseBoardSize(tagDataMap.value("#BOARD"));
	parseRules(tagDataMap.value("#RULE"));

	//For now we will assume the last rule entry is the state.
	numStates = getNumberOfStates(ruleData.at(ruleData.size() - 1));
	
	if (numStates == -1) {
		qWarning("Error parsing number of states, assuming 2");
		numStates = 2;
	}

	unsigned int *pFlatGrid = parseLattice(tagDataMap.value("#L"));

	QList<int>* survNums = ruleData.at(0);
	QList<int>* bornNums = ruleData.at(1);

	//Get our dynamic list of survNums
	int* surviveNo = Util::createDynamicListFromQList(*survNums);
	int survSize = (*survNums).size();

	//Get our dynamic list of bornNums
	int* bornNo = Util::createDynamicListFromQList(*bornNums);
	int bornSize = (*bornNums).size();

	int stateNum = numStates;

	Generations* gen = new Generations();

	Abstract2DCA* ca2d = new Abstract2DCA();
	ca2d->neighbourhoodType = Abstract2DCA::MOORE;
	ca2d->xDIM = DIM;
	ca2d->yDIM = DIM;
	
	gen->lattice = ca2d;

	gen->setSurviveNo(surviveNo,survSize);
	gen->setBornNo(bornNo,bornSize);
	gen->setStates(stateNum);


	ca2d->pFlatGrid = (void*)pFlatGrid;

	return gen;
}


QString MCellParser::getTag(const QString& line){
	    
	QRegExp rx(".+\\s");    
	QStringList caplist;

	int pos = rx.indexIn(line);
	if (pos > -1) {
		caplist = rx.capturedTexts();
	}
		
	if (caplist.isEmpty()) {
		 return 0;
	}

	QString returnString(caplist.at(0).trimmed());
	return returnString;
}

QString MCellParser::getData(const QString& line, const QString& tag) {
	
	int index = line.indexOf(tag);

	QString data(line.mid(index + tag.size()));

	data = data.trimmed();

	return data;
}


int MCellParser::parseBoardSize(const QString& data) {

	//Will be in the format 200x200
	QStringList list = data.split("x");

	bool ok;
	unsigned int result = list.at(0).toUInt(&ok,10); //base 10

	if(ok) {
		return result;
	}
	return 0;
}

void MCellParser::parseRules(const QString& data) {
	
	QStringList list = data.split("/");

	foreach(QString item, list) {
			QList<int>* intList = new QList<int>;

			for(unsigned int i =0; i < item.size();i++) {
				bool ok;
				int value = QString(item.at(i)).toUInt(&ok,10);
				if(ok) {
					intList->append(value);
				}
			}
			ruleData.append(intList);
	}
}

unsigned int* MCellParser::parseLattice(const QString& data) {
	unsigned int* pFlatGrid = new unsigned int[DIM * DIM];
	

	//Zero everything..
	for(unsigned int k = 0; k < DIM * DIM; k++) {
		pFlatGrid[k] = 0;
	}
	
	//TODO add different states.
	//TODO center the grid..
	//set inital values
	int i = 0;
	int initI = i;
	int highestI = 0;

	int j = 0;
	int initJ = j;

	QString numBuilder;

	//loop through all characters
	for(int charNum = 0; charNum < data.size(); ++charNum) {

	//get char
		QString c(data.at(charNum));

		bool ok;

		int val = c.toUInt(&ok,10);

		//if we were a number...
		if (ok) {
			numBuilder.append(QString("%1").arg(val));
		}
		else {

			int numTimes = 1;
			if (numBuilder.size() > 0) {
				//perform this operation numBuilder times
				bool numOk;
				int concatNum = numBuilder.toUInt(&numOk,10);
				if (numOk) {
					numTimes = concatNum;
				}	
			}
			
			//Are we moving rows or columns?
			//Get move length
			int length;
			int l;
			if (c == "$") {
				length =  j + numTimes;
				l = j;
			}
			else {
				length =  i + numTimes;
				 l = i;
			}

			
			for (l; l < length; ++l) {
				if(c != "." && c != "$") {
					unsigned int temp = getNumericalForLeter(c.at(0).toAscii());
					pFlatGrid[l * DIM + j] = temp;
				}
				//qDebug("Adding %c to %d - %d", c.at(0).toAscii(), i, j);
			}
			
			//Add our temp counter back to our i/k
			if (c == "$") {
				j += numTimes;

				highestI = i > highestI? i: highestI;
				i = initI;
			}
			else {
				i += numTimes;
			}
			numBuilder = "";
		}
	}
	unsigned int* centeredLattice = centerLattice(pFlatGrid,highestI,j);

	delete[]pFlatGrid;

	return centeredLattice;
}


unsigned int* MCellParser::centerLattice(unsigned int* lattice, int i, int j){

	unsigned int* pFlatGrid = new unsigned int[DIM * DIM];
	//Zero everything...for performance could use a ternary operator below instead..
	for(unsigned int k = 0; k < DIM * DIM; k++) {
		pFlatGrid[k] = 0;
	}


	int x = (DIM / 2) - (i/2);
	int y = (DIM / 2) - (j/2);

	//We add 1 to account for 0 being a valid row/column
	int xLimit = x + i + 1;
	int yLimit = y + j + 1;
	
	int iterI = 0;
	int iterJ = 0;
	
	for(x; x < xLimit; x++) {
		iterJ = 0;
		for(int iterY = y; iterY < yLimit; iterY++) {

			int latticeVal = lattice[iterI * DIM + iterJ];

			if (latticeVal > 0) {
				pFlatGrid[x * DIM + iterY] = latticeVal;
			}
			iterJ++;
		}
		iterI++;
	}

	return pFlatGrid;
}

unsigned int MCellParser::getNumericalForLeter(const char c) {

	//64 is the ACSII number for A - 1, must be caps
	unsigned int t = (int)c - 64;
	return t;
}

unsigned int MCellParser::getNumberOfStates(QList<int>* stateAsIntList) {

	QString builtState;

	for(int i = 0; i < stateAsIntList->size(); i++) {	
		int digit = stateAsIntList->at(i);
		builtState.append(QString("%1").arg(digit));
	}

	bool ok;

	int val = builtState.toUInt(&ok,10);

	//if we were a number...
	if (ok) {
		return val;
	}
	return -1;
}

