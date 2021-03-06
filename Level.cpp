#include "Level.h"
#include "RenderManager.h"
#include "Floor.h"
#include "Cube.h"
#include "Camera.h"
#include "Skybox.h"
#include "Terrain.h"
#include <VrLib\Application.h>
#include <ctime>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <GL/glew.h>
#include <iostream>
#include "Shader.h"
#include "Mob.h"
#include "ObjModel.h"
#include <glm/gtc/type_ptr.hpp>
#include "Hydra.h"
#include "debug.h"
#include "CameraCharacter.h"
#include "Main.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sys/timeb.h>
#include "stb_image.h"

CollisionInformation leftHydraCollisionInformation;
CollisionInformation rightHydraCollisionInformation;

bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
{
	btRigidBody *rBody0 = (btRigidBody*)body0;
	btRigidBody *rBody1 = (btRigidBody*)body1;

	if (!GameManager::getInstance()->level->hydra->initRigidbodies)
		return false;

	//check if mob
	Mob* foundMob = 0;
	bool found = false;
	for each (Mob *m in GameManager::getInstance()->level->mobs)
	{
		if (rBody0->getUserPointer() == m->pObjModel || rBody1->getUserPointer() == m->pObjModel)
		{
			found = true;
			foundMob = m;
			break;
		}
	}
	if (!found) return false;

	GLint64 timer;
	if (rBody0->getUserPointer() == GameManager::getInstance()->level->hydra->leftModel || rBody1->getUserPointer() == GameManager::getInstance()->level->hydra->leftModel)
	{
		switch (leftHydraCollisionInformation.m_state)
		{
		case CollisionState::NO_COLLISION:
			glGetInteger64v(GL_TIMESTAMP, &timer);
			timer /= 10000000.0f;
			leftHydraCollisionInformation = CollisionInformation(CollisionState::COLLISION, timer, foundMob);
			break;
		}
	}

	if (rBody0->getUserPointer() == GameManager::getInstance()->level->hydra->rightModel || rBody1->getUserPointer() == GameManager::getInstance()->level->hydra->rightModel)
	{
		switch (rightHydraCollisionInformation.m_state)
		{
		case CollisionState::NO_COLLISION:
			glGetInteger64v(GL_TIMESTAMP, &timer);
			timer /= 10000000.0f;
			rightHydraCollisionInformation = CollisionInformation(CollisionState::COLLISION, timer, foundMob);
			break;
		}
	}

	if (rBody0->getUserPointer() == GameManager::getInstance()->level->cameraCharacter || rBody1->getUserPointer() == GameManager::getInstance()->level->cameraCharacter)
	{
		glGetInteger64v(GL_TIMESTAMP, &timer);
		timer /= 10000000.0f;
		if (foundMob->lastHitPlayer + 50 <= timer)
		{
			GameManager::getInstance()->level->health -= 10;
			if (GameManager::getInstance()->level->health <= 0)
				GameManager::getInstance()->level->gameOver = true;
			foundMob->lastHitPlayer = timer;
		}
	}

	return false;
}

Level::Level()
{
	// temporary stuff ( Terrain generator )
	g_Terrain = new Terrain(1,1);
	g_Terrain->init_FlatGrassLand();
	if (!g_Terrain->LoadHeightmap(65, 65))
	{
		std::cerr << "Failed to load heightmap for terrain!" << std::endl;
	}
	if (!g_Terrain->LoadTexture("Data/Textures/grass.jpg", 0))
	{
		std::cerr << "Failed to load terrain texture for texture stage 0!" << std::endl;
	}
	if (!g_Terrain->LoadTexture("Data/Textures/rock.png", 1))
	{
		std::cerr << "Failed to load terrain texture for texture stage 1!" << std::endl;
	}
	if (!g_Terrain->LoadTexture("Data/Textures/snow.jpg", 2))
	{
		std::cerr << "Failed to load terrain texture for texture stage 2!" << std::endl;
	}

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
	hydra = new Hydra();
	hydra->init();

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0.0f, -9.81f * 20.0f, 0.0f));

#ifdef BULLET_DEBUG_DRAW
	debug* debug_1 = new debug();
	world->setDebugDrawer(debug_1);
#endif

	cameraCharacter = new CameraCharacter();
	cameraCharacter->init();
	world->addRigidBody(cameraCharacter->rigidBody);

	//Creating a static shape which will act as ground	
	{
		//f = new Floor();
		btTriangleMesh *tMesh = new btTriangleMesh();

		Terrain::IndexBuffer m_IndexBuffer = g_Terrain->m_IndexBuffer;
		Terrain::PositionBuffer m_PositionBuffer = g_Terrain->m_PositionBuffer;
		for (unsigned long int i = 0; i < m_IndexBuffer.size(); i += 3)
		{
			glm::vec3 t01 = ((glm::vec3)m_PositionBuffer[m_IndexBuffer[i + 0]]);
			btVector3 t1 = btVector3(t01.x, (t01.y * 5.), t01.z);

			glm::vec3 t02 = ((glm::vec3)m_PositionBuffer[m_IndexBuffer[i + 1]]);
			btVector3 t2 = btVector3(t02.x, (t02.y * 5.), t02.z);

			glm::vec3 t03 = ((glm::vec3)m_PositionBuffer[m_IndexBuffer[i + 2]]);
			btVector3 t3 = btVector3(t03.x, (t03.y * 5.), t03.z);
			
			tMesh->addTriangle(t1, t2, t3);
		}
		btCollisionShape *groundShape = new btBvhTriangleMeshShape(tMesh, true);
		//btCollisionShape* groundShape = new btBoxShape(btVector3(FLOOR_WIDTH / 2., 0, FLOOR_HEIGHT / 2.));

		btScalar mass = 0; //rigidbody is static if mass is zero, otherwise dynamic
		btVector3 localInertia(0, 0, 0);

		//groundShape->calculateLocalInertia(mass, localInertia);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform); //motionstate provides interpolation capabilities, and only synchronizes 'active' objects
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		terrainBody = new btRigidBody(rbInfo);
		//terrainBody->setUserPointer(terrainBody);
		//	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		world->addRigidBody(terrainBody); //add the body to the dynamics world
	}

	srand(time(NULL));

	{
		for (int i = 0; i < 10; i++)
		{
			Mob *mob = new Mob();
			mob->init(btVector3(rand() % 100 - 50, 2, rand() % 100 - 50));
			world->addRigidBody(mob->pObjModel->rigidBody);
			mobs.push_back(mob);
		}
		//world->add
	}

	gContactProcessedCallback = &contactProcessedCallback;
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
	if (gameOver)
		return;

	GLint64 timer;
	glGetInteger64v(GL_TIMESTAMP, &timer);
	timer /= 10000000.0f;

	if (leftHydraCollisionInformation.m_state == CollisionState::COLLISION && leftHydraCollisionInformation.m_collisionTime + 100 <= timer)
	{
		leftHydraCollisionInformation.m_state = CollisionState::NO_COLLISION;
	}
	else if (leftHydraCollisionInformation.m_state == CollisionState::COLLISION)
	{
		if ((!leftHydraCollisionInformation.m_damageCalculated) && leftHydraCollisionInformation.m_hitMob != 0)
		{
			leftHydraCollisionInformation.m_hitMob->health -= 20;
			if (leftHydraCollisionInformation.m_hitMob->health <= 0) {
				mobs.erase(std::remove(mobs.begin(), mobs.end(), leftHydraCollisionInformation.m_hitMob), mobs.end());
				leftHydraCollisionInformation.m_hitMob = 0;
			}
			leftHydraCollisionInformation.m_damageCalculated = true;
		}
	}

	if (rightHydraCollisionInformation.m_state == CollisionState::COLLISION && rightHydraCollisionInformation.m_collisionTime + 100 <= timer)
	{
		rightHydraCollisionInformation.m_state = CollisionState::NO_COLLISION;
	}
	else if (rightHydraCollisionInformation.m_state == CollisionState::COLLISION)
	{
		if ((!rightHydraCollisionInformation.m_damageCalculated) && rightHydraCollisionInformation.m_hitMob != 0)
		{
			rightHydraCollisionInformation.m_hitMob->health -= 20;
			if (rightHydraCollisionInformation.m_hitMob->health <= 0) {
				mobs.erase(std::remove(mobs.begin(), mobs.end(), rightHydraCollisionInformation.m_hitMob), mobs.end());
				rightHydraCollisionInformation.m_hitMob = 0;
			}
			rightHydraCollisionInformation.m_damageCalculated = true;
		}
	}

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

	hydra->update();

	cameraCharacter->update();

	//srand(time(NULL));

	//if ((cubeList.size() < 0))
	//{
	//	btCollisionShape* colShape = new btBoxShape(btVector3(5, 5, 5));
	//	btVector3 inertia;
	//	colShape->calculateLocalInertia(btScalar(CUBE_MASS), inertia);
	//	btTransform startTransform;

	//	while (cubeList.size() < 100)
	//	{
	//		startTransform.setIdentity();
	//		startTransform.setOrigin(btVector3(0, cubeList.size() < 50 ? 50 : 100, 20));
	//		Cube *cube = new Cube(0, cubeList.size() < 50 ? 50 : 100, 20, (rand() % 4) - 2, (rand() % 4) - 2, (rand() % 4) - 2, 5, 5, 5, true, true);
	//		while (cube->xVelocity == 0 && cube->yVelocity == 0 && cube->zVelocity == 0)
	//		{
	//			cube->xVelocity = (rand() % 4);
	//			cube->yVelocity = (rand() % 4);
	//			cube->zVelocity = (rand() % 4);
	//		}

	//		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//		btRigidBody::btRigidBodyConstructionInfo cInfo(CUBE_MASS, myMotionState, colShape, inertia);

	//		btRigidBody* body = new btRigidBody(cInfo);
	//		body->setFriction(0.1);
	//		body->setLinearFactor(btVector3(1, 1, 1));
	//		body->setLinearVelocity(btVector3(cube->xVelocity, cube->yVelocity, cube->zVelocity));
	//		world->addRigidBody(body);
	//		cube->setBody(body);
	//		cubeList.push_back(cube);
	//	}
	//}

	//std::vector<Cube *>::iterator iter;
	//for (iter = cubeList.begin(); iter != cubeList.end();) {
	//	(*iter)->update();

	//	//if ((*iter).isOutOfBounds(-50, 50, -50, 50, -50, 50))
	//	//	iter = cubeList.erase(iter);
	//	//else
	//	++iter;
	//}

	std::vector<Mob *>::iterator iter2;
	for (iter2 = mobs.begin(); iter2 != mobs.end();) {
		(*iter2)->update();

		++iter2;
	}
}

void Level::update(double frameTime, double totalTime)
{
	world->stepSimulation(frameTime / 1000.);
	skybox->update(frameTime, totalTime);
	update();
}

void Level::draw(DrawMode drawMode)
{
	float InitialModelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &InitialModelView[0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float LightPosition[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glEnable(GL_MULTISAMPLE_ARB);
	glPushMatrix();
	InitCameraRotation();
	skybox->draw();
	InitCameraTranslation();

	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

#ifdef BULLET_DEBUG_DRAW
	world->debugDrawWorld();
#endif

	hydra->draw(InitialModelView);

	cameraCharacter->draw();

	for each (Cube *cube in cubeList)
	{
		cube->draw();
	}

	//glm::value_ptr(glm::mat4());
	glPushMatrix();
	btTransform trans;
	btScalar m[16];

	terrainBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);

	g_Terrain->Render();

	glPopMatrix();

	for each (Mob *mob in mobs)
	{
		mob->draw();
	}
	//f->draw();
	glPopMatrix();
	glDisable(GL_MULTISAMPLE_ARB);

	//Draw Health
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//Draw healthborders
	float healthPercentage = health / maxHealth;
#ifndef SIMULATION_MODE
	if (drawMode == DrawMode::LeftEye)
	{
		glColor4f(1 - healthPercentage, healthPercentage, 0, 0.5);
		glBegin(GL_QUADS);

		//Under
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, -0.8f);
		glVertex2f(-1.0f, -0.8f);

		//Upper
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 0.8f);
		glVertex2f(-1.0f, 0.8f);

		//Left
		glVertex2f(-0.8f, -0.8f);
		glVertex2f(-1.0f, -0.8f);
		glVertex2f(-1.0f, 0.8f);
		glVertex2f(-0.8f, 0.8f);
		glEnd();
	}
	else if (drawMode == DrawMode::RightEye)
	{
		glColor4f(1 - healthPercentage, healthPercentage, 0, 0.5);
		glBegin(GL_QUADS);

		//Under
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, -0.8f);
		glVertex2f(-1.0f, -0.8f);

		//Upper
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 0.8f);
		glVertex2f(-1.0f, 0.8f);

		//Right
		glVertex2f(0.8f, -0.8f);
		glVertex2f(1.0f, -0.8f);
		glVertex2f(1.0f, 0.8f);
		glVertex2f(0.8f, 0.8f);
		glEnd();
	}
	else if (drawMode == DrawMode::Simulation)
	{
#endif
		glColor4f(1 - healthPercentage, healthPercentage, 0, 0.5);
		glBegin(GL_QUADS);

		//Under
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, -0.8f);
		glVertex2f(-1.0f, -0.8f);

		//Upper
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 0.8f);
		glVertex2f(-1.0f, 0.8f);

		//Right
		glVertex2f(0.8f, -0.8f);
		glVertex2f(1.0f, -0.8f);
		glVertex2f(1.0f, 0.8f);
		glVertex2f(0.8f, 0.8f);

		//Left
		glVertex2f(-0.8f, -0.8f);
		glVertex2f(-1.0f, -0.8f);
		glVertex2f(-1.0f, 0.8f);
		glVertex2f(-0.8f, 0.8f);
		glEnd();
#ifndef SIMULATION_MODE
	}
#endif
	glPopMatrix();
	glPopMatrix();
}