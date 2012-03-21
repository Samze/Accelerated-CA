#include "gl3ddrawer.h"

GL3DDrawer::GL3DDrawer() {
	
	//TODO These are all specific to 3D, should not be here
	sceneZoom = -15;
	yRot = yRotDefault;
	xRot = xRotDefault;

	mouseCurrentlyDown = false;
}


GL3DDrawer::GL3DDrawer(QWidget *parent) : GLDrawer(parent)
{
	VBO = new QGLBuffer(QGLBuffer::VertexBuffer);
	IBO = new QGLBuffer(QGLBuffer::IndexBuffer);

}

GL3DDrawer::~GL3DDrawer()
{
	
	VBO->release(QGLBuffer::VertexBuffer);
	IBO->release(QGLBuffer::IndexBuffer);
}

void GL3DDrawer::paintGL(){
	
	if (mouseCurrentlyDown) {
		float normalisedW = ((float)(5 * 2) / width()) * xDifference;
		float normalisedH = ((float)(5 * 2) / height()) * yDifference;
	
		yRot -= normalisedW;
		xRot -= normalisedH;
	}
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	glTranslatef(0,0,sceneZoom);

	glColor3f(0.0,1.0,0.0);
	
	glRotatef(xRot,1,0,0);
	glRotatef(yRot,0,1,0);	

	drawWireFrameCube();	
	//glCullFace(GL_BACK);
	
	if(CA != NULL) {
	////this should not be passed

	bool dim2D;
	//unsigned int dim = CA->getDIM() * CA->getDIM();

	unsigned int DIM = CA->getCARule()->getLattice()->xDIM;


	float cellSpace = ((float) width() /DIM) / width() ;

	float trans = -(cellSpace * DIM/2) + cellSpace/2;

	glTranslatef(trans,trans,trans);
	
	unsigned int* grid = (unsigned int*)CA->getCARule()->getLattice()->pFlatGrid;
	
	Abstract3DCA* v3 = dynamic_cast<Abstract3DCA*>(CA->getCARule()->getLattice());
	
	int count = 0;
	//TODO this is bad.
	for(int i = 0; i < DIM; i++) {
		for(int j = 0; j < DIM; j++) {	
				for(int k = 0; k < DIM; k++) {
					int state = grid[k * (DIM*DIM) + i * DIM + j];
					unsigned int* neighbours = v3->neighbourCount;
					int neighCount = neighbours[(k * DIM * DIM) + (i * DIM) + j];
					if (state >  0) {
						//if (neighCount != 26) {
							CellPos pos;
							pos.x = i;
							pos.y = j;
							pos.z = k;
							drawCell(pos,cellSpace,state);
					//	}
						//else {
						//	count++;
						//}
					}
					//qDebug("x = %d, y = %d, z = %d, state = %d, liveNeighs = %d", i,j,k, grid[(k * DIM * DIM) + (i * DIM) + j],neighCount);
				}
			}
		}
	//qDebug("Number skipped = %d",count);
	}
	//qDebug("Done");
	//rot += 1;

}

void GL3DDrawer::drawCell(CellPos pos, float cellSpace,int state) {
	
	
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;


	float r = 0;
	float g = 0;
	float b = 0;

	Totalistic* totalistic = static_cast<Totalistic*>(CA->getCARule());

	int states = totalistic->getNoStates();

	//int states = CA->getDIM() * CA->getDIM() * CA->getDIM();

	//float colourValue = 1 - ((float)state / states);
	//r = colourValue;
	
	float third = (float)states / 3;

	//float r = state < third ? state/third : 0;
	//float g = state < third && state < third * 2 ? state/third * 2: 0;
	//float b = state < third * 3? state/third * 3: 0;

	int stateRange = (state / third);

	int val = state - (stateRange * third);

	float colourValue = 1 - ((float)val / third);
	
	if(stateRange == 0) r = colourValue;
	if(stateRange == 1) g = colourValue;
	if(stateRange >= 2) b = colourValue;
	
	//Creates cool coloured map

	int DIM = CA->getCARule()->getLattice()->xDIM;

	r = ((float)1.0 / DIM) * x;
	g = ((float)1.0 / DIM) * y;
	b = ((float)1.0 / DIM) * z;

//	glMaterialf(r,g,b);
	glColor3f(r,g,b);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(cellSpace * x,cellSpace * y, (cellSpace * z) - 5.5f);

	glPushMatrix();
	glTranslatef(pos.x * cellSpace,pos.y * cellSpace, pos.z * cellSpace);
	
	//glTranslatef(x/CA->getDIM(),y/CA->getDIM(), z/CA->getDIM());
	//glTranslatef((float)x/CA->getDIM(),(float)y/CA->getDIM(), (float)z/CA->getDIM());
	//glScalef(1/cellSpace - 0.1, 1/cellSpace- 0.1, 1/cellSpace- 0.1); //minus an ammount here to get a "grid" look if desired

	//glRotatef(30,0.0f,1.0f,0.0f);
	//glTranslatef(0,0, -35.5);
	//glRotatef(rot,0.0f,1.0f,0.0f);
	//glRotatef(rot,1.0f,1.0f,1.0f);

	//include this comment to allow for a seperation of cubes.
	float scaledSpace = cellSpace ;//- (cellSpace * 0.1);
	glScalef(scaledSpace,scaledSpace, scaledSpace);
	
	//glScalef(cellSpace- 0.05,cellSpace- 0.05,cellSpace - 0.05);
	//draw3DCell();
	//draw3DElements();
	//drawVBO();
	glCallList(id);
	glPopMatrix();
}

void GL3DDrawer::initializeGL() {

	QGLFormat format;

	format.defaultFormat();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glEnable(GL_POLYGON_SMOOTH);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2, 0.2, 0.2, 1);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-0.5,0.5,-0.5,0.5,5,50);

	//id = createCombinedDisplayList();
	

	id = createDisplayList();
	//createVBO();	
	//bool resultVBO = VBO->bind();
	//bool resultIBO = IBO->bind();
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

void GL3DDrawer::draw3DElements() {

	GLubyte indices[] =  {0,1,2,3,   // 24 of indices
                0,3,4,5,
                0,5,6,1,
				1,6,7,2,
                7,4,3,2,
                4,7,6,5};

	GLfloat vertices[] =  {0.5f, 0.5f, -0.5f,
						-0.5f, 0.5f, -0.5f,
						-0.5f,-0.5f,-0.5f,
						0.5f,-0.5f,-0.5f,
						0.5f,-0.5f, 0.5f,
						0.5f, 0.5f, 0.5f,
						-0.5f, 0.5f, 0.5f,
						-0.5f,-0.5f, 0.5f};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,vertices);

	glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,indices);

	glDisableClientState(GL_VERTEX_ARRAY);

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

void GL3DDrawer::drawWireFrameCube(){

	glBegin(GL_LINES);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f); glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f( 0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);  glVertex3f(-0.5f, -0.5f, 0.5f);

	glVertex3f( 0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f,-0.5f, -0.5f); glVertex3f( 0.5f,-0.5f, -0.5f);
    glVertex3f( 0.5f, 0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);  glVertex3f(-0.5f, -0.5f, -0.5f);

    glVertex3f(-0.5f, 0.5f, -0.5f);  glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);  glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);  glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);  glVertex3f(-0.5f, -0.5f, 0.5f);
    
    glEnd();
}

void GL3DDrawer::drawVBO() {
	
	//VBO->setUsagePattern(QGLBuffer::StaticDraw);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawElements(GL_QUADS,24, GL_UNSIGNED_BYTE,0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

void GL3DDrawer::createVBO() {
	GLubyte indices[] =  {0,1,2,3,   // 24 of indices
                0,3,4,5,
                0,5,6,1,
				1,6,7,2,
                7,4,3,2,
                4,7,6,5};

	GLfloat vertices[] =  {0.5f, 0.5f, -0.5f,
						-0.5f, 0.5f, -0.5f,
						-0.5f,-0.5f,-0.5f,
						0.5f,-0.5f,-0.5f,
						0.5f,-0.5f, 0.5f,
						0.5f, 0.5f, 0.5f,
						-0.5f, 0.5f, 0.5f,
						-0.5f,-0.5f, 0.5f};

	bool resultVBO;
	bool resultIBO;

	resultVBO = VBO->create();
	resultIBO = IBO->create();

	if(!resultVBO || !resultIBO ) {
		qWarning("GL implementation does not support buffers or there is no QGLContext");
	}
	
	resultVBO = VBO->bind();
	resultIBO = IBO->bind();

	if(!resultVBO || !resultIBO ) {
		qWarning("Unable to bind");
	}

	VBO->setUsagePattern(QGLBuffer::StaticDraw);
	IBO->setUsagePattern(QGLBuffer::StaticDraw);

	VBO->allocate(vertices,sizeof(GLfloat) * 24);
	IBO->allocate(indices,sizeof(GLfloat) * 24);

	qDebug("%d",VBO->bufferId());
	qDebug("%d",IBO->bufferId());

	if (VBO->size() != sizeof(GLfloat) * 24) {
		qWarning("VBO datasizes do not match");
	}

}

void GL3DDrawer::createCombinedVBO() {


}

GLuint GL3DDrawer::createDisplayList() {

	GLuint cubeDL;

	cubeDL = glGenLists(1);

	glNewList(cubeDL,GL_COMPILE);
		draw3DCell();
		//drawWireFrameCube();
	glEndList();


	return cubeDL;

}


GLuint GL3DDrawer::createCombinedDisplayList() {

	GLuint cubeDL;

	cubeDL = glGenLists(1);
	/*
	unsigned int* grid = CA->getGrid();
	
	unsigned int dim = CA->getDIM() * CA->getDIM();*/
	float cellSpace = ((float) width() / 50) / width() ;

	glNewList(cubeDL,GL_COMPILE);

	for(int i = 0; i < 50; i++) {
		for(int j = 0; j < 50; j++) {	
				for(int k = 0; k < 50; k++) {
			
					glPushMatrix();
					glTranslatef(i * cellSpace,j * cellSpace, k * cellSpace);
	
					float scaledSpace = cellSpace - (cellSpace * 0.1);
					glScalef(scaledSpace,scaledSpace, scaledSpace);
					draw3DCell();
		
					glPopMatrix();

				}
			}
	}
	glEndList();

	return cubeDL;
}
