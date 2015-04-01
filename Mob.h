#ifndef MOB_H
#define MOB_H

#include "GameObject.h"

#define PI 3.14159265358979323846
//#define TO_RADIANS(X) X / 180. * PI
#define TO_EULER(X) X / PI * 180.

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
	float health = 100;
	float maxHealth = 100;
	float speed = 8;
};

#endif