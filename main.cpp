#include"system.h"
#include"world.h"

RenderWindow *window;
Rsc *rsc;

World *world;

void init(){
	setRenderWindow(rsc, window);
	window->setFramerateLimit(120);

	loadSpriteMap(rsc, "rsc/spritemap.png");
	addAllGraphics(rsc);
	loadAllSounds(rsc);
	loadAllMusic(rsc);

	world = new World;

	clearWorld(world);
	resetGameEvents(rsc, true);
}

void updateWindow(){
	sf::Event event;
	while (window->pollEvent(event))
		if (event.type == Event::Closed)
			window->close();
}

void initTitleMenu(){
	clearWorld(world);

	createBillboard(world, WINDOW_WIDTH/2, -100, GRAPHIC_TITLE, 255, 255, 255, WINDOW_WIDTH/2, WINDOW_HEIGHT/2-100);

	createButton(world, WINDOW_WIDTH/2-48,  -100, EVENT_GOTO_CSELECT,  "PLAY",     255, 255, 255, WINDOW_WIDTH/2-48,  WINDOW_HEIGHT/2+40);
	createButton(world, WINDOW_WIDTH/2-110, -100, EVENT_GOTO_HISCORES, "HISCORES", 255, 255, 255, WINDOW_WIDTH/2-110, WINDOW_HEIGHT/2+120);

	if(!musicIsPlaying(rsc)){
		if(getRandom(2) == 0)
			playSong(rsc, MUSIC_01);
		else
			playSong(rsc, MUSIC_02);
	}
}

void initHiscores(){
	clearWorld(world);

	createButton(world, 40, -100, EVENT_GOTO_TITLE, "<BACK", 255, 255, 255, 40, 30);

	createButton(world, WINDOW_WIDTH/2-110, -100, EVENT_NONE, "HISCORES", 255, 255, 255, WINDOW_WIDTH/2-110, 64);

	createHiscorelist(world, WINDOW_WIDTH/2-290, -100, WINDOW_WIDTH/2-290, 128);
}

void initColorSelect(){
	clearWorld(world);

	createButton(world, 40, -100, EVENT_GOTO_TITLE, "<BACK", 255, 255, 255, 40, 30);

	createButton(world, WINDOW_WIDTH/2-170, -100, EVENT_NONE, "COLOR SELECT", 255, 255, 255, WINDOW_WIDTH/2-170, 128);

	//color selectors
	createButton(world, WINDOW_WIDTH/2-64,  -100, EVENT_COLOR_RDOWN, "<", 255, 0, 0, WINDOW_WIDTH/2-64,  WINDOW_HEIGHT/2-48);
	createButton(world, WINDOW_WIDTH/2+64,  -100, EVENT_COLOR_RUP,   ">", 255, 0, 0, WINDOW_WIDTH/2+64,  WINDOW_HEIGHT/2-48);

	createButton(world, WINDOW_WIDTH/2-64,  -100, EVENT_COLOR_GDOWN, "<", 0, 255, 0, WINDOW_WIDTH/2-64,  WINDOW_HEIGHT/2);
	createButton(world, WINDOW_WIDTH/2+64,  -100, EVENT_COLOR_GUP,   ">", 0, 255, 0, WINDOW_WIDTH/2+64,  WINDOW_HEIGHT/2);

	createButton(world, WINDOW_WIDTH/2-64,  -100, EVENT_COLOR_BDOWN, "<", 0, 0, 255, WINDOW_WIDTH/2-64,  WINDOW_HEIGHT/2+48);
	createButton(world, WINDOW_WIDTH/2+64,  -100, EVENT_COLOR_BUP,   ">", 0, 0, 255, WINDOW_WIDTH/2+64,  WINDOW_HEIGHT/2+48);

	createPlayerColorTemplate(world, WINDOW_WIDTH/2, -100, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

	createButton(world, WINDOW_WIDTH/2-48, -100, EVENT_GOTO_GAME, "PLAY", 255, 255, 255, WINDOW_WIDTH/2-48, WINDOW_HEIGHT-128);
}

void initGame(){
	clearWorld(world);

	createScoreboard(world, 40,				  -100, "SCORE", EVENT_ADD_POINT, 40, 30);
	createScoreboard(world, WINDOW_WIDTH-220, -100, "HP",    EVENT_GOTO_GAME, WINDOW_WIDTH-220, 30);
	
	createPlayer(world, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

	world->wave = 1;

	world->playTime.restart();
}

void handleEvents(){
	if(checkGameEvent(rsc, EVENT_GOTO_TITLE))
		initTitleMenu();
	if(checkGameEvent(rsc, EVENT_GOTO_HISCORES))
		initHiscores();
	if(checkGameEvent(rsc, EVENT_GOTO_CSELECT))
		initColorSelect();
	if(checkGameEvent(rsc, EVENT_GOTO_GAME))
		initGame();
}

int main(){
	window = new RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Crusade!", Style::Close);
	rsc = new Rsc;

	init();

	initTitleMenu();

    while (window->isOpen()){
		handleEvents();

		handleWave(world, rsc);
		handleSystems(world, rsc);

		updateWindow();
		resetGameEvents(rsc, false);
	}

	delete world;
	delete rsc;
	delete window;
    return 0;
}