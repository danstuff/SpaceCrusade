#ifndef WORLD_H
#define WORLD_H

#include"main.h"
#include"component.h"

struct World{
	unsigned int mask[ENTITY_MAX];

	Position   position  [ENTITY_MAX];
	Velocity   velocity  [ENTITY_MAX];
	Rotation   rotation  [ENTITY_MAX];

	Appearance appearance[ENTITY_MAX];
	ColorMod   color     [ENTITY_MAX];
	Audio      audio     [ENTITY_MAX];

	Click    click   [ENTITY_MAX];
	Counter  counter [ENTITY_MAX];
	KillTime killtime[ENTITY_MAX];

	Controlled controlled[ENTITY_MAX];

	AI ai[ENTITY_MAX];

	Laser	laser[ENTITY_MAX];
	Emitter emitter[ENTITY_MAX];

	Health	  health   [ENTITY_MAX];
	Explosive explosive[ENTITY_MAX];
	Collision collision[ENTITY_MAX];

	string text[ENTITY_MAX];

	Transition transition[ENTITY_MAX];

	ColorMod playerColor;

	Clock playTime;

	unsigned int wave;
};

unsigned int getRandom(unsigned int max);

Entity createEntity(World *world);
void destroyEntity(World* world, Entity entity);

void clearWorld(World *world);

Entity createPlayer(World* world, float x, float y);
Entity createPlayerColorTemplate(World* world, float x, float y, float targetx, float targety);
Entity createEnemyShooter(World* world, float x, float y, int difficulty, Entity target);
Entity createEnemyBomber(World* world, float x, float y, int difficulty, Entity target);
Entity createEnemyTank(World* world, float x, float y, int difficulty, Entity target);

Entity createRocket(World* world, float x, float y, float velx, float vely, float angle, bool killsenemies);
Entity createBullet(World* world, float x, float y, float velx, float vely, float angle, bool killsenemies);

Entity createPowerUp(World* world, float x, float y, Graphic type);

Entity createParticle(World* world, float x, float y, Uint8 r, Uint8 g, Uint8 b, float angle, float vel);
Entity createExplosion(World* world, float x, float y);
Entity createFireWork(World* world, float x, float y);

Entity createBillboard(World* world, float x, float y, Graphic graphic, Uint8 r, Uint8 g, Uint8 b, float targetx, float targety);
Entity createButton(World* world, float x, float y, GameEvent action, string text, Uint8 r, Uint8 g, Uint8 b, float targetx, float targety);
Entity createPopup(World* world, float x, float y, string text, Uint8 r, Uint8 g, Uint8 b, float targetx, float targety);
Entity createScoreboard(World* world, float x, float y, string text, GameEvent trigger, float targetx, float targety);
Entity createHiscorelist(World* world, float x, float y, float targetx, float targety);

#endif