#include <glm\matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <VrLib\Application.h>
#include <GL\glew.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <cmath>

#include "MyApplication.h"
#include "Main.h"
#include "Cube.h"
#include "Camera.h"
#include "Floor.h"

std::vector<Cube> cubeList;	
Floor f;

bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
{
	if (((btRigidBody*)body0)->getUserPointer() == &f)
	{
		btVector3 velocity = ((btRigidBody*)body1)->getLinearVelocity();
		((btRigidBody*)body1)->setLinearVelocity(btVector3(velocity.x(), 20, velocity.z()));
	}
	else if (((btRigidBody*)body1)->getUserPointer() == &f)
	{
		btVector3 velocity = ((btRigidBody*)body0)->getLinearVelocity();
		((btRigidBody*)body0)->setLinearVelocity(btVector3(velocity.x(), 20, velocity.z()));
	}

	return false;
}

void MyApplication::init()
{
	head.init("MainUserHead");

	WKey.init("WKey");
	AKey.init("AKey");
	SKey.init("SKey");
	DKey.init("DKey");
	UpKey.init("UpKey");
	QKey.init("QKey");
	EKey.init("EKey");
	DownKey.init("DownKey");
	LeftKey.init("LeftKey");
	RightKey.init("RightKey");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glClearColor(0, 0, 0, 0);

	//Creating a static shape which will act as ground
	{
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
		gContactProcessedCallback = contactProcessedCallback;
	}
}

void MyApplication::initLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_NORMALIZE);
	const float amb = 0.5;
	const float LightAmbient[4] = { amb, amb, amb, 1.0f };
	const float LightDiffuse[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const float whiteSpecularLight[] = { 1.0, 1.0, 1.0 };
	const GLfloat shiny = 60.0;
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_SHININESS, &shiny);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
}

void MyApplication::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	initLighting();
	glEnable(GL_MULTISAMPLE_ARB);
	const float LightPosition[4] = { 0.0f, 1.0f, 0.0f, 0.0f };

	glPushMatrix();
	InitCameraRotation();
	InitCameraTranslation();

	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);


	for each (Cube cube in cubeList)
	{
		cube.draw();
	}

	f.draw();

	glPopMatrix();
	glFlush();
	glFinish();
	glDisable(GL_MULTISAMPLE_ARB);
}

void MyApplication::preFrame(double frameTime, double totalTime)
{
	world->stepSimulation(frameTime / 1000.);
	preFrame();
}

void MyApplication::preFrame()
{
	//Keyboard
	if (WKey.isInitialized() && (WKey.getData() == DigitalState::ON || WKey.getData() == DigitalState::TOGGLE_ON))
	{
		MoveForward();
	}
	if (AKey.isInitialized() && (AKey.getData() == DigitalState::ON || AKey.getData() == DigitalState::TOGGLE_ON))
	{
		MoveLeft();
	}
	if (DKey.isInitialized() && (DKey.getData() == DigitalState::ON || DKey.getData() == DigitalState::TOGGLE_ON))
	{
		MoveRight();
	}
	if (SKey.isInitialized() && (SKey.getData() == DigitalState::ON || SKey.getData() == DigitalState::TOGGLE_ON))
	{
		MoveBackward();
	}
	if (QKey.isInitialized() && (QKey.getData() == DigitalState::ON || QKey.getData() == DigitalState::TOGGLE_ON))
	{
		MoveUpward();
	}
	if (EKey.isInitialized() && (EKey.getData() == DigitalState::ON || EKey.getData() == DigitalState::TOGGLE_ON))
	{
		MoveDownward();
	}
	if (UpKey.isInitialized() && (UpKey.getData() == DigitalState::ON || UpKey.getData() == DigitalState::TOGGLE_ON))
	{
		RotateUp();
	}
	if (DownKey.isInitialized() && (DownKey.getData() == DigitalState::ON || DownKey.getData() == DigitalState::TOGGLE_ON))
	{
		RotateDown();
	}
	if (LeftKey.isInitialized() && (LeftKey.getData() == DigitalState::ON || LeftKey.getData() == DigitalState::TOGGLE_ON))
	{
		RotateLeft();
	}
	if (RightKey.isInitialized() && (RightKey.getData() == DigitalState::ON || RightKey.getData() == DigitalState::TOGGLE_ON))
	{
		RotateRight();
	}
	//End of keyboard

	srand(time(NULL));

	if (cubeList.size() < 100)
	{
		btCollisionShape* colShape = new btBoxShape(btVector3(5, 5, 5));
		btVector3 inertia;
		colShape->calculateLocalInertia(btScalar(CUBE_MASS), inertia);
		btTransform startTransform;

		while (cubeList.size() < 100)
		{
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(0, cubeList.size() < 50 ? 50 : 100, 20));
			Cube cube(0, cubeList.size() < 50 ? 50 : 100, 20, (rand() % 4) - 2, (rand() % 4) - 2, (rand() % 4) - 2, 5, 5, 5, true, true);
			while (cube.xVelocity == 0 && cube.yVelocity == 0 && cube.zVelocity == 0)
			{
				cube.xVelocity = (rand() % 4);
				cube.yVelocity = (rand() % 4);
				cube.zVelocity = (rand() % 4);
			}

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo cInfo(CUBE_MASS, myMotionState, colShape, inertia);

			btRigidBody* body = new btRigidBody(cInfo);
			body->setFriction(0.1);
			body->setLinearFactor(btVector3(1, 1, 1));
			body->setLinearVelocity(btVector3(cube.xVelocity, cube.yVelocity, cube.zVelocity));
			world->addRigidBody(body);
			cube.setBody(body);
			cubeList.push_back(cube);
		}
	}

	std::vector<Cube>::iterator iter;
	for (iter = cubeList.begin(); iter != cubeList.end();) {
		(*iter).update();

		//if ((*iter).isOutOfBounds(-50, 50, -50, 50, -50, 50))
		//	iter = cubeList.erase(iter);
		//else
			++iter;
	}
}

void MyApplication::latePreFrame()
{

}

void MyApplication::stop()
{

}