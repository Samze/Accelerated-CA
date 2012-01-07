#ifndef GLDRAWER_H
#define GLDRAWER_H

#include <QGLWidget>
#include <QtGui/QMouseEvent>
#include "CellularAutomata.h"
/**
	This class is an implement of QTs support for openGL. It inherits from QGLWidget
	which offers familiar openGL methods such as paint, init etc. This class is used
	for all elements in relation to representing a Cellular Automata.
	TODO ADD MORE HERE

*/
class GLDrawer : public QGLWidget
{
	/**
	QObject.
	This class inherits from QOBJECT
	*/
	Q_OBJECT

public:
	/**
	* A constructor. 
	* Default Constructor.
	*/
	GLDrawer(QWidget *parent);
	/**
	* A destructor. 
	* Default destructor.
	*/
	~GLDrawer();

	/**
	* Representation of a Cellular Automata class, this should probably be
	* accessed from a getter.
	* TODO
	*/
	CellularAutomata *CA;

protected:
	void paintGL();
	void initalizeGL();
	void resizeGL(int w, int h);

	void drawCell(int x, int y, float cellSpace,int);

	//events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
	GLfloat rot;
	int cellScale;

};

#endif // GLDRAWER_H
