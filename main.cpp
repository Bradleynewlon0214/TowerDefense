#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <signal.h>
#include <unistd.h>

#include <initializer_list>
#include <functional>
#include <algorithm>
#include <iostream>

#define PI 3.14159


enum eventType {
	ATTACK,
};

enum entityType {
	PLAYER,
	NPC,
};

struct entity {
	entityType type;
	float playerId;
	float hp;
	float xp;
	float viewRadius; //how far the player can see
	int level;
	int x;
	int y;
};


struct event {
	eventType type;
	float attackDmg;
	entity* from;
	entity* to;
};

// entity* getEnemiesWithinView(entity* player, int* numEnemies){	
// 	int x = player->x;	
// 	int y = player->y;
//
// }

void updatePlayerPosition(entity* player, int newX, int newY){
	player->x = newX;
	player->y = newX;
}

void updatePlayerXP(entity* player, float xpMult) {
	float oldXp = player->xp;
	player->xp = oldXp * xpMult;
}

int getRandom(int max){	
	return rand() % max + 0;  
}

void seed(entity entities[], int numEntities){
	for(int i = 0; i < numEntities; i++){
		entities[i].x = getRandom(10);	
		entities[i].y = getRandom(10);	
		entities[i].hp = 100;
		entities[i].xp = 0;
		entities[i].viewRadius = 10;
		entities[i].level = 1;
		entities[i].type = PLAYER;	
	}
}

void generateEvents(event events[], int numEvents, entity entities[], int numEntities){
	for(int i = 0; i < numEvents; i++){
		int randomTo = getRandom(numEntities);
		int randomFrom = getRandom(numEntities);

		events[i].type = ATTACK;	
		events[i].attackDmg = getRandom(50);	
		events[i].to = &entities[randomTo];	
		events[i].from = &entities[randomFrom];	
	}
}

void render(int world[10][10], entity entities[], int numEntities){
	// system("clear");
	for(int i = 0; i < numEntities; i++){
		entity e = entities[i];
		int x = e.x;
		int y = e.y;
		printf("player \t hp \t xp \t level \n");
		printf("%p \t %0.f \t %0.f \t %d \n", &(entities[i]), e.hp, e.xp, e.level);
	}
}

void handlePlayerAttack(entity* playerTo, entity* playerFrom, float attackDmg){ //I dont think this is quite right
	float hp = playerTo->hp;
	// printf("player(%0.fhp): %p\t attacked player(%0.fhp) %p\t for %0.f dmg\n", hp, playerFrom, playerFrom->hp, playerTo, attackDmg);
	playerTo->hp = hp - attackDmg;	
	// printf("player(%0.fhp): %p\t now has: %0.fhp\n", playerTo->hp, playerTo, playerTo->hp);
	if(playerTo->hp <= 0){
		// printf("%p died\n", playerTo);
	}
}

void updateWorld(entity entities[], int numEntities, event events[], int numEvents){
	for(int i = 0; i <  numEvents; i++){
		event e = events[i];
		if(e.type == ATTACK){
			handlePlayerAttack(e.to, e.from, e.attackDmg);
		}
	}
}

void updatePlayers(entity entities[], int numEntities, int gameTicks){
	for(int i = 0; i < numEntities; i++){
		entity e = entities[i];
		e.xp = e.xp * 0.2;
		if(gameTicks % 10 == 0){
			e.level++;
		}
	}	
}

int main(){
	srand(time(NULL));   // Initialization, should only be called once.
	const int NUM_ENTITIES = 5;
	const int NUM_EVENTS = 5;
	entity entities[NUM_ENTITIES];
	event events[NUM_EVENTS];

	int world[10][10];

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			world[i][j] = 0;
		}
	}
	seed(entities, NUM_ENTITIES);
	int gameTicks = 0;

	while(1){	
		generateEvents(events, NUM_EVENTS, entities, NUM_ENTITIES);
		updateWorld(entities, NUM_ENTITIES, events, NUM_EVENTS);
		render(world, entities, NUM_ENTITIES);
		gameTicks++;
	}

	return 0;
}
