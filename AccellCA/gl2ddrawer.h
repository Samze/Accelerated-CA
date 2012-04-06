#ifndef GL2DDRAWER_H
#define GL2DDRAWER_H

#include "gldrawer.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

class GL2DDrawer : public GLDrawer
{
	Q_OBJECT

public:
	GL2DDrawer();
	GL2DDrawer(QWidget *parent);
	~GL2DDrawer();
	
	virtual void drawCell(CellPos pos, float cellSpace,int state);

private:
	
};

#endif // GL2DDRAWER_H
