#include "Level.h"
#include "RenderManager.h"
#include "Floor.h"
#include "Cube.h"
#include "Camera.h"
#include "Skybox.h"
#include <VrLib\Application.h>
#include <ctime>
#include <btBulletDynamicsCommon.h>
#include <GL/glew.h>

bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
{
	if (((btRigidBody*)body0)->getUserPointer() == &GameManager::getInstance()->level->f)
	{
		btVector3 velocity = ((btRigidBody*)body1)->getLinearVelocity();
		((btRigidBody*)body1)->setLinearVelocity(btVector3(velocity.x(), 20, velocity.z()));
	}
	else if (((btRigidBody*)body1)->getUserPointer() == &GameManager::getInstance()->level->f)
	{
		btVector3 velocity = ((btRigidBody*)body0)->getLinearVelocity();
		((btRigidBody*)body0)->setLinearVelocity(btVector3(velocity.x(), 20, velocity.z()));
	}

	return false;
}

Level::Level()
{
	head = new PositionalDevice();
	WKey = new DigitalDevice();
	AKey = new DigitalDevice();
	SKey = new DigitalDevice();
	DKey = new DigitalDevice();
	QKey = new DigitalDevice();
	EKey = new DigitalDevice();
	UpKey = new DigitalDevice();
	DownKey = new DigitalDevice();
	LeftKey = new DigitalDevice();
	RightKey = new DigitalDevice();

	head->init("MainUserHead");
	WKey->init("WKey");
	AKey->init("AKey");
	SKey->init("SKey");
	DKey->init("DKey");
	UpKey->init("UpKey");
	QKey->init("QKey");
	EKey->init("EKey");
	DownKey->init("DownKey");
	LeftKey->init("LeftKey");
	RightKey->init("RightKey");

	skybox = new Skybox();

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0, -9.81f, 0));

	//Creating a static shape which will act as ground	
	{
		f = new Floor();
		btCollisionShape* groundShape = new btBoxShape(btVector3(FLOOR_WIDTH / 2., 0, FLOOR_HEIGHT / 2.));

		btScalar mass = 0; //rigidbody is static if mass is zero, otherwise dynamic
		btVector3 localInertia(0, 0, 0);

		groundShape->calculateLocalInertia(mass, localInertia);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform); //motionstate provides interpolation capabilities, and only synchronizes 'active' objects
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		body->setUserPointer(&f);
		//	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		world->addRigidBody(body); //add the body to the dynamics world
		gContactProcessedCallback = &contactProcessedCallback;
	}
}

Level::~Level()
{
	delete head;
	delete WKey;
	delete AKey;
	delete SKey;
	delete QKey;
	delete EKey;
	delete UpKey;
	delete DownKey;
	delete LeftKey;
	delete RightKey;

	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	delete f;
}

void Level::update()
{
	//Keyboard
	if (WKey->isInitialized() && (WKey->getData() == DigitalState::ON || WKey->getData() == DigitalState::TOGGLE_ON))
	{
		MoveForward();
	}
	if (AKey->isInitialized() && (AKey->getData() == DigitalState::ON || AKey->getData() == DigitalState::TOGGLE_ON))
	{
		MoveLeft();
	}
	if (DKey->isInitialized() && (DKey->getData() == DigitalState::ON || DKey->getData() == DigitalState::TOGGLE_ON))
	{
		MoveRight();
	}
	if (SKey->isInitialized() && (SKey->getData() == DigitalState::ON || SKey->getData() == DigitalState::TOGGLE_ON))
	{
		MoveBackward();
		//for each (Cube cube in cubeList)
		//{
		//	world->removeRigidBody(cube.body);
		//}
		//cubeList.clear();
	}
	if (QKey->isInitialized() && (QKey->getData() == DigitalState::ON || QKey->getData() == DigitalState::TOGGLE_ON))
	{
		MoveUpward();
	}
	if (EKey->isInitialized() && (EKey->getData() == DigitalState::ON || EKey->getData() == DigitalState::TOGGLE_ON))
	{
		MoveDownward();
	}
	if (UpKey->isInitialized() && (UpKey->getData() == DigitalState::ON || UpKey->getData() == DigitalState::TOGGLE_ON))
	{
		RotateUp();
	}
	if (DownKey->isInitialized() && (DownKey->getData() == DigitalState::ON || DownKey->getData() == DigitalState::TOGGLE_ON))
	{
		RotateDown();
	}
	if (LeftKey->isInitialized() && (LeftKey->getData() == DigitalState::ON || LeftKey->getData() == DigitalState::TOGGLE_ON))
	{
		RotateLeft();
	}
	if (RightKey->isInitialized() && (RightKey->getData() == DigitalState::ON || RightKey->getData() == DigitalState::TOGGLE_ON))
	{
		RotateRight();
	}
	//End of keyboard

	srand(time(NULL));

	if ((cubeList.size() < 100))
	{
		btCollisionShape* colShape = new btBoxShape(btVector3(5, 5, 5));
		btVector3 inertia;
		colShape->calculateLocalInertia(btScalar(CUBE_MASS), inertia);
		btTransform startTransform;

		while (cubeList.size() < 100)
		{
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(0, cubeList.size() < 50 ? 50 : 100, 20));
			Cube *cube = new Cube(0, cubeList.size() < 50 ? 50 : 100, 20, (rand() % 4) - 2, (rand() % 4) - 2, (rand() % 4) - 2, 5, 5, 5, true, true);
			while (cube->xVelocity == 0 && cube->yVelocity == 0 && cube->zVelocity == 0)
			{
				cube->xVelocity = (rand() % 4);
				cube->yVelocity = (rand() % 4);
				cube->zVelocity = (rand() % 4);
			}

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo cInfo(CUBE_MASS, myMotionState, colShape, inertia);

			btRigidBody* body = new btRigidBody(cInfo);
			body->setFriction(0.1);
			body->setLinearFactor(btVector3(1, 1, 1));
			body->setLinearVelocity(btVector3(cube->xVelocity, cube->yVelocity, cube->zVelocity));
			world->addRigidBody(body);
			cube->setBody(body);
			cubeList.push_back(cube);
		}
	}

	std::vector<Cube *>::iterator iter;
	for (iter = cubeList.begin(); iter != cubeList.end();) {
		(*iter)->update();

		//if ((*iter).isOutOfBounds(-50, 50, -50, 50, -50, 50))
		//	iter = cubeList.erase(iter);
		//else
		++iter;
	}
}

void Level::update(double frameTime, double totalTime)
{
	world->stepSimulation(frameTime / 1000.);
	update();
}




void Level::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float LightPosition[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glEnable(GL_MULTISAMPLE_ARB);
	glPushMatrix();
	InitCameraRotation();
	skybox->draw();
	InitCameraTranslation();

	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	for each (Cube *cube in cubeList)
	{
		cube->draw();
	}

	f->draw();
	glPopMatrix();
	glDisable(GL_MULTISAMPLE_ARB);
}