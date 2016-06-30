#include"world.h"

unsigned int getRandom(unsigned int max){
	static Clock randomTime;

	unsigned int random = 0;

	random = unsigned int(rand() * randomTime.getElapsedTime().asSeconds()) % max;

	return random;
}

Entity createEntity(World* world){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if(world->mask[entity] == COMPONENT_EMPTY){
			return entity;
		}
	}
	return ENTITY_MAX;
}

void destroyEntity(World* world, Entity entity){
	world->mask[entity] = COMPONENT_EMPTY;
}

void clearWorld(World *world){
	for(Entity entity = 0; entity < ENTITY_MAX; entity++)
		world->mask[entity] = COMPONENT_EMPTY;

	world->wave = 0;
}

Entity createPlayer(World* world, float x, float y){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   | 
						  COMPONENT_VELOCITY   |
						  COMPONENT_FRICTION   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLOR	   |
						  COMPONENT_EMITTER	   |
						  COMPONENT_HEALTH	   |
						  COMPONENT_CONTROLLED |
						  COMPONENT_COLLISION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = 0;
	world->velocity[entity].y = 0;

	world->appearance[entity].id = GRAPHIC_PLAYER;

	world->color[entity] = world->playerColor;

	world->emitter[entity].type = 0;
	world->emitter[entity].offset = 10;
	world->emitter[entity].size = 10;

	world->health[entity].value = 50;
	world->health[entity].max = 50;

	world->controlled[entity].rocketBoost = false;

	world->collision[entity].radius = 38;

	return entity;
}

Entity createPlayerColorTemplate(World* world, float x, float y, float targetx, float targety){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION     | 
						  COMPONENT_ROTATION     |
						  COMPONENT_APPEARANCE   |
						  COMPONENT_COLOR	     |
						  COMPONENT_COLORTEMPLATE|
						  COMPONENT_TRANSITION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->appearance[entity].id = GRAPHIC_PLAYER;

	world->color[entity].r = 50;
	world->color[entity].g = 50;
	world->color[entity].b = 50;

	world->transition[entity].target.x = targetx;
	world->transition[entity].target.y = targety;

	return entity;
}

Entity createEnemyShooter(World* world, float x, float y, int difficulty, Entity target){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   | 
						  COMPONENT_VELOCITY   |
						  COMPONENT_FRICTION   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLOR	   |
						  COMPONENT_EMITTER	   |
						  COMPONENT_HEALTH	   |
						  COMPONENT_COLLISION  |
						  COMPONENT_AI_AVOIDING|
						  COMPONENT_AI_SHOOTING;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = 0;
	world->velocity[entity].y = 0;

	world->appearance[entity].id = GRAPHIC_FIGHTER;

	world->color[entity].r = getRandom(255);
	world->color[entity].g = getRandom(255);
	world->color[entity].b = getRandom(255);

	world->emitter[entity].type = 1;
	world->emitter[entity].offset = 25;
	world->emitter[entity].size = 10;

	world->health[entity].value = 6.5f;
	world->health[entity].max = 6.5f;

	world->collision[entity].radius = 30;

	world->ai[entity].target = target;
	world->ai[entity].timer = 15;
	world->ai[entity].movespeed = 4.0f+difficulty;

	return entity;
}
Entity createEnemyBomber(World* world, float x, float y, int difficulty, Entity target){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   | 
						  COMPONENT_VELOCITY   |
						  COMPONENT_FRICTION   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLOR	   |
						  COMPONENT_EMITTER	   |
						  COMPONENT_HEALTH	   |
						  COMPONENT_COLLISION  |
						  COMPONENT_EXPLOSIVE  |
						  COMPONENT_AI_SUICIDE;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = 0;
	world->velocity[entity].y = 0;

	world->appearance[entity].id = GRAPHIC_BOMBER;

	world->color[entity].r = getRandom(255);
	world->color[entity].g = getRandom(255);
	world->color[entity].b = getRandom(255);

	world->emitter[entity].type = 1;
	world->emitter[entity].offset = 40;
	world->emitter[entity].size = 10;

	world->health[entity].value = 4.0f;
	world->health[entity].max = 4.0f;

	world->collision[entity].radius = 30;

	world->explosive[entity].damage = 10.0f;
	world->explosive[entity].delay = 10;
	world->explosive[entity].radius = 2;
	world->explosive[entity].killsenemies = false;

	world->ai[entity].target = target;
	world->ai[entity].timer = 0;
	world->ai[entity].movespeed = 7.0f+difficulty;

	return entity;
}
Entity createEnemyTank(World* world, float x, float y, int difficulty, Entity target){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   | 
						  COMPONENT_VELOCITY   |
						  COMPONENT_FRICTION   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLOR	   |
						  COMPONENT_EMITTER	   |
						  COMPONENT_HEALTH	   |
						  COMPONENT_COLLISION  |
						  COMPONENT_AI_AVOIDING|
						  COMPONENT_AI_SHOOTING|
						  COMPONENT_AI_ROCKETS;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = 0;
	world->velocity[entity].y = 0;

	world->appearance[entity].id = GRAPHIC_TANK;

	world->color[entity].r = getRandom(255);
	world->color[entity].g = getRandom(255);
	world->color[entity].b = getRandom(255);

	world->emitter[entity].type = 1;
	world->emitter[entity].offset = 40;
	world->emitter[entity].size = 10;

	world->health[entity].value = 10.0f;
	world->health[entity].max = 10.0f;

	world->collision[entity].radius = 30;

	world->ai[entity].target = target;
	world->ai[entity].timer = 15;
	world->ai[entity].movespeed = 2.5f+difficulty;

	return entity;
}

Entity createRocket(World* world, float x, float y, float velx, float vely, float angle, bool killsenemies){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_VELOCITY   |
						  COMPONENT_ROTATION   |
						  COMPONENT_EXPLOSIVE  |
						  COMPONENT_APPEARANCE |
						  COMPONENT_EMITTER	   |
						  COMPONENT_KILLTIME   |
						  COMPONENT_COLLISION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = velx*50;
	world->velocity[entity].y = vely*50;

	world->rotation[entity].angle = angle;

	world->explosive[entity].damage = 10;
	world->explosive[entity].radius = 2;
	world->explosive[entity].delay = 1.5;
	world->explosive[entity].killsenemies = killsenemies;

	world->killtime[entity].value = 25;

	world->appearance[entity].id = GRAPHIC_ROCKET;

	world->emitter[entity].type = 1;
	world->emitter[entity].offset = 25;
	world->emitter[entity].size = 3;

	world->collision[entity].radius = 3;

	return entity;
}
Entity createBullet(World* world, float x, float y, float velx, float vely, float angle, bool killsenemies){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_VELOCITY   |
						  COMPONENT_ROTATION   |
						  COMPONENT_EXPLOSIVE  |
						  COMPONENT_APPEARANCE |
						  COMPONENT_EMITTER	   |
						  COMPONENT_KILLTIME   |
						  COMPONENT_COLLISION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = velx*100;
	world->velocity[entity].y = vely*100;

	world->rotation[entity].angle = angle;

	world->explosive[entity].damage = 2;
	world->explosive[entity].radius = 1;
	world->explosive[entity].delay = 1;
	world->explosive[entity].killsenemies = killsenemies;

	world->killtime[entity].value = 25;

	world->appearance[entity].id = GRAPHIC_LASER;

	world->emitter[entity].type = 3;
	world->emitter[entity].offset = 25;
	world->emitter[entity].size = 3;

	world->collision[entity].radius = 3;

	return entity;
}

Entity createPowerUp(World* world, float x, float y, Graphic type){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLLISION  |
						  COMPONENT_KILLTIME   |
						  COMPONENT_POWERUP;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->appearance[entity].id = type;

	world->collision[entity].radius = 4;

	world->killtime[entity].value = 30;

	return entity;
}

Entity createParticle(World* world, float x, float y, Uint8 r, Uint8 g, Uint8 b, float angle, float vel){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_VELOCITY   |
						  COMPONENT_ROTATION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLOR	   |
						  COMPONENT_KILLTIME;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->velocity[entity].x = -cos(angle*PI/180)*vel;
	world->velocity[entity].y = sin(angle*PI/180)*vel;

	world->appearance[entity].id = GRAPHIC_PARTICLE;

	world->color[entity].r = r;
	world->color[entity].g = g;
	world->color[entity].b = b;

	world->killtime[entity].value = 6;

	return entity;
}

Entity createExplosion(World* world, float x, float y){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_EMITTER    |
						  COMPONENT_KILLTIME;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->emitter[entity].type = 2;
	world->emitter[entity].offset = 0;
	world->emitter[entity].size = 3;

	world->killtime[entity].value = 2;

	return entity;
}
Entity createFireWork(World* world, float x, float y){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_EMITTER    |
						  COMPONENT_KILLTIME;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->emitter[entity].type = 4;
	world->emitter[entity].offset = 0;
	world->emitter[entity].size = 3;

	world->killtime[entity].value = 2;

	return entity;
}

Entity createBillboard(World* world, float x, float y, Graphic graphic, Uint8 r, Uint8 g, Uint8 b, float targetx, float targety){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_APPEARANCE |
						  COMPONENT_COLOR	   |
						  COMPONENT_TRANSITION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->appearance[entity].id = graphic;

	world->color[entity].r = r;
	world->color[entity].g = g;
	world->color[entity].b = b;

	world->transition[entity].target.x = targetx;
	world->transition[entity].target.y = targety;

	return entity;
}

Entity createButton(World* world, float x, float y, GameEvent action, string text, Uint8 r, Uint8 g, Uint8 b, float targetx, float targety){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_TEXT	   |
						  COMPONENT_CLICK      |
						  COMPONENT_COLOR	   |
						  COMPONENT_TRANSITION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->text[entity] = text;

	world->click[entity].event = action;
	world->click[entity].prevClick = true;

	world->color[entity].r = r;
	world->color[entity].g = g;
	world->color[entity].b = b;

	world->transition[entity].target.x = targetx;
	world->transition[entity].target.y = targety;

	return entity;
}

Entity createPopup(World* world, float x, float y, string text, Uint8 r, Uint8 g, Uint8 b, float targetx, float targety){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_KILLTIME   |
						  COMPONENT_TEXT	   |
						  COMPONENT_COLOR	   |
						  COMPONENT_TRANSITION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->killtime[entity].value = 10;

	world->text[entity] = text;

	world->color[entity].r = r;
	world->color[entity].g = g;
	world->color[entity].b = b;

	world->transition[entity].target.x = targetx;
	world->transition[entity].target.y = targety;

	return entity;
}

Entity createScoreboard(World* world, float x, float y, string text, GameEvent trigger, float targetx, float targety){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_TEXT	   |
						  COMPONENT_COUNTER    |
						  COMPONENT_TRANSITION;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->text[entity] = text;

	world->counter[entity].value = 0;
	world->counter[entity].trigger = trigger;

	world->transition[entity].target.x = targetx;
	world->transition[entity].target.y = targety;

	return entity;
};

Entity createHiscorelist(World* world, float x, float y, float targetx, float targety){
	Entity entity = createEntity(world);

	world->mask[entity] = COMPONENT_POSITION   |
						  COMPONENT_TRANSITION |
						  COMPONENT_TEXT;

	world->position[entity].x = x;
	world->position[entity].y = y;

	world->transition[entity].target.x = targetx;
	world->transition[entity].target.y = targety;

	world->text[entity] = " ";

	ifstream scorefile("rsc/score.dat", ios::in);

	for(unsigned int i = 0; i <= 5 && scorefile.is_open(); i++){
		string line;
		getline(scorefile, line);

		world->text[entity] = world->text[entity] + '#' + line;
	}
	
	return entity;
}