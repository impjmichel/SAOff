#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <VrLib\Device.h>

class Cube;
class Floor;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class PositionalDevice;
class DigitalDevice;
class Skybox;
class Terrain;
class Mob;
class btRigidBody;
class Hydra;
class CameraCharacter;

enum CollisionState
{
	NO_COLLISION,
	COLLISION
};

struct CollisionInformation
{
	CollisionInformation(CollisionState state, long collisionTime, Mob* hitMob) : m_state(state), m_collisionTime(collisionTime), m_hitMob(hitMob) {}
	CollisionInformation(){}
	CollisionState m_state = CollisionState::NO_COLLISION;
	long m_collisionTime = LONG_MAX;
	bool m_damageCalculated = false;
	Mob *m_hitMob;
};

extern CollisionInformation leftHydraCollisionInformation;
extern CollisionInformation rightHydraCollisionInformation;

enum DrawMode
{
	RightEye,
	LeftEye,
	Simulation
};

class Level{
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	//oculus
	PositionalDevice *head;
	//End Oculus

	//Keyboard
	DigitalDevice *WKey;
	DigitalDevice *AKey;
	DigitalDevice *SKey;
	DigitalDevice *DKey;
	DigitalDevice *QKey;
	DigitalDevice *EKey;
	DigitalDevice *UpKey;
	DigitalDevice *DownKey;
	DigitalDevice *LeftKey;
	DigitalDevice *RightKey;
	//End Keyboard

public:
	Terrain *g_Terrain;// (50, 50);
	std::vector<Cube *> cubeList;
	Floor *f;
	Skybox *skybox;
	std::vector<Mob *> mobs; 
	btRigidBody* terrainBody;
	Hydra *hydra;
	CameraCharacter *cameraCharacter;
	btDiscreteDynamicsWorld* world;
	bool gameOver = false;

	Level();
	~Level();
	void draw(DrawMode drawMode);
	void update();
	void update(double frameTime, double totalTime);
	unsigned int textureID = 0;

	float health = 100;
	float maxHealth = 100;
};

#endif