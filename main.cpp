#include"system.h"
#include"world.h"

RenderWindow *window;
Rsc *resource;

World *world;

void init(){
	loadSpriteMap(resource, "rsc/spritemap.png");
	addAllGraphics(resource);

	drawGraphic(resource, GRAPHIC_BUTTON, WINDOW_WIDTH/2.0f-32, WINDOW_HEIGHT/2.0f-32, 0, window);
	window->display();

	world = new World;
	clearWorld(world);
}

void updateWindow(){
	if(Keyboard::isKeyPressed(Keyboard::Escape))
			window->close();

	sf::Event event;
	while (window->pollEvent(event))
		if (event.type == Event::Closed)
			window->close();
}

void initTitleMenu(){
	clearWorld(world);

	createButton(world, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, EVENT_GOTO_GAME);
	createButton(world, WINDOW_WIDTH/2, WINDOW_HEIGHT/2+100, EVENT_GOTO_OPTIONS);
}

void initOptions(){
	clearWorld(world);

	createButton(world, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, EVENT_GOTO_TITLE);
}

void initGame(){
	clearWorld(world);

	createPlayer(world, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
	createEnemy(world, 50, 50);
	createEnemy(world, 100, 50);
	createEnemy(world, 150, 50);
	createEnemy(world, 200, 50);
	createEnemy(world, 250, 50);
}

void handleSystems(bool efficient){
	handleMovement(world);
		
	handleAudio(world, resource);
		
	handleClick(world, window);

	handlePlayer(world, window);
	handleEnemy(world);

	handleExplosion(world);

	handleRender(world, resource, window);
}

void handleEvents(){
	if(checkGameEvent(world, EVENT_GOTO_TITLE))
		initTitleMenu();
	if(checkGameEvent(world, EVENT_GOTO_OPTIONS))
		initOptions();
	if(checkGameEvent(world, EVENT_GOTO_GAME))
		initGame();
}

int main(){
	window = new RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Crusade!", Style::Titlebar);
	resource = new Rsc;

	init();

	initTitleMenu();

    while (window->isOpen()){
		handleSystems(false);
		handleEvents();

		updateWindow();
	}

	delete world;
	delete resource;
	delete window;
    return 0;
}