#ifndef COMPONENT_H
#define COMPONENT_H

#include"main.h"
#include"resource.h"

const unsigned int ENTITY_MAX = 1000U;

enum GameEvent{
	EVENT_GOTO_TITLE	 = 0,
	EVENT_GOTO_OPTIONS	 = 1,
	EVENT_GOTO_GAME		 = 2
};

//only suppourts up to 32 components, use std::bitset for more
enum Component{
	COMPONENT_EMPTY		 = 0,

	COMPONENT_POSITION   = 1 << 0,
	COMPONENT_VELOCITY	 = 1 << 1,
	COMPONENT_FRICTION   = 1 << 2,
	COMPONENT_ROTATION	 = 1 << 3,

	COMPONENT_APPEARANCE = 1 << 4,
	COMPONENT_AUDIO		 = 1 << 5,

	COMPONENT_CLICK		 = 1 << 6,

	COMPONENT_CONTROLLED = 1 << 7,
	COMPONENT_AI		 = 1 << 8,

	COMPONENT_HEALTH	 = 1 << 9,
	COMPONENT_EXPLOSIVE	 = 1 << 10,
	COMPONENT_COLLISION	 = 1 << 11
};

struct Position{
	float x;
	float y;
};

struct Velocity{
	float x;
	float y;
};

struct Accelerate{
	float x;
	float y;
};

struct Rotation{
	float angle;
};

struct Appearance{
	Graphic id;
};

struct Audio{
	GameSound id;
	bool on;
};

struct Click{
	GameEvent event;
};

struct Health{
	float value;
};

struct Explosive{
	float radius;
	float damage;
};

struct Collision{
	float radius;
};

#endif