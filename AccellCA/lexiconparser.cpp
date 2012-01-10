#include "lexiconparser.h"

LexiconParser::LexiconParser(QObject *parent)
	: QObject(parent)
{

}

LexiconParser::~LexiconParser()
{

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
	int i = 50;
	int initI = i;
	int j = 50;
	int initJ = j;

	QString numBuilder;

	//loop through all characters
	for(int charNum = 0; charNum < data.size(); ++charNum) {

	//get char
		QString c(data.at(charNum));

		bool ok;

		int val = c.toUInt(&ok,10);

		if (val == 5) {
			qDebug("");
		}
		//if we were a number...
		if (ok) {
			numBuilder.append(QString("%1").arg(val));
		}
		else if(c != "$") {

			int numTimes = 1;
			if (numBuilder.size() > 0) {
				//perform this operation numBuilder times
				bool numOk;
				int concatNum = numBuilder.toUInt(&numOk,10);
				if (numOk) {
					numTimes = concatNum;
				}	
			}


			int length = i + numTimes;
			for (i; i < length; ++i) {

				if(c != ".") {
					pFlatGrid[i * DIM + j] = 1;
				}
				
				//qDebug("Adding %c to %d - %d", c.at(0).toAscii(), i, j);
			}
			numBuilder = "";

		}
		else if(c == "$") {
			++j;
			i = initI;
			numBuilder = "";
		}
	}
	return pFlatGrid;
}