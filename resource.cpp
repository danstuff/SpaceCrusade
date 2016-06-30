#include"resource.h"

void setRenderWindow(Rsc *rsc, RenderWindow *window){
	rsc->window = window;
}

void loadSpriteMap(Rsc *rsc, const char* filename){
	rsc->spritemap.loadFromFile(filename);
}

void addGraphic(Rsc *rsc, Graphic graphicname, float x, float y, float width, float height){
	VertexArray graphic(Quads, 4);

	graphic[0].position = Vector2f(-width/2,-height/2);
	graphic[1].position = Vector2f( width/2,-height/2);
	graphic[2].position = Vector2f( width/2, height/2);
	graphic[3].position = Vector2f(-width/2, height/2);

	graphic[0].texCoords = Vector2f(x, y);
	graphic[1].texCoords = Vector2f(x+width, y);
	graphic[2].texCoords = Vector2f(x+width, y+height);
	graphic[3].texCoords = Vector2f(x, y+height);

	rsc->graphics[graphicname] = graphic;
}

void addAllGraphics(Rsc *rsc){
	addGraphic(rsc, GRAPHIC_TITLE, 0,  0, 128, 64);
	addGraphic(rsc, GRAPHIC_BACKGROUND, 0, 128, 256, 128);
	
	unsigned int graphicNum = 1;

	for(unsigned int y = 0; y < 7; y++){
		for(unsigned int x = 0; x < 8; x++){
			if(graphicNum < 51){
				addGraphic(rsc, static_cast<Graphic>(graphicNum), 128+x*16.0f, y*16.0f, 16, 16);
				graphicNum++;
			}
			else break;
		}
	}
}

void setGraphicColor(Rsc *rsc, Graphic graphic, Uint8 r, Uint8 g, Uint8 b){
	Color color(r, g, b);

	rsc->graphics[graphic][0].color = color;
	rsc->graphics[graphic][1].color = color;
	rsc->graphics[graphic][2].color = color;
	rsc->graphics[graphic][3].color = color;
}

void drawGraphic(Rsc *rsc, Graphic graphic, float x, float y, float angle){
	Transform transform = Transform::Identity;
	transform.translate(x,y);
	transform.rotate(angle);
	transform.scale(Vector2f(3,3), Vector2f(0,0));

	RenderStates states;
	states.texture = &rsc->spritemap;
	states.transform = transform;

	rsc->window->draw(rsc->graphics[graphic], states);
}


bool checkGameEvent(Rsc *rsc, GameEvent event){
	return rsc->events[event].isActive;
}

void triggerGameEvent(Rsc *rsc, GameEvent event){
	rsc->events[event].willBeActive = true;
	rsc->events[event].isActive = false;
}

void resetGameEvents(Rsc *rsc, bool fullReset){
	if(fullReset == true){
		for(unsigned int i = 0; i < 21; i++){
			rsc->events[i].isActive = false;
			rsc->events[i].willBeActive = false;
		}
	}
	else{
		for(unsigned int i = 0; i < 21; i++){
			if(rsc->events[i].isActive == true){
				rsc->events[i].isActive = false;
				rsc->events[i].willBeActive = false;
			}
			if(rsc->events[i].willBeActive == true){
				rsc->events[i].isActive = true;
				rsc->events[i].willBeActive = false;
			}
		}
	}
}


void loadSound(Rsc *rsc, GameSound sound, string filename){
	rsc->soundbufs[sound].loadFromFile(filename);
	rsc->sounds[sound].setBuffer(rsc->soundbufs[sound]);
}

void loadAllSounds(Rsc *rsc){
	loadSound(rsc, SOUND_CLICK, "rsc/sounds/click.wav");

	loadSound(rsc, SOUND_EXPLOSION_1, "rsc/sounds/explosion_1.wav");
	loadSound(rsc, SOUND_EXPLOSION_2, "rsc/sounds/explosion_2.wav");

	loadSound(rsc, SOUND_HURT_1, "rsc/sounds/hurt_1.wav");
	loadSound(rsc, SOUND_HURT_2, "rsc/sounds/hurt_2.wav");

	loadSound(rsc, SOUND_HIT, "rsc/sounds/hit.wav");

	loadSound(rsc, SOUND_LASER_1, "rsc/sounds/laser_1.wav");
	loadSound(rsc, SOUND_LASER_2, "rsc/sounds/laser_2.wav");

	loadSound(rsc, SOUND_PLAYER_DEATH, "rsc/sounds/player_death.wav");

	loadSound(rsc, SOUND_POINT, "rsc/sounds/point.wav");

	loadSound(rsc, SOUND_POWERUP, "rsc/sounds/powerup.wav");

	loadSound(rsc, SOUND_ROCKET_1, "rsc/sounds/rocket_1.wav");
	loadSound(rsc, SOUND_ROCKET_2, "rsc/sounds/rocket_2.wav");
}

void playSound(Rsc *rsc, GameSound sound){
	rsc->sounds[sound].play();
}

void playRandomSound(Rsc *rsc, GameSoundCategory category, int seed){
	switch(category){
	case SOUNDCAT_EXPLOSION:
		if(seed == 1)
			playSound(rsc, SOUND_EXPLOSION_1);
		else
			playSound(rsc, SOUND_EXPLOSION_2);
		break;
	case SOUNDCAT_HURT:
		if(seed == 1)
			playSound(rsc, SOUND_HURT_1);
		else
			playSound(rsc, SOUND_HURT_2);
		break;
	case SOUNDCAT_LASER:
		if(seed == 1)
			playSound(rsc, SOUND_LASER_1);
		else
			playSound(rsc, SOUND_LASER_2);
		break;
	case SOUNDCAT_ROCKET:
		if(seed == 1)
			playSound(rsc, SOUND_ROCKET_1);
		else
			playSound(rsc, SOUND_ROCKET_2);
	}
}


void loadMusic(Rsc *rsc, GameMusic song, string filename){
	rsc->music[song].openFromFile(filename);
	rsc->music[song].setVolume(20);
	rsc->music[song].setLoop(true);
}
void loadAllMusic(Rsc *rsc){
	loadMusic(rsc, MUSIC_01, "rsc/music/Deadly Class.ogg");
	loadMusic(rsc, MUSIC_02, "rsc/music/Reaching Null.ogg");
	loadMusic(rsc, MUSIC_03, "rsc/music/testflight remix.ogg");
}

void playSong(Rsc *rsc, GameMusic song){
	if(rsc->music[song].getStatus() == Music::Stopped)
		rsc->music[song].play();
}
void stopSong(Rsc *rsc, GameMusic song){
	if(rsc->music[song].getStatus() == Music::Playing)
		rsc->music[song].stop();
}

bool musicIsPlaying(Rsc *rsc){
	for(unsigned int i = 0; i < 3; i++){
		if(rsc->music[i].getStatus() == Music::Playing)
			return true;
	}

	return false;
}