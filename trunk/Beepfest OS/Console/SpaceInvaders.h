#ifndef __SPACEINVADERS_H__
#define __SPACEINVADERS_H__

#define SIZEX 32
#define SIZEY 24
#define MAXPLAYERLASERS 1
#define DIRECTION_UP -1
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1
#define PAUSED 0
#define RUNNING 1
#define INPROGRESS 0
#define LOSS -1
#define WIN 1

typedef struct{
	unsigned char posx, posy;
	unsigned char firing; // 0 = false, otherwise true
	char image;
	unsigned char alive; // 0 = false, otherwise true
}siEnemy;

typedef struct{
	unsigned char posx, posy;
	char image;
	unsigned char laserCount; // so we can max player lasers
}siPlayer;

typedef struct{
	char direction; // -1 is up (player shot) 1 down(enemy shot)
	unsigned char posx, posy;
	unsigned char active; // 1 is it is still existent, 0 is it is empty space
	char image;
}siLaser;

typedef struct{
	siEnemy enemies[32];
	char enemyDirectionX; // - is to left, when farthest row left hits wall switches, etc + is right
	siPlayer player;
	siLaser lasers[MAXPLAYERLASERS + 32];
	unsigned char playing; // 1 is playing, 0 paused
}siGame;

void siCreateGame(siGame* game);
void siPauseGame(siGame* game);
char siDoIteration(siGame* game); // - if player lost 0 if not over + if player won
void siDraw(siGame* game);
void siDoInput(siGame* game);

#endif

