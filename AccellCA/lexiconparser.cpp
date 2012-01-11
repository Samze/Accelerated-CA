#include "lexiconparser.h"

LexiconParser::LexiconParser(QObject *parent)
	: QObject(parent)
{

}

LexiconParser::~LexiconParser()
{
	foreach(QList<int>* list, ruleData) {
		delete list;
		list = NULL;
	}
}


CellularAutomata* LexiconParser::parseContent(QStringList& lines)
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
	
	CellularAutomata* CA = new CellularAutomata_GPGPU(pFlatGrid,DIM);

	return CA;
}


QString LexiconParser::getTag(const QString& line){
	    
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

QString LexiconParser::getData(const QString& line, const QString& tag) {
	
	int index = line.indexOf(tag);

	QString data(line.mid(index + tag.size()));

	data = data.trimmed();

	return data;
}


int LexiconParser::parseBoardSize(const QString& data) {

	//Will be in the format 200x200
	QStringList list = data.split("x");

	bool ok;
	unsigned int result = list.at(0).toUInt(&ok,10); //base 10

	if(ok) {
		return result;
	}
	return 0;
}

void LexiconParser::parseRules(const QString& data) {
	
	QStringList list = data.split("/");

	foreach(QString item, list) {
		if (!item.isEmpty()) {
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
}

unsigned int* LexiconParser::parseLattice(const QString& data) {

	unsigned int* pFlatGrid = new unsigned int[DIM * DIM];
	

	//Zero everything..
	for(unsigned int k = 0; k < DIM * DIM; k++) {
		pFlatGrid[k] = 0;
	}
	
	//TODO add different states.
	//TODO center the grid..
	//set inital values
	int i = 20;
	int initI = i;
	int j = 20;
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
				i = initI;
			}
			else {
				i += numTimes;
			}
			numBuilder = "";
		}
	}
	return pFlatGrid;
}

unsigned int LexiconParser::getNumericalForLeter(const char c) {

	//64 is the ACSII number for A - 1, must be caps
	unsigned int t = (int)c - 64;
	return t;
}

unsigned int LexiconParser::getNumberOfStates(QList<int>* stateAsIntList) {

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
