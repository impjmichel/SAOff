#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

#include <VrLib\Application.h>

class RenderManager;
class Level;

class GameManager : public Application
{
public:
	RenderManager *renderManager;
	Level *level;
	float fpTime = 0;

	static GameManager* getInstance();
	void init();
	void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
	void preFrame(double frameTime, double totalTime);
	
	void latePreFrame();
	
	
	void stop();
};

#endif