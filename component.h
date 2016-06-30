#ifndef COMPONENT_H
#define COMPONENT_H

#include"main.h"
#include"resource.h"

const unsigned int ENTITY_MAX = 10000U;
typedef unsigned int Entity;

//only suppourts up to 32 components, use std::bitset for more
enum Component{
	COMPONENT_EMPTY			= 0,

	COMPONENT_POSITION		= 1 << 0,
	COMPONENT_VELOCITY		= 1 << 1,
	COMPONENT_FRICTION		= 1 << 2,
	COMPONENT_ROTATION		= 1 << 3,

	COMPONENT_APPEARANCE	= 1 << 4,
	COMPONENT_COLOR			= 1 << 5,
	COMPONENT_AUDIO			= 1 << 6,

	COMPONENT_CLICK			= 1 << 7,
	COMPONENT_COUNTER		= 1 << 8,
	COMPONENT_KILLTIME		= 1 << 9,

	COMPONENT_CONTROLLED	= 1 << 10,
	COMPONENT_AI_SUICIDE	= 1 << 11,
	COMPONENT_AI_AVOIDING	= 1 << 12,
	COMPONENT_AI_SHOOTING	= 1 << 13,
	COMPONENT_AI_ROCKETS	= 1 << 14,

	COMPONENT_LASER			= 1 << 15,
	COMPONENT_EMITTER		= 1 << 16,

	COMPONENT_HEALTH		= 1 << 18,
	COMPONENT_EXPLOSIVE		= 1 << 19,
	COMPONENT_COLLISION		= 1 << 20,

	COMPONENT_COLORTEMPLATE = 1 << 21,
	COMPONENT_TEXT			= 1 << 22,
	COMPONENT_TRANSITION	= 1 << 23,

	COMPONENT_POWERUP		= 1 << 24
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

struct ColorMod{
	Uint8 r, g, b;
};

struct Audio{
	GameSound id;
	bool on;
};

struct Click{
	GameEvent event;

	bool prevClick;
};

struct Counter{
	int value;
	GameEvent trigger;
};

struct KillTime{
	float value;
};

struct Controlled{
	bool rocketBoost;
};

struct AI{
	Entity target;
	float timer;

	float movespeed;
};

struct Laser{
	Entity emitter;
	unsigned int units;
	float damage;
	bool killsenemies;
};

struct Emitter{
	unsigned int type;
	unsigned int size;

	float offset;
};

struct Health{
	float max;
	float value;
};

struct Explosive{
	float radius;
	float damage;
	float delay;
	bool killsenemies;
};

struct Collision{
	float radius;
};

struct Transition{
	Position target;
};

#endif