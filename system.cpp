#include"system.h"

#define MOVEMENT_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY)

void handleMovement(World *world){
	//calculate time elapsed from last update
	static Clock frameTime;
	float timeElapsed = frameTime.restart().asSeconds() * 10;

	const float friction = 0.995f;

	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & MOVEMENT_MASK) == MOVEMENT_MASK){	
			world->position[entity].x += world->velocity[entity].x * timeElapsed;
			world->position[entity].y += world->velocity[entity].y * timeElapsed;

			//apply friction
			if((world->mask[entity] & COMPONENT_FRICTION) == COMPONENT_FRICTION){
				world->velocity[entity].x *= friction;
				world->velocity[entity].y *= friction;	
			}

			//if the entity is out of bounds and unimportant, destroy it
			if((world->mask[entity] & COMPONENT_CONTROLLED) != COMPONENT_CONTROLLED &&
			   (world->mask[entity] & COMPONENT_AI)			!= COMPONENT_AI			&&
			   (0 > world->position[entity].x || world->position[entity].x > WINDOW_WIDTH) &&
			   (0 > world->position[entity].y || world->position[entity].y > WINDOW_HEIGHT)){
				  
				   destroyEntity(world, entity);
			}
		}

		if((world->mask[entity] & COMPONENT_EXPLOSIVE) == COMPONENT_EXPLOSIVE){
			//boom bitch
		}
	}
}

#define RENDER_MASK (COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ROTATION)

void handleRender(World *world, Rsc *rsc, RenderWindow *window){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & RENDER_MASK) == RENDER_MASK){
			drawGraphic(rsc, world->appearance[entity].id, world->position[entity].x, world->position[entity].y, world->rotation[entity].angle, window);
		}
		else if((world->mask[entity] & RENDER_MASK) == (COMPONENT_POSITION | COMPONENT_APPEARANCE)){
			drawGraphic(rsc, world->appearance[entity].id, world->position[entity].x, world->position[entity].y, 0, window);
		}
		else if((world->mask[entity] & COMPONENT_APPEARANCE) == COMPONENT_APPEARANCE){
			drawGraphic(rsc, world->appearance[entity].id, 0, 0, 0, window);
		}
	}

	window->display();
	window->clear();
}

void handleAudio(World *world, Rsc *rsc){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & COMPONENT_AUDIO) == COMPONENT_AUDIO &&
			world->audio[entity].on){
			//world->audioshort[entity].play.play();
		}
	}
}

void handleClick(World *world, RenderWindow *window){
	static bool prevClick = false;

	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & COMPONENT_CLICK) == COMPONENT_CLICK &&
			Mouse::isButtonPressed(Mouse::Left) && prevClick == false){
			if(Mouse::getPosition(*window).x < world->position[entity].x + 64 &&
			   Mouse::getPosition(*window).x > world->position[entity].x - 64 &&
			   Mouse::getPosition(*window).y < world->position[entity].y + 64 &&
			   Mouse::getPosition(*window).y > world->position[entity].y - 64 ){
				triggerGameEvent(world, world->click[entity].event);
				prevClick = true;
			}
		}else if(!Mouse::isButtonPressed(Mouse::Left) && prevClick){
			if(Mouse::getPosition(*window).x < world->position[entity].x + 64 &&
			   Mouse::getPosition(*window).x > world->position[entity].x - 64 &&
			   Mouse::getPosition(*window).y < world->position[entity].y + 64 &&
			   Mouse::getPosition(*window).y > world->position[entity].y - 64 )
				prevClick = false;
		}
	}
}

#define PLAYER_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_CONTROLLED)

void handlePlayer(World *world, RenderWindow *window){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & PLAYER_MASK) == PLAYER_MASK){
			float *velx = &world->velocity[entity].x;
			float *vely = &world->velocity[entity].y;

			float *posx = &world->position[entity].x;
			float *posy = &world->position[entity].y;

			float *rotation = &world->rotation[entity].angle;

			float speed = 20;

			float vx = Mouse::getPosition(*window).x - *posx;
			float vy = Mouse::getPosition(*window).y - *posy;

			float length = sqrt(vx*vx + vy*vy);
			vx = vx/length;
			vy = vy/length;

			*rotation = atan2(vy,vx)*180/PI;
		
			if(Keyboard::isKeyPressed(Keyboard::W))
				*vely = -speed;

			if(Keyboard::isKeyPressed(Keyboard::S))
				*vely = speed;

			if(Keyboard::isKeyPressed(Keyboard::A))
				*velx = -speed;

			if(Keyboard::isKeyPressed(Keyboard::D))
				*velx = speed;

			static Clock bulletCoolDown;
			if(Mouse::isButtonPressed(Mouse::Left) && bulletCoolDown.getElapsedTime().asSeconds() > 0.25f){
				createBullet(world, *posx, *posy, vx, vy, *rotation);
				bulletCoolDown.restart();
			}
			
			static Clock rocketCoolDown;
			if(Mouse::isButtonPressed(Mouse::Right) && rocketCoolDown.getElapsedTime().asSeconds() > 3){
				createRocket(world, *posx, *posy, vx, vy, *rotation);
				rocketCoolDown.restart();
			}
		}
	}
}

#define ENEMY_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_AI)

void handleEnemy(World *world){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & ENEMY_MASK) == ENEMY_MASK){
			//perform action
		}
	}
}

#define EXPLOSIVE_MASK (COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_EXPLOSIVE | COMPONENT_COLLISION)
#define COLLISION_MASK (COMPONENT_POSITION | COMPONENT_COLLISION)

void handleExplosion(World *world){
	for(unsigned int entity = 0; entity < ENTITY_MAX; entity++){
		if((world->mask[entity] & EXPLOSIVE_MASK) == EXPLOSIVE_MASK){
			//loop through all entities and find one to collide with
			for(unsigned int centity = 0; centity < ENTITY_MAX; centity++){
				if(centity != entity && (world->mask[centity] & COLLISION_MASK) == COLLISION_MASK){

					float distX = world->position[entity].x - world->position[centity].x;
					float distY = world->position[entity].y - world->position[centity].y;

					float rad1 = world->collision[entity].radius;
					float rad2 = world->collision[centity].radius;

					//if collided, loop AGAIN and damage everything inside blast radius
					if((distX*distX) + (distY*distY) <= (rad1+rad2) * (rad1+rad2)){
						for(unsigned int eentity = 0; eentity < ENTITY_MAX; eentity++){
							if((world->mask[eentity] & (COLLISION_MASK | COMPONENT_HEALTH)) == (COLLISION_MASK | COMPONENT_HEALTH)){
								distX = world->position[entity].x - world->position[eentity].x;
								distY = world->position[entity].y - world->position[eentity].y;
								
								rad1 = world->explosive[entity].radius;
								rad2 = world->collision[eentity].radius;

								if((distX*distX) + (distY*distY) <= (rad1+rad2) * (rad1+rad2)){
										world->health[eentity].value -= world->explosive[entity].damage;

										if(world->health[eentity].value <= 0)
											destroyEntity(world, eentity);
								}
							}
						}
						destroyEntity(world, entity);
						break;
					}
				}
			}
		}
	}
}