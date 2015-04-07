#ifndef MOB_H
#define MOB_H

#include "GameObject.h"
#include <btBulletCollisionCommon.h>

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
	void init(const btVector3 &origin);
	void draw();
	void update();
	float health = 100;
	float maxHealth = 100;
	float speed = 8;
	long lastHitPlayer = 0;
private:
	~Mob();
};

#endif