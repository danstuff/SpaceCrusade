#include"world.h"

bool checkGameEvent(World *world, GameEvent event){
	if(world->events[event]){
		world->events[event] = false;
		return true;
	}
	return false;
}

void triggerGameEvent(World *world, GameEvent event){
	world->events[event] = true;
}


unsigned int createEntity(World* world){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if(world->mask[entity] == COMPONENT_EMPTY)
			return entity;
	}
	return ENTITY_MAX;
}

void destroyEntity(World* world, unsigned int entity){
	world->mask[entity] = COMPONENT_EMPTY;
}
void clearWorld(World *world){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++)
		world->mask[entity] = COMPONENT_EMPTY;

	world->events[0] = 0;
	world->events[1] = 0;
	world->events[2] = 0;
}

unsigned int createPlayer(World* world, float x, float y){
	unsigned int entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   | 
						  COMPONENT_VELOCITY   |
						  COMPONENT_FRICTION   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_CONTROLLED;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = 0;
	world->velocity[entity].y = 0;

	world->appearance[entity].id = GRAPHIC_PLAYERSHIP;

	return entity;
}

unsigned int createEnemy(World* world, float x, float y){
	unsigned int entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   | 
						  COMPONENT_VELOCITY   |
						  COMPONENT_FRICTION   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_HEALTH	   |
						  COMPONENT_COLLISION  |
						  COMPONENT_AI;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = 0;
	world->velocity[entity].y = 0;

	world->appearance[entity].id = GRAPHIC_PLAYERSHIP;

	world->health[entity].value = 30;
	world->collision[entity].radius = 50;

	return entity;
}

unsigned int createBullet(World* world, float x, float y, float velx, float vely, float angle){
	unsigned int entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_VELOCITY   |
						  COMPONENT_ROTATION   |
						  COMPONENT_EXPLOSIVE  |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLLISION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = velx*100;
	world->velocity[entity].y = vely*100;

	world->rotation[entity].angle = angle;

	world->explosive[entity].damage = 3;
	world->explosive[entity].radius = 1;

	world->appearance[entity].id = GRAPHIC_BULLET;

	world->collision[entity].radius = 1;

	return entity;
}

unsigned int createRocket(World* world, float x, float y, float velx, float vely, float angle){
	unsigned int entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_VELOCITY   |
						  COMPONENT_ROTATION   |
						  COMPONENT_EXPLOSIVE  |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLLISION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = velx*50;
	world->velocity[entity].y = vely*50;

	world->rotation[entity].angle = angle;

	world->explosive[entity].damage = 10;
	world->explosive[entity].radius = 20;

	world->appearance[entity].id = GRAPHIC_ROCKET;

	world->collision[entity].radius = 5;

	return entity;
}

unsigned int createButton(World* world, float x, float y, GameEvent action){
	unsigned int entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_CLICK;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->appearance[entity].id = GRAPHIC_BUTTON;

	world->click[entity].event = action;

	return entity;
}

unsigned int createScoreBoard(World* world, float x, float y){
	unsigned int entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_CLICK;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->appearance[entity].id = GRAPHIC_SCOREBOARD;

	return entity;
};
