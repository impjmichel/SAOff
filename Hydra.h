#ifndef HYDRA_H
#define HYDRA_H

#include <glm\glm.hpp>
#include <VrLib/Device.h>

class Hydra
{
public:
	void Hydra::draw(float InitialModelView[16]);
	void update();
	void init();

private:
	PositionalDevice hydraRightPosition;
	PositionalDevice hydraLeftPosition;
	TwoDimensionDevice hydraRightJoystick;
	TwoDimensionDevice hydraLeftJoystick;
	AnalogDevice hydraLeftTrigger;
	DigitalDevice hydraLeftBumper;

	glm::vec4 hydraRightPositionVector;
	glm::vec4 hydraRightOrientation;
	glm::vec4 hydraLeftPositionVector;
	glm::vec4 hydraLeftOrientation;
	bool hydraEnabled = false;
};

#endif