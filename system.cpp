#include"system.h"


bool checkMask(World *world, Entity entity, int mask){
	return (world->mask[entity] & mask) == mask;
};

void deductHealth(World *world, Rsc *rsc, Entity entity, float value){
	//worlds shittiest function right here. but hey, it works.
	world->health[entity].value -= value;

	if(checkMask(world, entity, COMPONENT_CONTROLLED))
			triggerGameEvent(rsc, EVENT_PLAYERDAMAGED);

	if(world->health[entity].value <= 0){
		if(checkMask(world, entity, COMPONENT_AI_AVOIDING) ||
		   checkMask(world, entity, COMPONENT_AI_ROCKETS)  ||
		   checkMask(world, entity, COMPONENT_AI_SHOOTING) ||
		   checkMask(world, entity, COMPONENT_AI_SUICIDE)){
	   		triggerGameEvent(rsc, EVENT_ENEMY_DEAD);
			createExplosion(world, world->position[entity].x, world->position[entity].y);

			if(checkMask(world, entity, COMPONENT_AI_ROCKETS))
				triggerGameEvent(rsc, EVENT_ADD_POINTS_5);
			else if(checkMask(world, entity, COMPONENT_AI_SUICIDE))
				triggerGameEvent(rsc, EVENT_ADD_POINTS_1);
			else
				triggerGameEvent(rsc, EVENT_ADD_POINTS_2);

			destroyEntity(world, entity);

			playRandomSound(rsc, SOUNDCAT_EXPLOSION, getRandom(2));
		}
		else if(checkMask(world, entity, COMPONENT_CONTROLLED)){
			createExplosion(world, world->position[entity].x, world->position[entity].y);

			world->position[entity].y = 100000;
			world->health[entity].value  = 0;

			createButton(world, WINDOW_WIDTH/2-130, -100, EVENT_NONE,		"GAME OVER", 255, 240, 240, WINDOW_WIDTH/2-130, WINDOW_HEIGHT/2-48);
			createButton(world, WINDOW_WIDTH/2-78,  -100, EVENT_GOTO_TITLE, "RETRY",      255, 255, 255, WINDOW_WIDTH/2-78,  WINDOW_HEIGHT/2+48);

			playSound(rsc, SOUND_PLAYER_DEATH);
			stopSong(rsc, MUSIC_01);
			stopSong(rsc, MUSIC_02);

			triggerGameEvent(rsc, EVENT_PLAYER_DEAD);
		}
		else
			destroyEntity(world, entity);
	}
}

bool checkCollision(World *world, Entity entity1, Entity entity2, float multiplier = 1){
	float distX = world->position[entity1].x - world->position[entity2].x;
	float distY = world->position[entity1].y - world->position[entity2].y;

	float rad1 = world->collision[entity1].radius * multiplier;
	float rad2 = world->collision[entity2].radius * multiplier;

	return (distX*distX) + (distY*distY) <= (rad1+rad2) * (rad1+rad2);
}

Entity findPlayer(World *world){
	for(Entity player = 0; player < ENTITY_MAX; player++){
		if(checkMask(world, player, COMPONENT_CONTROLLED | COMPONENT_POSITION)){
			return player;
		}
	}

	return -1;
}


void screenShake(World *world, Rsc *rsc, float deltatime){
	//most important part of the game, screenshake
	static float i = 0;
	static float runtime = 0;

	if(runtime > 0){
		View view = rsc->window->getDefaultView();

		float rotfac = sin(i)*2;

		view.setRotation(rotfac);
	
		rsc->window->setView(view);

		i += (25 - i)*0.5f*deltatime;

		runtime -= 1*deltatime;
	}
	else if(checkGameEvent(rsc, EVENT_PLAYERDAMAGED)){
		runtime = 10;
		i = 0;

		playSound(rsc, SOUND_HURT_2);
	}
	else{
		rsc->window->setView(rsc->window->getDefaultView());
	}
}


void tileBackground(World* world, Rsc *rsc){
	for(float x = 0; x <= WINDOW_WIDTH; x += 768){
		for(float y = 0; y <= WINDOW_HEIGHT+384; y += 384){
			drawGraphic(rsc, GRAPHIC_BACKGROUND, x, y, 0);
		}
	}
}


void submitHiscore(World *world, Rsc *rsc, int score, int wave){
	ifstream scorefilein("rsc/score.dat");

	string scorestr = "";

	int oldscorefinal = 0;
	bool submitted = false;

	while(!scorefilein.eof()){
		string line;
		getline(scorefilein, line);

		int oldscore = 0, oldwave = 0;

		if(strcmp(line.c_str(), "SCORE"))
			sscanf_s(line.c_str(), "SCORE %i WAVE %i", &oldscore, &oldwave);
		
		if(oldscore > oldscorefinal)
			oldscorefinal = oldscore;
	
		if(score > oldscore && !submitted){
			scorestr = scorestr + "SCORE " + to_string(long double(score)) + " TIME " + to_string(long double(int(world->playTime.getElapsedTime().asSeconds()))) + "\n" + line + "\n";
			submitted = true;
		}else{
			scorestr = scorestr + line + "\n";
		}
	};

	scorefilein.close();

	ofstream scorefileout("rsc/score.dat");
	scorefileout.clear();
	scorefileout << scorestr;

	scorefileout.close();

	if(score > oldscorefinal){
		createButton(world, WINDOW_WIDTH/2 - 155, -100, EVENT_NONE, "NEW HISCORE!", 255, 255, 255, WINDOW_WIDTH/2 - 155, 100);
		playSound(rsc, SOUND_POINT);
		playSound(rsc, SOUND_POWERUP);
	}
}

void doHiscore(World *world, Rsc *rsc){
	if(checkGameEvent(rsc, EVENT_PLAYER_DEAD)){
		int wave = 0, score = 0;

		for(Entity entity = 0; entity < ENTITY_MAX; entity++){
			if(checkMask(world, entity, COMPONENT_COUNTER | COMPONENT_TEXT)){
				if(world->text[entity] == "SCORE")
					score = world->counter[entity].value;
				else if(world->text[entity] == "WAVE")
					wave = world->counter[entity].value;
			}
		}

		if(wave || score)
			submitHiscore(world, rsc, score, wave);
	}
}


void applyVelocity(World *world, Rsc *rsc, Entity entity, float deltatime){
	world->position[entity].x += world->velocity[entity].x * deltatime;
	world->position[entity].y += world->velocity[entity].y * deltatime;
}
void applyFriction(World *world, Rsc *rsc, Entity entity){
	const float FRICTION = 0.90f;
	
	if((world->mask[entity] & COMPONENT_FRICTION) == COMPONENT_FRICTION){
		world->velocity[entity].x *= FRICTION;
		world->velocity[entity].y *= FRICTION;	
	}
}


void emitParticles(World *world, Rsc *rsc, Entity entity){
	float randomfacx = getRandom(world->emitter[entity].size)-(world->emitter[entity].size/2.0f);
	float randomfacy = getRandom(world->emitter[entity].size)-(world->emitter[entity].size/2.0f);

	float x = world->position[entity].x;
	float y = world->position[entity].y;

	if(checkMask(world, entity, COMPONENT_ROTATION)){
		x += (-cos(-world->rotation[entity].angle*PI/180)*world->emitter[entity].offset);
		y += (sin(-world->rotation[entity].angle*PI/180)*world->emitter[entity].offset);
	}

	if(!checkMask(world, entity, COMPONENT_VELOCITY) || (abs(world->velocity[entity].x) > 1 || abs(world->velocity[entity].y) > 1)){
		switch(world->emitter[entity].type){
		//Player Flame
		case 0:
			if(getRandom(5) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 225, 0,   float(getRandom(360)), 2);
			if(getRandom(4) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 0, 0,     float(getRandom(360)), 2);
			if(getRandom(3) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 70, 0,   float(getRandom(360)), 1);
			
			break;
		//Duller Flame
		case 1:
			if(getRandom(18) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 225, 0,   float(getRandom(360)), 2);
			if(getRandom(17) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 0, 0,     float(getRandom(360)), 2);
			if(getRandom(16) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 70, 0,   float(getRandom(360)), 1);
			
			break;
		//Explosion
		case 2:
			if(getRandom(1) == 0)
				createParticle(world, x + randomfacx*3, y + randomfacy*3, 225, 225, 0,   float(getRandom(360)), 7);
			if(getRandom(2) == 0)
				createParticle(world, x + randomfacx*3, y + randomfacy*3, 225, 0, 0,     float(getRandom(360)), 6);
			if(getRandom(1) == 0)
				createParticle(world, x + randomfacx*3, y + randomfacy*3, 225, 70, 0,    float(getRandom(360)), 7);
			if(true)
				createParticle(world, x + randomfacx*6, y + randomfacy*6, getRandom(255), getRandom(255), getRandom(255),
				float(getRandom(360)), 3);
			break;
		//Bullet path
		case 3:
			if(getRandom(18) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 100, 100,   float(getRandom(360)), 0.5f);
			if(getRandom(17) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 0, 0,       float(getRandom(360)), 0.5f);
			if(getRandom(16) == 0)
				createParticle(world, x + randomfacx, y + randomfacy, 225, 200, 200,   float(getRandom(360)), 0.5f);
			break;
		//Item Firework
		case 4:
			if(getRandom(1) == 0)
				createParticle(world, x + randomfacx*3, y + randomfacy*3, 225, 225, 0,      float(getRandom(360)), 7);
			if(getRandom(2) == 0)
				createParticle(world, x + randomfacx*3, y + randomfacy*3, 225, 0, 255,        float(getRandom(360)), 6);
			if(getRandom(1) == 0)
				createParticle(world, x + randomfacx*3, y + randomfacy*3, 225, 255, 255,    float(getRandom(360)), 7);
			if(true)
				createParticle(world, x + randomfacx*6, y + randomfacy*6, getRandom(255), getRandom(255), getRandom(255),
				float(getRandom(360)), 3);
			break;
		}
	}
}


void transitionEntity(World *world, Rsc *rsc, Entity entity, float deltatime){
	world->position[entity].x += (world->transition[entity].target.x - world->position[entity].x)*deltatime;
	world->position[entity].y += (world->transition[entity].target.y - world->position[entity].y)*deltatime;
}


void accumulatePoints(Rsc *rsc){
	static int pointbuffer = 0;
	static Clock delay;

	if(checkGameEvent(rsc, EVENT_ADD_POINTS_1))
		pointbuffer += 1;
	if(checkGameEvent(rsc, EVENT_ADD_POINTS_2))
		pointbuffer += 2;
	if(checkGameEvent(rsc, EVENT_ADD_POINTS_5))
		pointbuffer += 5;
	if(checkGameEvent(rsc, EVENT_ADD_POINTS_10))
		pointbuffer += 10;

	if(pointbuffer > 0 && delay.getElapsedTime().asSeconds() >= 0.05f){
		triggerGameEvent(rsc, EVENT_ADD_POINT);
		playSound(rsc, SOUND_POINT);
		pointbuffer -= 1;
		delay.restart();
	}
}
void drawDigit(Rsc *rsc, char digit, float x, float y){
	switch(digit){
	case '0':
		drawGraphic(rsc, GRAPHIC_0, x, y, 0);
		break;
	case '1':
		drawGraphic(rsc, GRAPHIC_1, x, y, 0);
		break;
	case '2':
		drawGraphic(rsc, GRAPHIC_2, x, y, 0);
		break;
	case '3':
		drawGraphic(rsc, GRAPHIC_3, x, y, 0);
		break;
	case '4':
		drawGraphic(rsc, GRAPHIC_4, x, y, 0);
		break;
	case '5':
		drawGraphic(rsc, GRAPHIC_5, x, y, 0);
		break;
	case '6':
		drawGraphic(rsc, GRAPHIC_6, x, y, 0);
		break;
	case '7':
		drawGraphic(rsc, GRAPHIC_7, x, y, 0);
		break;
	case '8':
		drawGraphic(rsc, GRAPHIC_8, x, y, 0);
		break;
	case '9':
		drawGraphic(rsc, GRAPHIC_9, x, y, 0);
		break;
	}
}

void runCounter(World *world, Rsc *rsc, Entity entity){
	if(checkGameEvent(rsc, world->counter[entity].trigger))
		world->counter[entity].value++;

	if(world->text[entity] == "HP"){
		Entity player = findPlayer(world);

		world->counter[entity].value = int(world->health[player].value / world->health[player].max * 100);
	}
	if(world->text[entity] == "WAVE"){
		if(checkGameEvent(rsc, EVENT_WAVE_END))
			world->transition[entity].target.y = 100;
		else if(checkGameEvent(rsc, EVENT_WAVE_START))
			world->transition[entity].target.y = -100;
	}
}
void drawCounter(World *world, Rsc *rsc, Entity entity, float xoffset){
	float x = world->position[entity].x;
	float y = world->position[entity].y;

	unsigned int value = world->counter[entity].value;

	string digits = to_string(long double(value));

	for(unsigned int i = 0; i < digits.length(); i++)
		drawDigit(rsc, digits[i], x + xoffset + 32*i,  y);

	if(world->text[entity] == "HP")
		drawGraphic(rsc, GRAPHIC_PCT, x + xoffset + 5 + 32*digits.length(), y, 0);
}


void explode(World *world, Rsc *rsc, Entity entity){
	for(unsigned int eentity = 0; eentity <= ENTITY_MAX; eentity++){
		if(checkCollision(world, entity, eentity, world->explosive[entity].radius) &&
		   checkMask(world, eentity, COMPONENT_POSITION | COMPONENT_HEALTH) &&
		  !checkMask(world, eentity, COMPONENT_TEXT))

		  	if(world->explosive[entity].killsenemies)
				deductHealth(world, rsc, eentity, world->explosive[entity].damage);
			else if(!checkMask(world, eentity, COMPONENT_AI_SUICIDE)  &&
					!checkMask(world, eentity, COMPONENT_AI_AVOIDING) &&
					!checkMask(world, eentity, COMPONENT_AI_SHOOTING))
				deductHealth(world, rsc, eentity, world->explosive[entity].damage);
	}

	deductHealth(world, rsc, entity, 100000);
	playSound(rsc, SOUND_HIT);
}
void explodeOnImpact(World *world, Rsc *rsc, Entity entity){
	//loop through all entities and find one to collide with
	for(Entity centity = 0; centity <= ENTITY_MAX; centity++){
		if(checkMask(world, centity, COMPONENT_POSITION | COMPONENT_COLLISION | COMPONENT_HEALTH) &&
		  !checkMask(world, centity, COMPONENT_TEXT)	  &&
		   checkCollision(world, entity, centity) && centity != entity){

			if(world->explosive[entity].killsenemies)
				explode(world, rsc, entity);
			else if(!checkMask(world, centity, COMPONENT_AI_SUICIDE)  &&
					!checkMask(world, centity, COMPONENT_AI_AVOIDING) &&
					!checkMask(world, centity, COMPONENT_AI_SHOOTING))
				explode(world, rsc, entity);

			break;
		}
	}
};


void onClick(World *world, Rsc *rsc, Entity entity){
	float width = 32;
	float height = 32;

	if(checkMask(world, entity, COMPONENT_TEXT))
		width *= world->text[entity].length();

	if(Mouse::isButtonPressed(Mouse::Left) && world->click[entity].prevClick == false){
		if(Mouse::getPosition(*rsc->window).x < world->position[entity].x + width &&
		   Mouse::getPosition(*rsc->window).x > world->position[entity].x - width &&
		   Mouse::getPosition(*rsc->window).y < world->position[entity].y + height &&
		   Mouse::getPosition(*rsc->window).y > world->position[entity].y - height &&
		   world->click[entity].event != EVENT_NONE){
			triggerGameEvent(rsc, world->click[entity].event);
			playSound(rsc, SOUND_CLICK);
			world->click[entity].prevClick = true;
		}
	}
	else if(!Mouse::isButtonPressed(Mouse::Left) && world->click[entity].prevClick){
		  if(Mouse::getPosition(*rsc->window).x < world->position[entity].x + width &&
		     Mouse::getPosition(*rsc->window).x > world->position[entity].x - width &&
		     Mouse::getPosition(*rsc->window).y < world->position[entity].y + height &&
		     Mouse::getPosition(*rsc->window).y > world->position[entity].y - height )
			world->click[entity].prevClick = false;
	}
}


void doPlayerHealth(World *world, Rsc *rsc, float deltatime){
	Entity player = findPlayer(world);

	if(world->health[player].value < world->health[player].max && world->position[player].y < 10000)
	   world->health[player].value += 0.1f*deltatime;
}

void playerMovement(World *world, Rsc *rsc, Entity player){
	float *velx = &world->velocity[player].x;
	float *vely = &world->velocity[player].y;

	float posx = world->position[player].x;
	float posy = world->position[player].y;
	
	float speed = 20;

	float margin = 32;

	if(Keyboard::isKeyPressed(Keyboard::W) && posy > 0 + margin)
		*vely = -speed;

	if(Keyboard::isKeyPressed(Keyboard::S) && posy < WINDOW_HEIGHT - margin)
		*vely = speed;

	if(Keyboard::isKeyPressed(Keyboard::A) && posx > 0 + margin)
		*velx = -speed;

	if(Keyboard::isKeyPressed(Keyboard::D) && posx < WINDOW_WIDTH - margin)
		*velx = speed;
}
void playerShoot(World *world, Rsc *rsc, Entity player){
	float *posx = &world->position[player].x;
	float *posy = &world->position[player].y;

	float *rotation = &world->rotation[player].angle;	

	float vx = Mouse::getPosition(*rsc->window).x - *posx;
	float vy = Mouse::getPosition(*rsc->window).y - *posy;
	
	float length = sqrt(vx*vx + vy*vy);
	
	vx = vx/length;
	vy = vy/length;

	*rotation = atan2(vy,vx)*180/PI;

	static Clock rocketBoostTimer;
	if(rocketBoostTimer.getElapsedTime().asSeconds() > 5.0f && rocketBoostTimer.getElapsedTime().asSeconds() < 6.0f)
		world->controlled[player].rocketBoost = false;
	else if(rocketBoostTimer.getElapsedTime().asSeconds() > 6.0f && world->controlled[player].rocketBoost == true)
		rocketBoostTimer.restart();

	static Clock bulletCoolDown;
	if(Mouse::isButtonPressed(Mouse::Left) && bulletCoolDown.getElapsedTime().asSeconds() > 0.2 && *posy < 10000){
		if(world->controlled[player].rocketBoost == false){
			createBullet(world, *posx, *posy, vx, vy, *rotation, true);
			playRandomSound(rsc, SOUNDCAT_LASER, getRandom(2));
		}
		else{
			createRocket(world, *posx, *posy, vx, vy, *rotation, true);
			playRandomSound(rsc, SOUNDCAT_ROCKET, getRandom(2));
		}

		bulletCoolDown.restart();
	}

	static Clock rocketCoolDown;
	if(Mouse::isButtonPressed(Mouse::Right) && rocketCoolDown.getElapsedTime().asSeconds() > 2.5 && *posy < 10000){
		createRocket(world, *posx, *posy, vx, vy, *rotation, true);
		playRandomSound(rsc, SOUNDCAT_ROCKET, getRandom(2));
		rocketCoolDown.restart();
	}
}


void moveToPoint(World *world, Rsc *rsc, Entity entity, float posx, float posy, float speed){
	float *ex = &world->position[entity].x;
	float *ey = &world->position[entity].y;

	float *velx = &world->velocity[entity].x;
	float *vely = &world->velocity[entity].y;

	float dirx = posx - *ex;
	float diry = posy - *ey;

	//normalize the direction
	dirx /= sqrt(dirx*dirx + diry*diry);
	diry /= sqrt(dirx*dirx + diry*diry);

	*velx = dirx * speed;
	*vely = diry * speed;
}

void doEnemyMotion(World *world, Rsc *rsc, Entity entity, float stoprange, float deltatime){
	float playerx = 0;
	float playery = 0;

	if(world->ai[entity].target <= ENTITY_MAX){
		playerx =  world->position[world->ai[entity].target].x;
		playery =  world->position[world->ai[entity].target].y;
	}

	float ex = world->position[entity].x;
	float ey = world->position[entity].y;
	
	if(!checkCollision(world, entity, world->ai[entity].target, stoprange))
		moveToPoint(world, rsc, entity, playerx, playery, world->ai[entity].movespeed);

	else if(checkMask(world, entity, COMPONENT_AI_SUICIDE))
		world->explosive[entity].delay = 0.1f;
		
	float targetdir = atan2(playery - ey,playerx - ex)*180/PI;

	world->rotation[entity].angle = targetdir;
}
void doEnemyShooting(World *world, Rsc *rsc, Entity entity, float deltatime){
	if(world->ai[entity].timer <= 0 && checkCollision(world, entity, world->ai[entity].target, 10)){
		world->ai[entity].timer = 10.0f + getRandom(50);

		createBullet(world, world->position[entity].x, world->position[entity].y, 
			 cos(world->rotation[entity].angle*PI/180), sin(world->rotation[entity].angle*PI/180), world->rotation[entity].angle, false);

		playRandomSound(rsc, SOUNDCAT_LASER, getRandom(2));
	}

	if(world->ai[entity].timer > 0)
		world->ai[entity].timer -= 1*deltatime;
}
void doEnemyRockets(World *world, Rsc *rsc, Entity entity, float deltatime){
	if(world->ai[entity].timer <= 0 && checkCollision(world, entity, world->ai[entity].target, 10)){
		world->ai[entity].timer = 15.0f + getRandom(50);

		createRocket(world, world->position[entity].x, world->position[entity].y, 
			 cos(world->rotation[entity].angle*PI/180), sin(world->rotation[entity].angle*PI/180), world->rotation[entity].angle, false);

		playRandomSound(rsc, SOUNDCAT_ROCKET, getRandom(2));
	}

	if(world->ai[entity].timer > 0)
		world->ai[entity].timer -= 1*deltatime;
}

void doEnemyAI(World *world, Rsc *rsc, Entity entity, float deltatime){
	if(checkMask(world, entity, COMPONENT_AI_SUICIDE))
		doEnemyMotion(world, rsc, entity, 0, deltatime);
	else if(checkMask(world, entity, COMPONENT_AI_AVOIDING))
		doEnemyMotion(world, rsc, entity, 2, deltatime);

	if(checkMask(world, entity, COMPONENT_AI_SHOOTING))
		doEnemyShooting(world, rsc, entity, deltatime);
	if(checkMask(world, entity, COMPONENT_AI_ROCKETS))
		doEnemyRockets(world, rsc, entity, deltatime);
}


void doTemplateColor(World *world, Rsc *rsc, Entity entity){
	float vx = Mouse::getPosition(*rsc->window).x - world->position[entity].x;
	float vy = Mouse::getPosition(*rsc->window).y - world->position[entity].y;
	
	float length = sqrt(vx*vx + vy*vy);
	
	vx = vx/length;
	vy = vy/length;

	world->rotation[entity].angle = atan2(vy,vx)*180/PI;

	int additionFac = 30;

	int r = world->color[entity].r;
	int g = world->color[entity].g;
	int b = world->color[entity].b;

	if(checkGameEvent(rsc, EVENT_COLOR_RUP))
		r += additionFac;
	if(checkGameEvent(rsc, EVENT_COLOR_RDOWN))
		r -= additionFac;

	if(checkGameEvent(rsc, EVENT_COLOR_GUP))
		g += additionFac;
	if(checkGameEvent(rsc, EVENT_COLOR_GDOWN))
		g -= additionFac;

	if(checkGameEvent(rsc, EVENT_COLOR_BUP))
		b += additionFac;
	if(checkGameEvent(rsc, EVENT_COLOR_BDOWN))
		b -= additionFac;

	if(r > 255)
		r = 255;
	else if(r < additionFac)
		r = additionFac;
	if(g > 255)
		g = 255;
	else if(g < additionFac)
		g = additionFac;
	if(b > 255)
		b = 255;
	else if(b < additionFac)
		b = additionFac;

	world->color[entity].r = r;
	world->color[entity].g = g;
	world->color[entity].b = b;

	world->playerColor = world->color[entity];
}


void drawText(World *world, Rsc *rsc, Entity entity){
	string text = world->text[entity];

	float yoffset = 0;
	float xoffset = 0;

	for(unsigned int i = 0; i < text.length(); i++){
		if(text[i] != '<' && text[i] != '>' && text[i] != '!' && text[i] != '#'){
			int letter = int(text[i]) - 64;

			if(letter <= 26 && letter > 0){
				if(checkMask(world, entity, COMPONENT_COLOR))
					setGraphicColor(rsc, Graphic(letter), world->color[entity].r, world->color[entity].g, world->color[entity].b);

				drawGraphic(rsc, Graphic(letter), world->position[entity].x + (i*32) + xoffset, world->position[entity].y + yoffset, 0);
			}
			else if(text[i] >= '0' && text[i] <= '9'){
				letter += 16 + 27;

				if(checkMask(world, entity, COMPONENT_COLOR))
					setGraphicColor(rsc, Graphic(letter), world->color[entity].r, world->color[entity].g, world->color[entity].b);

				drawGraphic(rsc, Graphic(letter), world->position[entity].x + (i*32) + xoffset, world->position[entity].y + yoffset, 0);
			}
		}
		else if(text[i] == '<'){
			if(checkMask(world, entity, COMPONENT_COLOR))
				setGraphicColor(rsc, GRAPHIC_LARROW, world->color[entity].r, world->color[entity].g, world->color[entity].b);

			drawGraphic(rsc, GRAPHIC_LARROW, world->position[entity].x + (i*32) + xoffset, world->position[entity].y + yoffset, 0);
		}
		else if(text[i] == '>'){
			if(checkMask(world, entity, COMPONENT_COLOR))
				setGraphicColor(rsc, GRAPHIC_RARROW, world->color[entity].r, world->color[entity].g, world->color[entity].b);

			drawGraphic(rsc, GRAPHIC_RARROW, world->position[entity].x + (i*32) + xoffset, world->position[entity].y + yoffset, 0);
		}
		else if(text[i] == '!'){
			if(checkMask(world, entity, COMPONENT_COLOR))
				setGraphicColor(rsc, GRAPHIC_XPT, world->color[entity].r, world->color[entity].g, world->color[entity].b);

			drawGraphic(rsc, GRAPHIC_XPT, world->position[entity].x + (i*32) + xoffset, world->position[entity].y + yoffset, 0);
		}
		else if(text[i] == '#'){
			yoffset += 56;
			xoffset = -float(i)*32;
		}
	}

	if(checkMask(world, entity, COMPONENT_COUNTER))
		drawCounter(world, rsc, entity, text.length()*32.0f + 20);
}


void doPowerUp(World *world, Rsc *rsc, Entity entity){
	Entity player = findPlayer(world);
	string popupText;

	if(checkCollision(world, player, entity)){
		switch(world->appearance[entity].id){
		case GRAPHIC_HPUP:
			world->health[player].value += 10;
			popupText = "HEALTH UP!";
			break;
		case GRAPHIC_RBOOST:
			world->controlled[player].rocketBoost = true;
			popupText = "ROCKET BOOST!";
			break;
		case GRAPHIC_BOMB:
			for(Entity enemy = 0; enemy < ENTITY_MAX; enemy++){
				if(checkMask(world, enemy, COMPONENT_AI_AVOIDING) ||
				   checkMask(world, enemy, COMPONENT_AI_SUICIDE))
				   deductHealth(world, rsc, enemy, 1000);
			}
			popupText = "BOMB!";

			if(!getRandom(10))
				popupText = "LOVE THE BOMB!";
			break;
		}

		createFireWork(world, world->position[entity].x, world->position[entity].y);
		createPopup(world, WINDOW_WIDTH/2-popupText.length()*16.0f, -100, popupText, 255, 255, 255, WINDOW_WIDTH/2-popupText.length()*16.0f, WINDOW_HEIGHT/2-100);
		playSound(rsc, SOUND_POWERUP);
		destroyEntity(world, entity);
	}
}


void renderEntity(World *world, Rsc *rsc, Entity entity){
	if(checkMask(world, entity, COMPONENT_COLOR))
		setGraphicColor(rsc, world->appearance[entity].id, world->color[entity].r, world->color[entity].g, world->color[entity].b);

	if(checkMask(world, entity, COMPONENT_LASER | COMPONENT_ROTATION))
		return;
	else if(checkMask(world, entity, COMPONENT_COUNTER)){
		drawCounter(world, rsc, entity, 30);
		drawGraphic(rsc, world->appearance[entity].id, world->position[entity].x, world->position[entity].y, 0);
	}
	else if(checkMask(world, entity, COMPONENT_ROTATION))
		drawGraphic(rsc, world->appearance[entity].id, world->position[entity].x, world->position[entity].y, world->rotation[entity].angle);
	else
		drawGraphic(rsc, world->appearance[entity].id, world->position[entity].x, world->position[entity].y, 0);
}


void spawnEnemies(World *world, unsigned int difficulty, Entity target){
	for(unsigned int i = 0; i < 1+difficulty+getRandom(3); i++){
		const float offset = 50;
		float x, y;
		
		switch(getRandom(3)){
		case 0:
			x = -offset;
			y = float(getRandom(WINDOW_HEIGHT));
			break;
		
		case 1:
			x = WINDOW_WIDTH + offset;
			y = float(getRandom(WINDOW_HEIGHT));
			break;
	
		case 2:
			x = float(getRandom(WINDOW_WIDTH));
			y = -offset;
			break;

		case 3:
			x = float(getRandom(WINDOW_WIDTH));
			y = WINDOW_HEIGHT + offset;
			break;
		}

		unsigned int rand = getRandom(6);

		if(rand == 6 || rand == 5)
			createEnemyTank(world, x, y, difficulty, target);
		else if(rand == 4)
			createEnemyBomber(world, x, y, difficulty, target);
		else
			createEnemyShooter(world, x, y, difficulty, target);
	}
}
void spawnPowerUps(World *world){
	if(!getRandom(2)){
		if(!getRandom(4))
			createPowerUp(world, float(getRandom(WINDOW_WIDTH)), float(getRandom(WINDOW_HEIGHT)), GRAPHIC_BOMB);
	
		if(!getRandom(2))
			createPowerUp(world, float(getRandom(WINDOW_WIDTH)), float(getRandom(WINDOW_HEIGHT)), GRAPHIC_RBOOST);

		if(!getRandom(1))
			createPowerUp(world, float(getRandom(WINDOW_WIDTH)), float(getRandom(WINDOW_HEIGHT)), GRAPHIC_HPUP);
	}
}

void handleWave(World *world, Rsc *rsc){
	bool hasEnemies = false;
	static Clock wavedelay;

	for(Entity entity = 0; entity < ENTITY_MAX; entity++){
		if(checkMask(world, entity, COMPONENT_AI_SUICIDE)  ||
		   checkMask(world, entity, COMPONENT_AI_AVOIDING) ||
		   checkMask(world, entity, COMPONENT_AI_SHOOTING)){
			hasEnemies = true;
			break;
		}
	}

	if(!hasEnemies && world->wave != 0){
		triggerGameEvent(rsc, EVENT_WAVE_END);
		
		if(wavedelay.getElapsedTime().asSeconds() > 2){
			Entity player = findPlayer(world);

			spawnEnemies(world, unsigned int(world->wave/0.75f), player);
			spawnPowerUps(world);

			triggerGameEvent(rsc, EVENT_WAVE_START);
			world->wave++;
		}	
	}	
	else
		wavedelay.restart();

	if(world->wave == 0)
		world->playTime.restart();
}


void handleSystems(World *world, Rsc *rsc){
	static Clock frameTime;

	float timeElapsed = frameTime.restart().asSeconds() * 10;
	
	tileBackground(world, rsc);

	for(Entity entity = 0; entity < ENTITY_MAX; entity++){
		//Move
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_VELOCITY)){
			applyVelocity(world, rsc, entity, timeElapsed);
			applyFriction(world, rsc, entity);
		}

		//Counter
		if(checkMask(world, entity, COMPONENT_COUNTER | COMPONENT_POSITION))
			runCounter(world, rsc, entity);	

		//Explosion
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_COLLISION | COMPONENT_EXPLOSIVE))
			if(world->explosive[entity].delay <= 0)
				explodeOnImpact(world, rsc, entity);
			else
				world->explosive[entity].delay -= timeElapsed;

		//Click
		if(checkMask(world, entity, COMPONENT_CLICK | COMPONENT_POSITION))
			onClick(world, rsc, entity);
	
		//Killtime
		if(checkMask(world, entity, COMPONENT_KILLTIME)){
			if(world->killtime[entity].value > 0)
				world->killtime[entity].value -= timeElapsed;
			else
				destroyEntity(world, entity);
		}

		//Particle Emission
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_EMITTER))
			emitParticles(world, rsc, entity);

		//Player
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_CONTROLLED)){
			playerMovement(world, rsc, entity);
			playerShoot(world, rsc, entity);
		}

		//Enemy
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_VELOCITY))
			doEnemyAI(world, rsc, entity, timeElapsed);

		//ColorTemplates
		if(checkMask(world, entity, COMPONENT_COLOR | COMPONENT_COLORTEMPLATE))
			doTemplateColor(world, rsc, entity);

		//Transition
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_TRANSITION))
			transitionEntity(world, rsc, entity, timeElapsed);
		
		//PowerUps
		if(checkMask(world, entity, COMPONENT_COLLISION | COMPONENT_POWERUP))
			doPowerUp(world, rsc, entity);
		

		//Render everything unimportant
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_APPEARANCE) &&
		  !checkMask(world, entity, COMPONENT_CONTROLLED) &&
		  !checkMask(world, entity, COMPONENT_AI_AVOIDING)&&
		  !checkMask(world, entity, COMPONENT_AI_SUICIDE) &&
		  !checkMask(world, entity, COMPONENT_TEXT))
			renderEntity(world, rsc, entity);
	}

	//Render all important entites above everything
	for(Entity entity = 0; entity < ENTITY_MAX; entity++){
		if(checkMask(world, entity, COMPONENT_POSITION | COMPONENT_APPEARANCE) &&
		  (checkMask(world, entity, COMPONENT_CONTROLLED) ||
		   checkMask(world, entity, COMPONENT_AI_AVOIDING)||
		   checkMask(world, entity, COMPONENT_AI_SUICIDE)))
			renderEntity(world, rsc, entity);
	}
	for(Entity entity = 0; entity < ENTITY_MAX; entity++){
		if(checkMask(world, entity, COMPONENT_TEXT))
				drawText(world, rsc, entity);
	}

	//shake the screen when the player is damaged
	screenShake(world, rsc, timeElapsed);

	//regen player health and set hp counter
	doPlayerHealth(world, rsc, timeElapsed);

	//if the player died, find/submit highscores
	doHiscore(world, rsc);

	accumulatePoints(rsc);
	
	rsc->window->display();
	rsc->window->clear();
}
