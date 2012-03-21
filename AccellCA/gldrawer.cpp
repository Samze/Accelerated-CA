#include "gldrawer.h"

GLDrawer::GLDrawer() {
	CA = NULL;
}


GLDrawer::GLDrawer(QWidget *parent)
	: QGLWidget(parent), CA(0)
{
	setGeometry(parent->geometry());
	//flicker problem because of a widget attribute...this may be the soltuion.
	setAttribute(Qt::WidgetAttribute::WA_OpaquePaintEvent );

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);


}

GLDrawer::~GLDrawer()
{

}

void GLDrawer::initializeGL() {

	int val = QGLWidget::context()->format().openGLVersionFlags();



    glDisable(GL_TEXTURE_2D); // no need for setting the depth up
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}

void GLDrawer::paintGL(){
	
    glClear(GL_COLOR_BUFFER_BIT);

	if(CA != NULL) {
	////this should not be passed

	unsigned int DIM = CA->getCARule()->getLattice()->xDIM;

	float cellSpace = (float) width() / DIM;

	void* grid = CA->getCARule()->getLattice()->pFlatGrid;

	unsigned int* intGrid = (unsigned int*)grid;

	for(int i = 0; i < DIM; ++i) {
		for(int j = 0; j < DIM; ++j) {
				if (intGrid[i * DIM + j] > 0) {
					//draw!

					CellPos pos;
					pos.x = i;
					pos.y = j;

					drawCell(pos,cellSpace,intGrid[i * DIM + j]);
				}
			}
		}

	return;
	SCIARA2::Cell* cellGrid = (SCIARA2::Cell*)grid;

	//TODO this is bad.
	for(int i = 0; i < DIM; i++) {
		for(int j = 0; j < DIM; j++) {	
			if (cellGrid[i * DIM + j].altitude > 0) { 

				SCIARA2::Cell cell = cellGrid[i * DIM + j];
				float altitude = cell.altitude;
				float thickness = cell.thickness;

				float  flowN = cell.outflow[0];
				float  flowE = cell.outflow[1];
				float  flowW = cell.outflow[2];
				float  flowS = cell.outflow[3];
			

				float flowAvg = (float) (flowN + flowE + flowS + flowW) / 4;
				
				if(j == 10) {
					//qDebug("x = %d, y = %d, alti = %3.3f, thick = %3.3f, flowN = %3.3f, flowE = %3.3f, flowS = %3.3f, flowW = %3.3f", i,j, altitude,thickness,flowN,flowE,flowS,flowW);
				}
				if (thickness != 0) {
					CellPos pos;
					pos.x = i;
					pos.y = j;
					drawCell(pos,cellSpace,(int)thickness);
				}

			}
		}
	}

	}
}
	
	//glTranslatef(1.0f,0.0f,0.0f);

 //   glBegin(GL_POLYGON);
	//	glVertex2f(0,0);
	//	glVertex2f(100,300);
	//	glVertex2f(300,100);
 //   glEnd();


//void GLDrawer::drawCell(int x, int y, float cellSpace, int state) {
//
//	float r = 0;
//	float g = 0;
//	float b = 0;
//
//	/*int states = CA->getCARule()->getNoStates();*/
//	int states = CA->getDIM() * CA->getDIM() * CA->getDIM();
//
//	//float colourValue = 1 - ((float)state / states);
//	//r = colourValue;
//
//	
//	float third = (float)states / 3;
//
//	////float r = state < third ? state/third : 0;
//	////float g = state < third && state < third * 2 ? state/third * 2: 0;
//	////float b = state < third * 3? state/third * 3: 0;
//
//	int stateRange = (state / third);
//
//	int val = state - (stateRange * third);
//
//	float colourValue = 1 - ((float)val / third);
//	
//	if(stateRange == 0) r = colourValue;
//	if(stateRange == 1) g = colourValue;
//	if(stateRange >= 2) b = colourValue;
//
//
//
//	glColor3f(r,g,b);
//
//
//	glLoadIdentity();
//	glTranslatef(x* cellSpace,y * cellSpace,0.0f);
//	glScalef(cellSpace - 1, cellSpace - 1, 0.0f); //minus an ammount here to get a "grid" look if desired
//	//glScalef(9.0, 9.0,0.0f);
//	glBegin(GL_QUADS);
//		glVertex2f(0,0); //top left
//		glVertex2f(1,0); //top right
//		glVertex2f(1,1); //bottom right
//		glVertex2f(0,1); //bottom left
//	glEnd();
//}

//ran on opening scene too
void GLDrawer::resizeGL(int w, int h){

	//setup view
	glViewport(0, 0, w, h);
	
	//clean slate && origin set to top left
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);

	//set model and reset matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLDrawer::mousePressEvent(QMouseEvent *event) {
   // qDebug("%d, %d\n", event->x(), event->y());
}

void GLDrawer::mouseReleaseEvent(QMouseEvent *event) {
   // qDebug("%d, %d\n", event->x(), event->y());
	mouseCurrentlyDown = false;
}

void GLDrawer::mouseMoveEvent(QMouseEvent *event) {
   // qDebug("%d, %d\n", event->x(), event->y());
	
	mouseCurrentlyDown = true;

	//Get the center position
	float midW = width() / 2;
	float midH = height() / 2;

	//Get our co-ords
	float xPos = event->x();
	float yPos = event->y();

	//We want to normalise to this amount, further from center means closer to max
	float maxRotate = 5;

	xDifference = midW - xPos;
	yDifference = midH - yPos;

}

void GLDrawer::wheelEvent(QWheelEvent *event) {

	int delta = event->delta();

	if (delta >= 0) {
		sceneZoom += 1;
	}
	else {
		sceneZoom -= 1;
	}

	event->ignore();
}

void GLDrawer::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
	case Qt::Key_Left:
		yRot -= 5;
		break;
	case Qt::Key_Right:
		yRot += 5;
		break;
	case Qt::Key_Up:
		xRot += 5;
		break;
	case Qt::Key_Down:
		xRot -= 5;
		break;
	case Qt::Key_Return:
		xRot = xRotDefault;
		yRot = yRotDefault;
		break;
	case Qt::Key_PageDown:
		sceneZoom -= 1;
		break;
	case Qt::Key_PageUp:
		sceneZoom += 1;
		break;
    default:
        event->ignore();
        break;
    }
}