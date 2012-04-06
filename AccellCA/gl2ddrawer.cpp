#include "gl2ddrawer.h"

GL2DDrawer::GL2DDrawer(){

}

GL2DDrawer::GL2DDrawer(QWidget *parent)
	: GLDrawer(parent)
{

}

GL2DDrawer::~GL2DDrawer()
{
	qDebug("B");
}

void GL2DDrawer::drawCell(CellPos pos, float cellSpace,int state) {

	int x = pos.x;
	int y = pos.y;

	float r = 0.2;
	float g = 0.2;
	float b = 0.2;

	Totalistic* totalistic = static_cast<Totalistic*>(CA->getCARule());

	int states = totalistic->getNoStates();

	//For SCIARA2
	//int states = CA->getDIM() * CA->getDIM();

	float colourValue = 1 - (((float)state / states) * 0.8) + 0.2;
	r = colourValue;

	
	float third = (float)states / 3;

	////float r = state < third ? state/third : 0;
	////float g = state < third && state < third * 2 ? state/third * 2: 0;
	////float b = state < third * 3? state/third * 3: 0;

	int stateRange = (state / third);

	int val = state - (stateRange * third);

	//float colourValue = 1 - ((float)val / third);
	
	//if(stateRange == 0) r = colourValue;
	//if(stateRange == 1) g = colourValue;
	//if(stateRange >= 2) b = colourValue;



	glColor3f(r,g,b);


	glLoadIdentity();
	glTranslatef(x* cellSpace,y * cellSpace,0.0f);
	glScalef(cellSpace - 1, cellSpace - 1, 0.0f); //minus an ammount here to get a "grid" look if desired
	//glScalef(cellSpace, cellSpace, 0.0f); //minus an ammount here to get a "grid" look if desired
	//glScalef(9.0, 9.0,0.0f);
	glBegin(GL_QUADS);
		glVertex2f(0,0); //top left
		glVertex2f(1,0); //top right
		glVertex2f(1,1); //bottom right
		glVertex2f(0,1); //bottom left
	glEnd();
}