#ifndef HYDRA_H
#define HYDRA_H

#define SWORD_Y_OFFSET -1.0 //Used in order to align the hitbox and the drawn model.
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
	bool initRigidbodies = false;
	ObjModel *leftModel;
	ObjModel *rightModel;

private:
	unsigned int shaderID;
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

	glm::mat4 getWorldMatrixFromHydra(glm::mat4 old);
	void initHydraModels();
};

#endif