#ifndef RESOURCE_H
#define RESOURCE_H

#include"main.h"

enum Graphic{	
	GRAPHIC_TITLE		= 0,
	
	GRAPHIC_A			= 1,
	GRAPHIC_B			= 2,
	GRAPHIC_C			= 3,
	GRAPHIC_D			= 4,
	GRAPHIC_E			= 5,
	GRAPHIC_F			= 6,
	GRAPHIC_G			= 7,
	GRAPHIC_H			= 8,
	GRAPHIC_I			= 9,
	GRAPHIC_J			= 10,
	GRAPHIC_K			= 11,
	GRAPHIC_L			= 12,
	GRAPHIC_M			= 13,
	GRAPHIC_N			= 14,
	GRAPHIC_O			= 15,
	GRAPHIC_P			= 16,
	GRAPHIC_Q			= 17,
	GRAPHIC_R			= 18,
	GRAPHIC_S			= 19,
	GRAPHIC_T			= 20,
	GRAPHIC_U			= 21,
	GRAPHIC_V			= 22,
	GRAPHIC_W			= 23,
	GRAPHIC_X			= 24,
	GRAPHIC_Y			= 25,
	GRAPHIC_Z			= 26,

	GRAPHIC_0			= 27,
	GRAPHIC_1			= 28,
	GRAPHIC_2			= 29,
	GRAPHIC_3			= 30,
	GRAPHIC_4			= 31,
	GRAPHIC_5			= 32,
	GRAPHIC_6			= 33,
	GRAPHIC_7			= 34,
	GRAPHIC_8			= 35,
	GRAPHIC_9			= 36,
	
	GRAPHIC_XPT			= 37,
	GRAPHIC_PCT			= 38,
	GRAPHIC_LARROW		= 39,
	GRAPHIC_RARROW		= 40,

	GRAPHIC_PLAYER		= 41,
	GRAPHIC_FIGHTER		= 42,
	GRAPHIC_TANK		= 43,
	GRAPHIC_BOMBER		= 44,
	GRAPHIC_LASER		= 45,
	GRAPHIC_ROCKET		= 46,
	GRAPHIC_PARTICLE	= 47,
	GRAPHIC_RBOOST		= 48,
	GRAPHIC_HPUP		= 49,
	GRAPHIC_BOMB		= 50,

	GRAPHIC_BACKGROUND	= 51
};

enum GameSound{
	SOUND_CLICK			= 0,

	SOUND_EXPLOSION_1	= 1,
	SOUND_EXPLOSION_2	= 2,
	
	SOUND_HURT_1		= 3,
	SOUND_HURT_2		= 4,

	SOUND_HIT			= 5,

	SOUND_LASER_1		= 6,
	SOUND_LASER_2		= 7,
	
	SOUND_PLAYER_DEATH	= 8,

	SOUND_POINT			= 9,

	SOUND_POWERUP		= 10,

	SOUND_ROCKET_1		= 11,
	SOUND_ROCKET_2		= 12,

	SOUND_LAND			= 13
};

enum GameSoundCategory{
	SOUNDCAT_EXPLOSION	= 0,
	SOUNDCAT_HURT		= 1,
	SOUNDCAT_LASER		= 2,
	SOUNDCAT_ROCKET		= 3
};

enum GameMusic{
	MUSIC_01 = 0,
	MUSIC_02 = 1,
	MUSIC_03 = 2
};

enum GameEvent{
	EVENT_GOTO_TITLE	 = 0,
	EVENT_GOTO_HISCORES	 = 1,
	EVENT_GOTO_CSELECT	 = 2,
	EVENT_GOTO_GAME		 = 3,

	EVENT_ADD_POINT		 = 4,
	EVENT_ADD_POINTS_1	 = 5,
	EVENT_ADD_POINTS_2	 = 6,
	EVENT_ADD_POINTS_5	 = 7,
	EVENT_ADD_POINTS_10  = 8,

	EVENT_ENEMY_DEAD	 = 9,
	EVENT_PLAYER_DEAD	 = 10,
	
	EVENT_WAVE_START	 = 11,
	EVENT_WAVE_END		 = 12,

	EVENT_COLOR_RUP		 = 13,
	EVENT_COLOR_RDOWN	 = 14,
	EVENT_COLOR_GUP		 = 15,
	EVENT_COLOR_GDOWN	 = 16,
	EVENT_COLOR_BUP		 = 17,
	EVENT_COLOR_BDOWN	 = 18,

	EVENT_PLAYERDAMAGED	 = 19,

	EVENT_NONE			 = 20
};

struct REvent{
	bool willBeActive;
	bool isActive;
};

struct Rsc{
	RenderWindow *window;

	REvent events[21];

	Texture spritemap;
	VertexArray graphics[52];

	SoundBuffer soundbufs[13];
	Sound sounds[13];

	Music music[3];
};

void setRenderWindow(Rsc *rsc, RenderWindow *window);

void loadSpriteMap(Rsc *rsc, const char* filename);

void addGraphic(Rsc *rsc, float tx, float ty, float width, float height);
void addAllGraphics(Rsc *rsc);

void setGraphicColor(Rsc *rsc, Graphic graphic, Uint8 r, Uint8 g, Uint8 b);
void drawGraphic(Rsc *rsc, Graphic graphic, float x, float y, float angle);

bool checkGameEvent(Rsc *rsc, GameEvent event);
void triggerGameEvent(Rsc *rsc, GameEvent event);

void resetGameEvents(Rsc *rsc, bool fullReset);

void loadSound(Rsc *rsc, GameSound sound);
void loadAllSounds(Rsc *rsc);
void playSound(Rsc *rsc, GameSound sound);
void playRandomSound(Rsc *rsc, GameSoundCategory category, int seed);

void loadMusic(Rsc *rsc, GameMusic song, string filename);
void loadAllMusic(Rsc *rsc);

void playSong(Rsc *rsc, GameMusic song);
void stopSong(Rsc *rsc, GameMusic song);

bool musicIsPlaying(Rsc *rsc);

#endif