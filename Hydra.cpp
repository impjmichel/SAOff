#include "Hydra.h"
#include "Camera.h"
#include <VrLib\Application.h>
#include <VrLib\Device.h>
#include <GL/glew.h>

void Hydra::init()
{
	hydraRightPosition.init("RightNunchuck");
	hydraLeftPosition.init("LeftNunchuck");
	hydraRightJoystick.init("RightJoystick");
	hydraLeftJoystick.init("LeftJoystick");

	hydraLeftTrigger.init("LeftTrigger");
	hydraLeftBumper.init("LeftBumper");
}

void Hydra::draw(float InitialModelView[16])
{
	//Right Hydra
	if (!hydraEnabled)
	{
		return;
	}

	glPushMatrix();

	//Right Hydra

	//NOTE: This part is currently very dependend of the direction the hydra-globe is pointing, be sure to make the razer logo point towards you.

	//Hydra draw
	glPushMatrix();
	glLoadMatrixf(InitialModelView);
	glTranslatef(hydraRightPositionVector[0], hydraRightPositionVector[1] - 2, -2);// hydraRightPositionVector[2]);
	glLineWidth(5);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0.50f, 0.0f, 0.0f);
	glVertex3f(hydraRightOrientation[0] * 3 + 0.50, hydraRightOrientation[1] * 3, -hydraRightOrientation[3] * 3);
	glEnd();
	glPopMatrix();
	//End of hydra draw=

	//Left Hydra

	//Hydra draw
	glPushMatrix();
	glLoadMatrixf(InitialModelView);
	glTranslatef(hydraLeftPositionVector[0], hydraLeftPositionVector[1] - 2, -2); //hydraLeftPositionVector[2]);
	glLineWidth(5);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(-0.50f, 0.0f, 0.0f);
	glVertex3f(hydraLeftOrientation[0] * 3 - 0.50, hydraLeftOrientation[1] * 3, -hydraLeftOrientation[3] * 3);
	glEnd();
	glPopMatrix();
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