#ifndef SYSTEM_H
#define SYSTEM_H

#include"main.h"
#include"world.h"

void handleMovement(World *world);
void handleRender(World *world, Rsc *rsc, RenderWindow *window);

void handleAudio(World *world, Rsc *rsc);

void handleClick(World *world, RenderWindow *window);

void handlePlayer(World *world, RenderWindow *window);
void handleEnemy(World *world);

void handleExplosion(World *world);

#endif