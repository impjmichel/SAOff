#pragma once

#include <string>
#include <vector>

#include <windows.h>
#include <gl/GL.h>

#include <map>
#include <string>

#define PI 3.14159265358979323846
#define TO_RADIANS(X) X / 180. * PI

class Texture;
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;

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

	class ObjModelCore
	{
	public:
		std::vector<ObjGroup*> groups;
		std::vector<MaterialInfo*> materials;
		GLuint _vertexArray;
	};

	btRigidBody *rigidBody;
	btCollisionShape *groundShape;
	btDefaultMotionState *myMotionState;
	ObjModelCore *objModelCore;


	void loadMaterialFile(std::string fileName, std::string dirName);
//public:
	ObjModel(std::string filename);
	void ObjModel::createRigidBody();
	~ObjModel(void);

	void draw(unsigned int shaderID);
};