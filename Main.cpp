#include "Main.h"
#include "MyApplication.h"

#include <VrLib/Kernel.h>
#include <cstdlib>

btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* world;

void main(int argc, char* argv[])
{
	Kernel* kernel = Kernel::getInstance();
	MyApplication* application = new MyApplication();

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--config") == 0)
		{
			i++;
			kernel->loadConfig(argv[i]);
		}
	}
	//Should place this elsewhere later
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0, -9.81f, 0));
	//Untill here

	kernel->setApp(application);
	kernel->start();
}