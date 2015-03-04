#pragma once
#include <VrLib\Device.h>
#include <VrLib\Application.h>

class MyApplication : public Application
{
	//Oculus
	PositionalDevice head;
	//End Oculus

	//Keyboard
	DigitalDevice WKey;
	DigitalDevice AKey;
	DigitalDevice SKey;
	DigitalDevice DKey;
	DigitalDevice QKey;
	DigitalDevice EKey;
	DigitalDevice UpKey;
	DigitalDevice DownKey;
	DigitalDevice LeftKey;
	DigitalDevice RightKey;
	//End Keyboard
public:
	virtual void init();
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
	virtual void preFrame(double frameTime, double totalTime);
	virtual void preFrame();
	virtual void initLighting();

	virtual void latePreFrame();


	virtual void stop();
};