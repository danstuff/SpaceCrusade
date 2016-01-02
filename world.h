#ifndef WORLD_H
#define WORLD_H

#include"main.h"
#include"component.h"

struct World{
	bool events[3];

	unsigned int mask[ENTITY_MAX];

	Position   position  [ENTITY_MAX];
	Velocity   velocity  [ENTITY_MAX];
	Rotation   rotation  [ENTITY_MAX];

	Appearance appearance[ENTITY_MAX];

	Audio audio[ENTITY_MAX];

	Click click[ENTITY_MAX];

	Health health[ENTITY_MAX];
	Explosive explosive[ENTITY_MAX];
	Collision collision[ENTITY_MAX];
};

bool checkGameEvent(World *world, GameEvent event);
void triggerGameEvent(World *world, GameEvent event);

unsigned int createEntity(World *world);
void destroyEntity(World* world, unsigned int entity);
void clearWorld(World *world);

unsigned int createPlayer(World* world, float x, float y);
unsigned int createEnemy(World* world, float x, float y);
unsigned int createBullet(World* world, float x, float y, float velx, float vely, float angle);
unsigned int createRocket(World* world, float x, float y, float velx, float vely, float angle);

unsigned int createButton(World* world, float x, float y, GameEvent action);
unsigned int createScoreBoard(World* world, float x, float y);

#endif