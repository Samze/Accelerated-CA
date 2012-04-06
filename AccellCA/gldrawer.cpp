#include "gldrawer.h"

GLDrawer::GLDrawer() {
	CA = NULL;
}


GLDrawer::GLDrawer(QWidget *parent)
	: QGLWidget(parent), CA(0)
{
	//setGeometry(parent->geometry());
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

    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLDrawer::paintGL(){

	glClear(GL_COLOR_BUFFER_BIT);

	if(CA != NULL) {
		////this should not be passed

		unsigned int DIM = CA->getCARule()->getLattice()->getXSize();

		float cellSpace;
		if(width() > height())
			cellSpace = (float) height() / DIM;
		else {
			cellSpace = (float) width() / DIM;
		}



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
	}
}

//ran on opening scene too
void GLDrawer::resizeGL(int w, int h){

	//center our viewport
	int dX = 0;
	int dY = 0;

	if(w > h) {
		dX = w - h;
		w = h;
	}
	else {
		dY = h - w;
		h = w;
	}

	dX /= 2;
	dY /= 2;

	//setup view
	glViewport(dX, dY, w, h);
	
	//clean slate && origin set to top left
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);

	//set model and reset matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	updateGL();
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