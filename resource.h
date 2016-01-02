#ifndef RESOURCE_H
#define RESOURCE_H

#include"main.h"

enum Graphic{
	GRAPHIC_PLAYERSHIP = 0,
	GRAPHIC_ENEMYSHIP  = 1,

	GRAPHIC_BULLET = 2,
	GRAPHIC_ROCKET = 3,

	GRAPHIC_BUTTON     = 4,
	GRAPHIC_SCOREBOARD = 5
};

enum GameSound{
	SOUND_SHOOT = 0
};

struct Rsc{
	Texture spritemap;
	vector<VertexArray> graphics;
};

void loadSpriteMap(Rsc *rsc, const char* filename);

void addGraphic(Rsc *rsc, float tx, float ty, float width, float height);
void addAllGraphics(Rsc *rsc);

void drawGraphic(Rsc *rsc, Graphic graphic, float x, float y, float angle, RenderWindow *window);

#endif