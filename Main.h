#ifndef MAIN_H
#define MAIN_H

#ifdef _DEBUG
#pragma comment(lib, "Libs/Bullet3Common_vs2010_debug.lib")
#pragma comment(lib, "Libs/Bullet3Collision_vs2010_debug.lib")
#pragma comment(lib, "Libs/Bullet3Dynamics_vs2010_debug.lib")
#pragma comment(lib, "Libs/BulletCollision_vs2010_debug.lib")
#pragma comment(lib, "Libs/BulletDynamics_vs2010_debug.lib")
#pragma comment(lib, "Libs/Bullet3Geometry_vs2010_debug.lib")
#pragma comment(lib, "Libs/LinearMath_vs2010_debug.lib")
#else
#pragma comment(lib, "Libs/Bullet3Common_vs2010.lib")
#pragma comment(lib, "Libs/Bullet3Collision_vs2010.lib")
#pragma comment(lib, "Libs/Bullet3Dynamics_vs2010.lib")
#pragma comment(lib, "Libs/BulletCollision_vs2010.lib")
#pragma comment(lib, "Libs/BulletDynamics_vs2010.lib")
#pragma comment(lib, "Libs/Bullet3Geometry_vs2010.lib")
#pragma comment(lib, "Libs/LinearMath_vs2010.lib")
#endif

#include <btBulletDynamicsCommon.h>

extern btBroadphaseInterface* broadphase;
extern btDefaultCollisionConfiguration* collisionConfiguration;
extern btCollisionDispatcher* dispatcher;
extern btSequentialImpulseConstraintSolver* solver;
extern btDiscreteDynamicsWorld* world;

void main(int argc, char* argv[]);

#endif