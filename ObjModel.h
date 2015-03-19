#pragma once

#include <string>
#include <vector>

#include <windows.h>
#include <gl/GL.h>

class Texture;
class btRigidBody;
class btCollisionShape;
class btDefaultMotionState;

class ObjModel
{
public: //private:
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		unsigned int texture;
		std::string textureStr;
		unsigned int bumpMap;
		std::string bumpMapStr;

		bool hasTexture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int start;
		int end;
		int materialIndex;
	};

	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;
	GLuint _vertexArray;
	btRigidBody *rigidBody;
	btCollisionShape *groundShape;
	btDefaultMotionState *myMotionState;

	void loadMaterialFile(std::string fileName, std::string dirName);
//public:
	ObjModel(std::string filename);
	~ObjModel(void);

	void draw(unsigned int shaderID);
};

