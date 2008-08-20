#include "../Video/video.h"
#include "SpaceInvaders.h"
#include "../Keyboard/kb.h"
#include "../Kernel/timer.h"
#include "console.h"
#include "../Kernel/rand.h"
#include "../Memory/kmem.h"

extern int nbrCmds;
char* commandHelp[] = {
	"\tprint: Prints out the text following the command\nNOTE: spaces that aren't enclosed in \'\' will be written on a different line\n",
	"\tspaceinvaders: Plays space invaders!!\nCONTROLS: Arrow keys: move player, space: fire\n",
	"\tversion: Displays version information about the OS\n",
	"\tstatus: Displays some basic status information about the OS\n",
	"\trand: Rand expects two parameters, the first being the minimum number and the second the max, and it uses these to construct a pseudo-random number.\n",
	"\thelp: Well, I think you figured this one out... but it displays this nifty information!\n",
	"\tsnake: Plays Snake!!\nCONTROLS: Arrow keys: move snake\n",
	"\tpage: Tests the paging techniques in the kernel\n",
};

void cprint(int argc, char** argv){
	int x;
	for(x = 0; x < argc; x++){
		kprint(argv[x]);
		kprint("\n");
	}
}
void csi(int argc, char** argv){
	setKeybState(SPACEINVADERSKEYB);
	char lastData = 0;
	siGame game;
	siCreateGame(&game);
	siDoInput(&game);
	siDraw(&game);
	while(INPROGRESS == (lastData = siDoIteration(&game))){
		siDoInput(&game);
		siDraw(&game);		
		sleep(100);
	}
	clrscr();
	setKeybState(CONSOLE);
	if(lastData == WIN)
		kprint("YOU WIN!\n");
	else if(lastData == LOSS)
		kprint("YOU LOSE!\n");
	else
		kprint("LOL WUT?\n");
	kprint("\tTip: type help for command info\n");
}
void cversion(int argc, char** argv){
	kprint("Beepfest OS v0.01a\n07/2008 - Kevin Logan\n");
}
void cstatus(int argc, char** argv){
	kprint("Beepfest OS is running, and has been for ");
	kprintUN(timer_seconds(), 10);
	kprint(" seconds!\n");
}

void crand(int argc, char** argv){
	if(argc != 2){
		kprint("ERROR: rand takes 2 arguments, min and max\n");
		return;
	}
	int min = 0, max = 0;
	int step = 0, x;
	x = strlen(argv[0]) - 1;
	while(x >= 0){
		int temp = (argv[0][x] - '0'); /* thats the int of the single digit */
		int p;
		for(p = 0; p < step; p++)
			temp *= 10;
		
		min += temp;

		step++;
		x--;
	}
	step = 0;
	x = strlen(argv[1]) - 1;
	while(x >= 0){
		int temp = (argv[1][x] - '0'); /* thats the int of the single digit */
		int p;
		for(p = 0; p < step; p++)
			temp *= 10;
		
		max += temp;

		step++;
		x--;
	}
	
	kprint("Random number between ");
	kprintUN(min, 10);
	kprint(" and ");
	kprintUN(max, 10);
	kprint(" resulted in ");
	kprintUN(rand(min, max), 10);
	kprint("!\n");
}

void chelp(int argc, char** argv){
	int x;
	kprint("Beepfest OS Console contains the following commands:\n");
	for(x = 0; x < nbrCmds; x++){
		kprint(commandHelp[x]);
	}
	kprint("SPECIAL NOTE: Any commands that can take multiple arguments have a limit of 5 arguments!\n");
}

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define HEAD_C '#'
#define BODY_C '*'
#define FRUIT_C '@'
#define BACK 0x0
#define FORE 0xF

void csnake(int argc, char** argv){
	setKeybState(SNAKE);
	
	clrscr();
	unsigned short attrib = (BACK << 4) | ((FORE & 0x0F) << 8);
	unsigned short *vgaBuffer = (unsigned short*) 0xB8000;
	unsigned int playerPositions[256];
	unsigned int playerCount = 1;
	playerPositions[0] = (5 * 80) + 30;
	unsigned int direction = RIGHT;
	unsigned int last_tick;
	unsigned int fruitLocation = genrand_int32() % 2000;
	for(;;) 
	{
		last_tick = get_timer_ticks();
		// check keypresses
		switch (keyDown) {
		case ARROW_LEFT:
			if (direction == RIGHT) break;
			direction = LEFT; break;
		case ARROW_RIGHT:
			if (direction == LEFT) break;
			direction = RIGHT; break;
		case ARROW_UP:
			if (direction == DOWN) break;
			direction = UP; break;
		case ARROW_DOWN:
			if (direction == UP) break;
			direction = DOWN; break;
		}
		
		int clearPlayerPosition = playerPositions[playerCount - 1];
		// shift positions
		if (playerCount > 1) {
			int i = 0;
			for (i = playerCount - 2; i >= 0; i--)
				playerPositions[i+1] = playerPositions[i];
		}
		
		int tr = playerPositions[0] / 80;
		int tc = playerPositions[0] % 80;
		
		// move player
		switch (direction) {
		case LEFT:
			tc--;
			break;
		case RIGHT:
			tc++;
			break;
		case UP:
			tr--;
			break;
		case DOWN:
			tr++;
			break;
		}
		
		// edge check
		if (tc >= 80)
			tc = 0;
		if (tc < 0)
			tc = 79;
		if (tr < 0)
			tr = 24;
		if (tr >= 25)
			tr = 0;
		
		// update for edge check
		playerPositions[0] = (tr * 80) + tc;
		
		// collision check
		char c = vgaBuffer[playerPositions[0]] & 0xFF;
		if (c == BODY_C || c == HEAD_C) {
			// game over
			clrscr();
			setKeybState(CONSOLE);
			kprint("Game over!\n");
			kprint("Your Score: ");
			kprintUN(playerCount, 10);
			kprint("\n\tTip: type help for command info\n");
			return;
		}
		if (c == FRUIT_C) {
			playerCount++;
			playerPositions[playerCount - 1] = clearPlayerPosition;
			clearPlayerPosition = -1;
			fruitLocation = genrand_int32() % 2000;
		}
		
		// draw
		// clear old position
		if (clearPlayerPosition >= 0)
			vgaBuffer[clearPlayerPosition] = attrib;
		// draw new position
		vgaBuffer[playerPositions[0]] = attrib | (HEAD_C & 0xFF);
		// draw fruit
		vgaBuffer[fruitLocation] = attrib | (FRUIT_C & 0xFF);
		
		if (playerCount > 1) {
			// redraw previous position
			vgaBuffer[playerPositions[1]] = attrib | (BODY_C & 0xFF);
		}
		
		// 18 "fps"
		while (last_tick == get_timer_ticks());
	}
}

void cpage(int argc, char** argv){
	char* string = (char*)kAllocPage(); // so 4096 bytes... lol
	char* string2 = (char*)kAllocPage();
	char* string3 = (char*)kAllocPage();
	kprint("Allocated Page, testing memory....\n");
	int x;
	for(x = 0; x < 4095; x++){
		string[x] = 'A' + (x % 26);
	} 
	string[4095] = 0;
	kprint("Memory filled successfully... Received Locations:\n");
	kprintUN((unsigned int)string, 16);
	kprint("\n");
	kprintUN((unsigned int)string2, 16);
	kprint("\n");
	kprintUN((unsigned int)string3, 16);
	kprint("\n");
	//kprint(string);
	kprint("\nMemory output successfully! Testing free...\n");
	kFreePage(string);
	kFreePage(string2);
	kFreePage(string3);
	kprint("Memory freed successfully! Tests complete!\n");
}
