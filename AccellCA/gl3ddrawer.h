#ifndef GL3DDRAWER_H
#define GL3DDRAWER_H

#include "gldrawer.h"
#include "qglbuffer.h"

class GL3DDrawer : public GLDrawer
{
	Q_OBJECT

public:
	GL3DDrawer(QWidget *parent);
	~GL3DDrawer();

	
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
};

#endif // GL3DDRAWER_H
