#include"resource.h"

void loadSpriteMap(Rsc *rsc, const char* filename){
	rsc->spritemap.loadFromFile(filename);
}

void addGraphic(Rsc *rsc, float tx, float ty, float width, float height){
	VertexArray graphic(Quads, 4);

	graphic[0].position = Vector2f(-width/2,-height/2);
	graphic[1].position = Vector2f( width/2,-height/2);
	graphic[2].position = Vector2f( width/2, height/2);
	graphic[3].position = Vector2f(-width/2, height/2);

	graphic[0].texCoords = Vector2f(tx, ty);
	graphic[1].texCoords = Vector2f(tx+width, ty);
	graphic[2].texCoords = Vector2f(tx+width, ty+height);
	graphic[3].texCoords = Vector2f(tx, ty+height);

	rsc->graphics.push_back(graphic);
}

void addAllGraphics(Rsc *rsc){
	addGraphic(rsc, 0, 0, 64, 64);
	addGraphic(rsc, 0, 0, 64, 64);

	addGraphic(rsc, 64, 0, 64, 64);
	addGraphic(rsc, 0, 64, 64, 64);

	addGraphic(rsc, 0, 128, 128, 128);
	addGraphic(rsc, 0, 0, 64, 64);
}

void drawGraphic(Rsc *rsc, Graphic graphic, float x, float y, float angle, RenderWindow *window){
	if(!rsc->graphics.empty() && rsc->graphics.size() >= unsigned int(graphic)){
		Transform transform = Transform::Identity;
		transform.translate(x,y);
		transform.rotate(angle);

		RenderStates states;
		states.texture = &rsc->spritemap;
		states.transform = transform;

		window->draw(rsc->graphics[graphic], states);
	}
}
