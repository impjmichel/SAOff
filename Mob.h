#ifndef MOB_H
#define MOB_H

#include "GameObject.h"

class ObjModel;

class Mob : public GameObject
{
public:
	unsigned int shaderID;
	unsigned int vbo_triangle_colors;
	ObjModel *pObjModel;
	void init();
	void draw();
	void update();
};

#endif