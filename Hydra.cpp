#include "Hydra.h"
#include "Camera.h"
#include <VrLib\Application.h>
#include <VrLib\Device.h>
#include <GL/glew.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "ObjModel.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

void Hydra::init()
{
	shaderID = initShader("simple");

	hydraRightPosition.init("RightNunchuck");
	hydraLeftPosition.init("LeftNunchuck");
	hydraRightJoystick.init("RightJoystick");
	hydraLeftJoystick.init("LeftJoystick");

	hydraLeftTrigger.init("LeftTrigger");
	hydraLeftBumper.init("LeftBumper");

	initHydraModels();
}

void Hydra::draw(float InitialModelView[16])
{
	if (!hydraEnabled)
	{
		return;
	}

	glPushMatrix();
	btScalar m[16];
	btTransform trans;

	//Right Hydra
	//NOTE: This part is currently very dependend of the direction the hydra-globe is pointing, be sure to make the razer logo point towards you.
	//Hydra draw
	{
		glUseProgram(shaderID);
		glPushMatrix();
		glLoadMatrixf(InitialModelView);
		rightModel->rigidBody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(m);
		glTranslatef(hydraRightPositionVector[0], hydraRightPositionVector[1] - 1, -2);// hydraRightPositionVector[2]);

		glm::mat4 old = hydraRightPosition.getData();
		glm::mat4 r(old[0].x, old[0].y, old[0].z, old[0].w,
					old[2].x, old[2].y, old[2].z, old[1].w,
					old[1].x, old[1].y, old[1].z, old[2].w,
					old[3].x, old[3].y, old[3].z, old[3].w);

		glMultMatrixf(glm::value_ptr(r));
		glRotatef(180, 0, 0, 1);
		glScalef(0.0018, 0.0018, 0.0018);

		rightModel->draw(shaderID);

		glPopMatrix();
		glUseProgram(0);
	}
	//End of hydra draw

	//Left Hydra
	//Hydra draw
	{
		glUseProgram(shaderID);
		glPushMatrix();
		glLoadMatrixf(InitialModelView);
		leftModel->rigidBody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(m);
		glTranslatef(hydraLeftPositionVector[0], hydraLeftPositionVector[1] - 1, -2);// hydraLeftPositionVector[2]);

		glm::mat4 old = hydraLeftPosition.getData();
		glm::mat4 r(old[0].x, old[0].y, old[0].z, old[0].w,
					old[2].x, old[2].y, old[2].z, old[1].w,
					old[1].x, old[1].y, old[1].z, old[2].w,
					old[3].x, old[3].y, old[3].z, old[3].w);

		glMultMatrixf(glm::value_ptr(r));
		glRotatef(180, 0, 0, 1);
		glScalef(0.0018, 0.0018, 0.0018);

		leftModel->draw(shaderID);

		glPopMatrix();
		glUseProgram(0);
	}
	//End of hydra draw

	glPopMatrix();
}

void Hydra::update()
{
	if (hydraRightJoystick.isInitialized())
	{
		glm::mat4 hydraMatrix = hydraRightPosition.getData();
		hydraRightPositionVector = hydraMatrix * glm::vec4(0, 0, 0, 1);
		hydraRightOrientation = glm::normalize((hydraMatrix * glm::vec4(0, 0, -1, 1)) - hydraRightPositionVector);

		glm::vec2 joystickData = hydraRightJoystick.getData();
		if (!((joystickData[0] <= -900) && (joystickData[1] <= -900)))
		{
			hydraEnabled = true;
			if (joystickData[0] - 0.1 > 0)
				RotateRight();
			else if (joystickData[0] + 0.1 < 0)
				RotateLeft();

			if (joystickData[1] - 0.1 > 0)
				RotateUp();
			else if (joystickData[1] + 0.1 < 0)
				RotateDown();
		}
	}
	if (hydraLeftJoystick.isInitialized())
	{
		glm::mat4 hydraMatrix = hydraLeftPosition.getData();
		hydraLeftPositionVector = hydraMatrix * glm::vec4(0, 0, 0, 1);
		hydraLeftOrientation = glm::normalize((hydraMatrix * glm::vec4(0, 0, -1, 1)) - hydraLeftPositionVector);

		glm::vec2 joystickData = hydraLeftJoystick.getData();
		if (!((joystickData[0] <= -900) && (joystickData[1] <= -900)))
		{
			hydraEnabled = true;
			if (joystickData[0] - 0.1 > 0)
				MoveRight();
			else if (joystickData[0] + 0.1 < 0)
				MoveLeft();

			if (joystickData[1] - 0.1 > 0)
				MoveForward();
			else if (joystickData[1] + 0.1 < 0)
				MoveBackward();
		}
	}
	if (hydraLeftBumper.isInitialized())
	{
		if (hydraLeftBumper.getData() == DigitalState::ON || hydraLeftBumper.getData() == DigitalState::TOGGLE_ON)
			MoveUpward();
	}
	if (hydraLeftTrigger.isInitialized())
	{
		if (hydraLeftTrigger.getData() - 0.1 > 0)
			MoveDownward();
	}
}

void Hydra::initHydraModels()
{
	rightModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\Sword01\\rusword.obj");
	leftModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\Sword02\\rusword.obj");
}
