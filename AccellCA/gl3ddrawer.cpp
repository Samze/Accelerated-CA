#include "gl3ddrawer.h"

GL3DDrawer::GL3DDrawer() {
	
	//TODO These are all specific to 3D, should not be here
	VBO = new QGLBuffer(QGLBuffer::VertexBuffer);
	IBO = new QGLBuffer(QGLBuffer::IndexBuffer);

	sceneZoom = -15;
	yRot = yRotDefault;
	xRot = xRotDefault;

	mouseCurrentlyDown = false;
	setupVBO = false;

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	vertexList = NULL;
	indexList = NULL;
	
}


GL3DDrawer::GL3DDrawer(QWidget *parent) : GLDrawer(parent)
{

	VBO = new QGLBuffer(QGLBuffer::VertexBuffer);
	IBO = new QGLBuffer(QGLBuffer::IndexBuffer);

	sceneZoom = -15;
	yRot = yRotDefault;
	xRot = xRotDefault;

	mouseCurrentlyDown = false;
	setupVBO = false;

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	
	vertexList = NULL;
	indexList = NULL;
}

GL3DDrawer::~GL3DDrawer()
{
	if(vertexList != NULL)
		delete[] vertexList;

	if(indexList != NULL)
		delete[] indexList;

	VBO->release(QGLBuffer::VertexBuffer);
	IBO->release(QGLBuffer::IndexBuffer);

	delete VBO;
	delete IBO;
}

void GL3DDrawer::paintGL(){

	//paintGLVBO();
	//return;

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
	
	Lattice3D* v3 = dynamic_cast<Lattice3D*>(CA->getCARule()->getLattice());
	
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

	if(states == 2) {
		int DIM = CA->getCARule()->getLattice()->xDIM;
			
		//Creates cool coloured map
		r = ((float)1.0 / DIM) * x;
		g = ((float)1.0 / DIM) * y;
		b = ((float)1.0 / DIM) * z;
	}
	else {
		float third = (float)states / 3;

		//float r = state < third ? state/third : 0;
		//float g = state < third && state < third * 2 ? state/third * 2: 0;
		//float b = state < third * 3? state/third * 3: 0;

		int stateRange = (state / third);

		int val = state - (stateRange * third);

		float colourValue = 1 - ((float)val / third);
	/*
		if(stateRange == 0) r = colourValue;
		if(stateRange == 1) g = colourValue;
		if(stateRange >= 2) b = colourValue;*/

		float colourValue2 = 1 - (((float)state / states) * 0.8) + 0.2;
		r = colourValue2;

	}
	//int states = CA->getDIM() * CA->getDIM() * CA->getDIM();

	//float colourValue = 1 - ((float)state / states);
	//r = colourValue;
	

	
	glColor3f(r,g,b);

	glPushMatrix();
	glTranslatef(pos.x * cellSpace,pos.y * cellSpace, pos.z * cellSpace);
	
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
	glEnable(GL_CULL_FACE); //Does this do anything?
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

	glDisable(GL_LIGHTING);

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

	glEnable(GL_LIGHTING);
}

void GL3DDrawer::paintGLVBO(){

	if(!setupVBO) {
		populateVertexList();
		createVBO();
		setupVBO = true;
	}

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
		glScalef(0.01,0.01,0.01);

		populateIndexBuffer();
		drawVBO();
	}
}

void GL3DDrawer::populateIndexBuffer(){

	unsigned int DIM = CA->getCARule()->getLattice()->xDIM;

	unsigned int* grid = (unsigned int*)CA->getCARule()->getLattice()->pFlatGrid;

	Lattice3D* v3 = dynamic_cast<Lattice3D*>(CA->getCARule()->getLattice());

	QVector<VEC3> list;

	int count = 0;
	//TODO this is bad.
	for(int i = 0; i < DIM; i++) {
		for(int j = 0; j < DIM; j++) {	
			for(int k = 0; k < DIM; k++) {
				int state = grid[k * (DIM*DIM) + i * DIM + j];
				unsigned int* neighbours = v3->neighbourCount;
				int neighCount = neighbours[(k * DIM * DIM) + (i * DIM) + j];
				if (state >  0) {
					VEC3 v;
					v.x = i;
					v.y = j;
					v.z = k;
					list.append(v);
				}
			}
		}
	}

	GLuint* data = (GLuint*)IBO->map(QGLBuffer::WriteOnly);

	
	if(data) {

		int size2 = DIM + 1;
		int sizeSquared =  size2 * size2;

		int pos = 0;
		foreach(VEC3 cell , list) {

			int x = cell.x;
			int y = cell.y;
			int z = cell.z;


			int XYz = ((sizeSquared) * z) + (size2 * (y+1)) + x + 1; // 1,1,0		0
			int xYz =  ((sizeSquared) * z) + (size2 * (y + 1)) + x; // 0,1,0			1
			int xyz =  ((sizeSquared) * z) + (size2 * y) + x; // 0,0,0				2
			int Xyz = ((sizeSquared) * z) + (size2 * y) + x + 1; // 1,0,0			3
			int XyZ = ((sizeSquared) * (z + 1)) + (size2 * y) + x + 1; // 1,0,1		4
			int XYZ = ((sizeSquared) * (z + 1)) + (size2 * (y+1)) + x + 1; // 1,1,1	5
			int xYZ = ((sizeSquared) * (z + 1)) + (size2 * (y+1)) + x; // 0,1,1		6
			int xyZ =  ((sizeSquared) * (z + 1)) + (size2 * y) + x; // 0,0,1			7

			GLuint indices[] =  {XYz,xYz,xyz,Xyz,   // 24 of indices
				XYz,Xyz,XyZ,XYZ,
				XYz,XYZ,xYZ,xYz,
				xYz,xYZ,xyZ,xyz,
				xyZ,XyZ,Xyz,xyz,
				XyZ,xyZ,xYZ,XYZ};

			for(int i = 0; i < 8 ; i++) {

				int idx = i * 3;

				data[pos + idx] = indices[idx];
				data[pos + idx + 1] = indices[idx + 1];
				data[pos + idx + 2] = indices[idx + 2];
			}


			pos += 24;
		}
		newIndiceNum = pos;
		IBO->unmap();
	}
}



void GL3DDrawer::drawVBO() {
	
	//VBO->setUsagePattern(QGLBuffer::StaticDraw);
	int size = CA->getCARule()->getLattice()->xDIM;

	int indexNum = size * size * size * 24;

	int indexNum2 = newIndiceNum;

	int vertexNum = (size + 1) * (size +1) * (size + 1) * 3;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3,GL_FLOAT,0,0);
	glColorPointer(3,GL_FLOAT,0,(void*)(sizeof(GLfloat) * vertexNum));

	glDrawElements(GL_QUADS,indexNum2, GL_UNSIGNED_INT,0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void GL3DDrawer::populateVertexList() {
	
	QTime timer;

	timer.start();
	int size = CA->getCARule()->getLattice()->xDIM;

	GLfloat vertices[] =  {0.5f, 0.5f, -0.5f,
						-0.5f, 0.5f, -0.5f,
						-0.5f,-0.5f,-0.5f,
						0.5f,-0.5f,-0.5f,
						0.5f,-0.5f, 0.5f,
						0.5f, 0.5f, 0.5f,
						-0.5f, 0.5f, 0.5f,
						-0.5f,-0.5f, 0.5f};


	GLfloat line[] = {1.0f, 1.0f, 1.0f};
					//0.0f, 0.0f, 1.0f};
	
	QVector<VEC3*> vertexVec;

	for(int i = 0; i <= size; i++) {

		for(int j = 0; j <= size; j++) {

			for(int k = 0; k <= size; k++) { //This is here on purpose!
				
				//for(int vertIdx = 0; vertIdx < 3; vertIdx++){

					QVector3D v;

					v.setX(line[0]);
					v.setY(line[1]);
					v.setZ(line[2]);

					QMatrix4x4 trans;

					trans.translate(i,j,k);

					QVector3D v2 = trans * v;

					VEC3* vec = new VEC3();

					vec->x = v2.x();
					vec->y = v2.y();
					vec->z = v2.z();

					vertexVec.append(vec);
				//}

			}
		}
	}

	vertexList = new GLfloat[vertexVec.size() * 3];

	for(int i = 0; i < vertexVec.size(); i++){
		vertexList[i * 3] = vertexVec.at(i)->x;
		vertexList[i * 3 + 1] = vertexVec.at(i)->y;
		vertexList[i * 3 + 2] = vertexVec.at(i)->z;
	}
	
	qDebug("vertex done = %d", timer.elapsed());

	timer.start();

	int sizeSquared = (size + 1)* (size +1);
	indexList = new GLuint[size * size * size * 24];

	QVector<GLuint> indexVec;

	//populate index list
	for(int x = 0; x < size; ++x) {
		for(int y = 0; y < size; ++y) {
			for(int z = 0; z < size; ++z) {

					//Caps indicates + 1;
	
				//QVector<GLuint>* list = getIndicesForCell(x,y,z);

					int size = CA->getCARule()->getLattice()->xDIM;

				int size2 = size + 1;

				int sizeSquared =  size2 * size2;

				int XYz = ((sizeSquared) * z) + (size2 * (y+1)) + x + 1; // 1,1,0		0
				int xYz =  ((sizeSquared) * z) + (size2 * (y + 1)) + x; // 0,1,0			1
				int xyz =  ((sizeSquared) * z) + (size2 * y) + x; // 0,0,0				2
				int Xyz = ((sizeSquared) * z) + (size2 * y) + x + 1; // 1,0,0			3
				int XyZ = ((sizeSquared) * (z + 1)) + (size2 * y) + x + 1; // 1,0,1		4
				int XYZ = ((sizeSquared) * (z + 1)) + (size2 * (y+1)) + x + 1; // 1,1,1	5
				int xYZ = ((sizeSquared) * (z + 1)) + (size2 * (y+1)) + x; // 0,1,1		6
				int xyZ =  ((sizeSquared) * (z + 1)) + (size2 * y) + x; // 0,0,1			7

				GLuint indices[] =  {XYz,xYz,xyz,Xyz,   // 24 of indices
					XYz,Xyz,XyZ,XYZ,
					XYz,XYZ,xYZ,xYz,
					xYz,xYZ,xyZ,xyz,
					xyZ,XyZ,Xyz,xyz,
					XyZ,xyZ,xYZ,XYZ};

				for(int i = 0; i < 24; i++){
					indexVec.append(indices[i]);
				}

				//delete list;
			}
		}
	}

	for(int i = 0; i < indexVec.size(); i++){
		indexList[i] = indexVec.at(i);
	}
	
	qDebug("index done = %d", timer.elapsed());
	qDebug() << "done";
}


void GL3DDrawer::createVBO() {
	

	int caSize = CA->getCARule()->getLattice()->xDIM;
	int caSize2 = caSize + 1;

	int vertexNum = caSize2 * caSize2 * caSize2 * 3;
	int indexNum = caSize * caSize * caSize * 24;


	//set colours
	GLfloat* colours = new GLfloat[vertexNum];

	for(int colourID = 0; colourID < vertexNum / 3; colourID++) {
		
		int z = colourID / (caSize2 * caSize2);
		int y = (colourID - z) / caSize2;
		int x = colourID % caSize2;

		//Blue/green sliding colours based on position of x/z
		float r = ((float)1.0 / caSize) * x;
		float g = ((float)1.0 / caSize) * y;
		float b = ((float)1.0 / caSize) * z;

		colours[colourID * 3] = r;
		colours[colourID * 3 + 1] = g;
		colours[colourID * 3 + 2] = b;

	}
	
	//set normals
	GLfloat* normals = new GLfloat[vertexNum];

	for(int normalsID = 0; normalsID < vertexNum / 3; normalsID++) {
		
		normals[normalsID * 3] = 0;
		normals[normalsID * 3 + 1] = 1; 
		normals[normalsID * 3 + 2] = 0;

	}

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
	IBO->setUsagePattern(QGLBuffer::StreamDraw);

	//VBO->allocate(vertexList,sizeof(GLfloat) * vertexNum);

	VBO->allocate(0,sizeof(GLfloat) * vertexNum * 2);

	VBO->write(0,vertexList,sizeof(GLfloat) * vertexNum);
	VBO->write(sizeof(GLfloat) * vertexNum, colours, sizeof(GLfloat) * vertexNum);


	IBO->allocate(indexList,sizeof(GLuint) * indexNum);

	qDebug("%d",VBO->bufferId());
	qDebug("%d",IBO->bufferId());

	if (VBO->size() != sizeof(GLfloat) * vertexNum) {
		qWarning("VBO datasizes do not match");
	}

	delete[] colours;
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
