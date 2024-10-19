#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <unistd.h>

#include <initializer_list>
#include <functional>
#include <algorithm>
#include <iostream>
#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

using namespace std;

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

void updatePlayerPosition(entity* player, const int newX, const int newY){
	player->x = newX;
	player->y = newY;
}

void updatePlayerXP(entity* player, const float xpMult) {
	const float oldXp = player->xp;
	player->xp = oldXp * xpMult;
}

int getRandom(int max){	
	return rand() % max + 0;  
}

void seed(entity entities[], const int numEntities){
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

void generateEvents(event events[], int numEvents, entity entities[], const int numEntities){
	for(int i = 0; i < numEvents; i++){
		const int randomTo = getRandom(numEntities);
		const int randomFrom = getRandom(numEntities);

		events[i].type = ATTACK;	
		events[i].attackDmg = static_cast<float>(getRandom(50));
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
	srand(time(nullptr));   // Initialization, should only be called once.
	constexpr int NUM_ENTITIES = 5;
	entity entities[NUM_ENTITIES];

	int world[10][10];

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			world[i][j] = 0;
		}
	}
	seed(entities, NUM_ENTITIES);
	int gameTicks = 0;
	bool breakLoop = false;

	while(!breakLoop){
		constexpr int NUM_EVENTS = 5;
		event events[NUM_EVENTS];
		generateEvents(events, NUM_EVENTS, entities, NUM_ENTITIES);
		updateWorld(entities, NUM_ENTITIES, events, NUM_EVENTS);
		render(world, entities, NUM_ENTITIES);
		gameTicks++;
	}

	return 0;
}
