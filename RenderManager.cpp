#include <GL/glew.h>
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Level.h"
#include "PostProcessing.h"
#include "GaussianBlur.h"

RenderManager::RenderManager()
{
	postProcessor = new GaussianBlur();
	postProcessor->init();
}

RenderManager::~RenderManager()
{
	delete postProcessor;
}

void RenderManager::draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_CULL_FACE); //Ouchta set normals for this.
	//glCullFace(GL_BACK);
	glClearColor(0, 0, 0, 0);

	postProcessor->draw();
}