#ifndef GL2DDRAWER_H
#define GL2DDRAWER_H

#include "gldrawer.h"

class GL2DDrawer : public GLDrawer
{
	Q_OBJECT

public:
	GL2DDrawer(QWidget *parent);
	~GL2DDrawer();
	
	virtual void drawCell(CellPos pos, float cellSpace,int state);

private:
	
};

#endif // GL2DDRAWER_H
