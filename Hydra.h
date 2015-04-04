#ifndef HYDRA_H
#define HYDRA_H

#define SWORD_Y_OFFSET -1.0
#define TO_RADIANS(X) X / 180. * PI

#include <glm\glm.hpp>
#include <VrLib/Device.h>

class ObjModel;

class Hydra
{
public:
	void Hydra::draw(float InitialModelView[16]);
	void update();
	void init();

private:
	unsigned int shaderID;
	PositionalDevice hydraRightPosition;
	PositionalDevice hydraLeftPosition;
	TwoDimensionDevice hydraRightJoystick;
	TwoDimensionDevice hydraLeftJoystick;
	AnalogDevice hydraLeftTrigger;
	DigitalDevice hydraLeftBumper;
	ObjModel *leftModel;
	ObjModel *rightModel;

	glm::vec4 hydraRightPositionVector;
	glm::vec4 hydraRightOrientation;
	glm::vec4 hydraLeftPositionVector;
	glm::vec4 hydraLeftOrientation;
	bool hydraEnabled = false;
	bool initRigidbodies = false;

	glm::mat4 getWorldMatrixFromHydra(glm::mat4 old);
	void initHydraModels();
};

#endif