#include "SpaceInvaders.h"
#include "../Video/video.h"
#include "../Keyboard/kb.h"
#include "../Kernel/rand.h"

static unsigned char lastData;
unsigned char doMove;
unsigned int lastCount;

void siCreateGame(siGame* game){
	doMove = 0; /* BECAUSE IT WILL BE SWITCHED TO 1 AT FIRST ITERATION */
	kprint("Creating Space Invaders...");
	lastData = 0;
	lastCount = 32;
	int x;
	for(x = 0; x < 32; x++){
		game->enemies[x].posx = (x % 8)+4;
		game->enemies[x].posy = (x / 8);
		game->enemies[x].alive = 1;
		game->enemies[x].firing = 0;
		game->enemies[x].image = '#';
	}
	game->player.posx = 7;
	game->player.posy = SIZEY - 2; // 2nd to last row SIZEY - 1 would be last row
	game->player.image = '^';
	game->player.laserCount = 0;
	
	game->enemyDirectionX = DIRECTION_LEFT;
	
	for(x = 0; x < MAXPLAYERLASERS + 32; x++){
		game->lasers[x].active = 0;
		game->lasers[x].posx = 0;
		game->lasers[x].posy = 0;
		game->lasers[x].direction = DIRECTION_UP;
		game->lasers[x].image = '|';
	}
	game->playing = RUNNING;
	kprint("Done\n");
}
void siPauseGame(siGame* game){
	game->playing = PAUSED;
}

char siDoIteration(siGame* game){ // - if player lost 0 if not over + if player won
	if(doMove){
		doMove = 0;
	}else{ doMove = 1; }
	if(game->playing == RUNNING){
		// first move all lasers
		int x;
		for(x = 0; x < 32; x++){
			if(game->enemies[x].alive == 1 && rand(1, (10+(3*lastCount))) == 5){
				int p;
				for(p = MAXPLAYERLASERS; p < MAXPLAYERLASERS + 32; p++){
					if(game->lasers[p].active == 0){
						game->lasers[p].active = 1;
						game->lasers[p].posx = game->enemies[x].posx;
						game->lasers[p].posy = game->enemies[x].posy + DIRECTION_DOWN;
						game->lasers[p].direction = DIRECTION_DOWN;
						game->lasers[p].image = '.';
						break;
					}
				}
			}
		}
		for(x = 0; x < MAXPLAYERLASERS + 32; x++){
			if(game->lasers[x].active != 0){
				if(game->lasers[x].direction == DIRECTION_DOWN) game->lasers[x].posy += game->lasers[x].direction;
				else game->lasers[x].posy += (game->lasers[x].direction * 3);
				
				
				// check to see if laser killed player
				if(game->lasers[x].posx == game->player.posx && game->lasers[x].posy == game->player.posy && game->lasers[x].direction == DIRECTION_DOWN){
					return LOSS;
				}
				// check to see if laser is off screen
				if(game->lasers[x].posy == 0 || game->lasers[x].posy >= SIZEY){
					if(game->lasers[x].direction == DIRECTION_UP)
						game->player.laserCount--;
					game->lasers[x].active = 0;
				}
			}
		}
		// now move all enemies
		int doDown = 0;
		int count = 0;
		for(x = 31; x >= 0; x--){
			if(game->enemies[x].alive != 0){
				count++;
				if(doMove){
					game->enemies[x].posx += game->enemyDirectionX;
					if(game->enemies[x].posx == 0 || game->enemies[x].posx == (SIZEX - 1)){
						doDown = 1;
					}
				}
				// check if they ran into a laser
				int y;
				for(y = 0; y < MAXPLAYERLASERS + 32; y++){
					if(game->enemies[x].posx == game->lasers[y].posx && game->enemies[x].posy == game->lasers[y].posy - (DIRECTION_UP * 2) && game->lasers[y].active == 1 && game->lasers[y].direction == DIRECTION_UP){
						/* CHECK INTERMEDIATE POSITION OF LASER */
						game->enemies[x].alive = 0;
						game->lasers[y].active = 0;
						count--;
						game->player.laserCount--;
					}else if(game->enemies[x].posx == game->lasers[y].posx && game->enemies[x].posy == game->lasers[y].posy - (DIRECTION_UP) && game->lasers[y].active == 1 && game->lasers[y].direction == DIRECTION_UP){
						/* CHECK INTERMEDIATE POSITION OF LASER */
						game->enemies[x].alive = 0;
						game->lasers[y].active = 0;
						count--;
						game->player.laserCount--;
					}else if(game->enemies[x].posx == game->lasers[y].posx && game->enemies[x].posy == game->lasers[y].posy && game->lasers[y].active == 1 && game->lasers[y].direction == DIRECTION_UP){
						/* CHECK CURRENT POS OF LASER */
						game->enemies[x].alive = 0;
						game->lasers[y].active = 0;
						count--;
						game->player.laserCount--;
					}
				}
			}
		}
		if(doDown == 1){
			game->enemyDirectionX = 0 - game->enemyDirectionX;
			// move each dude down
			for(x = 0; x < 32; x++){
				if(game->enemies[x].alive != 0){
					game->enemies[x].posy += 1;
					if(game->enemies[x].posy >= (SIZEY - 2))
						return LOSS;
				}
			}
		}
		if(count == 0)
			return WIN;
		lastCount = count;
	}else{
		return INPROGRESS;
	}
	
	return INPROGRESS;
}

void siDraw(siGame* game){
	// clear screen at start
	clrscr();
	
	// draw player first
	unsigned char* videoRam = (unsigned char*)0xB8000;
	videoRam[(game->player.posy * (80 * 2)) + (game->player.posx * 2)] = game->player.image;
	videoRam[(game->player.posy * (80 * 2)) + (game->player.posx * 2) + 1] = 0x01;//(1 << 8);
	
	// draw each enemy next
	int x;
	for(x = 0; x < 32; x++){
		if(game->enemies[x].alive != 0){
			videoRam[(game->enemies[x].posy * (80 * 2)) + (game->enemies[x].posx * 2)] = game->enemies[x].image;
			videoRam[(game->enemies[x].posy * (80 * 2)) + (game->enemies[x].posx * 2) + 1] = 0x04;//(4 << 8);
		}
	}
	
	// finally draw all lasers
	for(x = 0; x < MAXPLAYERLASERS + 32; x++){
		if(game->lasers[x].active != 0){
			videoRam[(game->lasers[x].posy * (80 * 2)) + (game->lasers[x].posx * 2)] = game->lasers[x].image;
			videoRam[(game->lasers[x].posy * (80 * 2)) + (game->lasers[x].posx * 2) + 1] = 0x0F;//(15 << 8);
		}
	}
}

void siDoInput(siGame* game){
	unsigned char data = getNextKeypress(0);
	lastData = data;
	if(data == 0x48){ // up
	
	}else if(data == 0x50){ // down
	
	}else if(data == 0x4B){ // left
		game->player.posx--;
		if((char)game->player.posx < 0)
			game->player.posx = 0;
	}else if(data == 0x4D){ // right
		game->player.posx++;
		if(game->player.posx > SIZEX - 1)
			game->player.posx = SIZEX - 1;
	}else if(data == 0x39){
		if(game->player.laserCount == MAXPLAYERLASERS)
			return;
		// need to find unused laser
		unsigned char foundOne = MAXPLAYERLASERS + 32;
		int x;
		for(x = 0; x < MAXPLAYERLASERS + 32; x++){
			if(game->lasers[x].active == 0){
				foundOne = x;
				break;
			}
		}		
		if(foundOne == MAXPLAYERLASERS + 32){// found nothing, that means too many lasers exist
		
		}else{
			game->lasers[foundOne].active = 1;
			game->lasers[foundOne].posx = game->player.posx;
			game->lasers[foundOne].posy = game->player.posy + DIRECTION_UP;
			game->lasers[foundOne].direction = DIRECTION_UP;
			game->lasers[foundOne].image = '|';
			game->player.laserCount++;
		}
	}
}

