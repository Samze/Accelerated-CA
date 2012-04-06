#ifndef GL3DDRAWER_H
#define GL3DDRAWER_H

#include "gldrawer.h"
#include "qglbuffer.h"
#include <QVector>
#include <qvector3d.h>
#include <QMatrix4x4>
#include <QList>
#include <QTime>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

class GL3DDrawer : public GLDrawer
{
	Q_OBJECT

public:
	GL3DDrawer();
	GL3DDrawer(QWidget *parent);
	~GL3DDrawer();

	struct VEC3 {
		float x;
		float y;
		float z;
	};
	
	virtual void drawCell(CellPos pos, float cellSpace,int state);
	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int w, int h);

private:
	void draw3DCell();
	void draw3DElements();
	void drawVBO();
	void createVBO();
	QGLBuffer* VBO; //= new QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer* IBO;// = new QGLBuffer(QGLBuffer::IndexBuffer);

	GLuint createDisplayList();
	GLuint id;
	GLuint createCombinedDisplayList();
	
	void createCombinedVBO();
	void drawWireFrameCube();
	
	void paintGLVBO();
	void populateIndexBuffer();
	void populateVertexList();

	//Size 24
	//QVector<GLuint>* getIndicesForCell(int x, int y, int z);

	GLfloat* vertexList;
	GLuint* indexList;

	bool setupVBO;

	int newIndiceNum;

};

#endif // GL3DDRAWER_H
