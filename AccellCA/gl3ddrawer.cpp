#include "gl3ddrawer.h"

GL3DDrawer::GL3DDrawer(QWidget *parent) : GLDrawer(parent)
{
	rot = 0;
}

GL3DDrawer::~GL3DDrawer()
{

}

void GL3DDrawer::paintGL(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	glTranslatef(-0.5,-0.5,-20);
	//glRotatef(30,-1.0f,0.0f,0.0f);
	glRotatef(rot,0,1,0);

	if(CA != NULL) {
	////this should not be passed

	bool dim2D;
	unsigned int dim = CA->getDIM() * CA->getDIM();

	float cellSpace = ((float) width() / CA->getDIM()) / width() ;

	unsigned int* grid = CA->getGrid();

	int count = 0;
	//TODO this is bad.
	for(int i = 0; i < CA->getDIM(); i++) {
		for(int j = 0; j < CA->getDIM(); j++) {	
				for(int k = 0; k < CA->getDIM(); k++) {
					if (grid[k * dim + i * CA->getDIM() + j] >  0) {
						CellPos pos;
						pos.x = i;
						pos.y = j;
						pos.z = k;
						drawCell(pos,cellSpace,grid[k * dim + i * CA->getDIM() + j]);
					}
					
					//qDebug("x = %d, y = %d, z = %d, state = %d", i,j,k, grid[k * dim + i * CA->getDIM() + j]);
					count++;
				}
			}
		}
	qDebug("%d",count);
	}

	rot += 0.3;
}

void GL3DDrawer::drawCell(CellPos pos, float cellSpace,int state) {
	

	float x = pos.x;
	float y = pos.y;
	float z = pos.z;


	float r = 0;
	float g = 0;
	float b = 0;

	/*int states = CA->getCARule()->getNoStates();*/
	int states = CA->getDIM() * CA->getDIM() * CA->getDIM();

	//float colourValue = 1 - ((float)state / states);
	//r = colourValue;

	
	float third = (float)states / 3;

	////float r = state < third ? state/third : 0;
	////float g = state < third && state < third * 2 ? state/third * 2: 0;
	////float b = state < third * 3? state/third * 3: 0;

	int stateRange = (state / third);

	int val = state - (stateRange * third);

	float colourValue = 1 - ((float)val / third);
	
	if(stateRange == 0) r = colourValue;
	if(stateRange == 1) g = colourValue;
	if(stateRange >= 2) b = colourValue;



	glColor3f(r,g,b);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(cellSpace * x,cellSpace * y, (cellSpace * z) - 5.5f);

	glPushMatrix();
	glTranslatef(x * cellSpace,y * cellSpace, z * cellSpace);
	//glTranslatef(x/CA->getDIM(),y/CA->getDIM(), z/CA->getDIM());
	//glTranslatef((float)x/CA->getDIM(),(float)y/CA->getDIM(), (float)z/CA->getDIM());
	//glScalef(1/cellSpace - 0.1, 1/cellSpace- 0.1, 1/cellSpace- 0.1); //minus an ammount here to get a "grid" look if desired

	//glRotatef(30,0.0f,1.0f,0.0f);

	//glTranslatef(0,0, -35.5);
	//glRotatef(rot,0.0f,1.0f,0.0f);
	//glRotatef(rot,1.0f,1.0f,1.0f);

	float scaledSpace = cellSpace - (cellSpace * 0.1);
	glScalef(scaledSpace,scaledSpace, scaledSpace);
	//glScalef(cellSpace- 0.05,cellSpace- 0.05,cellSpace - 0.05);
	draw3DCell();

	glPopMatrix();
}

void GL3DDrawer::initializeGL() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
   /* glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-0.5,0.5,-0.5,0.5,5,50);
}


//ran on opening scene too
void GL3DDrawer::resizeGL(int w, int h){

	//setup view
	glViewport(0, 0, w, h);
	
	//clean slate && origin set to top left


	//set model and reset matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GL3DDrawer::draw3DCell() {
    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f); glVertex3f( 0.5f,-0.5f, 0.5f);

    glNormal3f( 0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f,-0.5f,-0.5f);  glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f( 0.5f, 0.5f, -0.5f); glVertex3f( 0.5f,-0.5f,-0.5f);

    glNormal3f( 0.0f, 1.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, 0.5f);  glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);

    glNormal3f( 0.0f,-1.0f, 0.0f);
    glVertex3f(-0.5f,-0.5f,-0.5f);  glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);  glVertex3f(-0.5f,-0.5f, 0.5f);

    glNormal3f( 1.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f( 0.5f, -0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f); glVertex3f( 0.5f, 0.5f,-0.5f);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f,-0.5f); glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f,-0.5f);
    
    glEnd();

}