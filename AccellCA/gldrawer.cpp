#include "gldrawer.h"

GLDrawer::GLDrawer(QWidget *parent)
	: QGLWidget(parent), CA(0)
{
	setGeometry(parent->geometry());
	//flicker problem because of a widget attribute...this may be the soltuion.
	setAttribute(Qt::WidgetAttribute::WA_OpaquePaintEvent );

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	//TODO These are all specific to 3D, should not be here
	sceneZoom = -15;
	yRot = yRotDefault;
	xRot = xRotDefault;

}

GLDrawer::~GLDrawer()
{

}

void GLDrawer::initializeGL() {
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

	unsigned int dim= CA->getDIM();

	float cellSpace = (float) width() / dim;

	unsigned int* grid = CA->getGrid();

	//unsigned int* grid = new unsigned int[CA->getDIM() * CA->getDIM() * CA->getDIM()];
	//
	//int count = 1;
	//for(int i = 0; i < CA->getDIM(); i++) {
	//	for(int j = 0; j < CA->getDIM(); j++) {		
	//		for(int k = 0; k < CA->getDIM(); k++) {
	//			grid[k * dim + i * CA->getDIM() + j] = count;
	//			qDebug("%d",count);
	//			count++;
	//		}
	//	}
	//}

	//TODO this is bad.
	for(int i = 0; i < CA->getDIM(); i++) {
		for(int j = 0; j < CA->getDIM(); j++) {	
			if (grid[i * CA->getDIM() + j] > 0) { 
				CellPos pos;
					pos.x = i;
					pos.y = j;
					drawCell(pos,cellSpace,grid[i * CA->getDIM() + j]);
			}
			
		}
	}
	}

}
	//for(int i = 0; i < dim; ++i) {
	//	for(int j = 0; j < CA->getDIM(); ++j) {
	//			if (grid[i * CA->getDIM() + j] > 0) {
	//				//draw!

	//				CellPos pos;
	//				pos.x = i;
	//				pos.y = j;

	//				drawCell(pos,cellSpace,grid[i * CA->getDIM() + j]);
	//			}
	//		}
	//	}
	//}
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
    gluOrtho2D(0, w, h, 0);

	//set model and reset matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLDrawer::mousePressEvent(QMouseEvent *event) {
   // qDebug("%d, %d\n", event->x(), event->y());

}
void GLDrawer::mouseMoveEvent(QMouseEvent *event) {
    qDebug("%d, %d\n", event->x(), event->y());

	//Get the center position
	float midW = width() / 2;
	float midH = height() / 2;

	//Get our co-ords
	float xPos = event->x();
	float yPos = event->y();

	//We want to normalise to this amount, further from center means closer to max
	float maxRotate = 5;

	float xDifference = midW - xPos;
	float yDifference = midH - yPos;

	float normalisedW = ((float)(5 * 2) / width()) * xDifference;
	float normalisedH = ((float)(5 * 2) / height()) * yDifference;
	
	yRot -= normalisedW;
	xRot -= normalisedH;


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
    default:
        event->ignore();
        break;
    }
}