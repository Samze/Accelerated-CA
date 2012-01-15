#ifndef GL3DDRAWER_H
#define GL3DDRAWER_H

#include "gldrawer.h"

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
};

#endif // GL3DDRAWER_H
