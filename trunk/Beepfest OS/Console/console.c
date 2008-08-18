#include "console.h"
#include "consoleFunctions.h"

#define NULL 0

int nbrCmds = 7;
char* commands[] = {
	"print",
	"spaceinvaders",
	"version",
	"status",
	"rand",
	"help",
	"snake",
};
void* commandFuncs[] = {
	cprint,
	csi,
	cversion,
	cstatus,
	crand,
	chelp,
	csnake,
};

int strlen(char* str){
	int x;
	for(x=0; *(str++) != 0; x++);	
	return x;
}
int strcmp(char* str1, char* str2){
	while(*str1 != 0 && *str2 != 0 && *(str1++) == *(str2++));
	
	return *(str1-1) - *(str2-1); /* SO IF THEY'RE THE SAME, IT RETURNS 0, LIKE IT'S SUPPOSED TO */
}

void* getStrFunc(char* str){
	int x;
	for(x = 0; x < nbrCmds; x++){
		if(!strcmp(str, commands[x]))
			return commandFuncs[x];
	}
	
	return NULL;
}
void doConsole(char* cmd){
	/* OK WHAT I HAVE TO DO IS THIS:
	 * SPLIT THE cmd ARRAY ON SPACES
	 * INTO SEVERAL CHAR POINTERS, 
	 * KEEPING TRACK OF THE NUMBER */
	char command[256];
	char arg1[256];
	char arg2[256];
	char arg3[256];
	char arg4[256];
	char arg5[256];
	int x;
	for(x = 0; x < 256; x++){
		command[x] = 0;
		arg1[x] = 0;
		arg2[x] = 0;
		arg3[x] = 0;
		arg4[x] = 0;
		arg5[x] = 0;
	}
	int curArg = -1;
	int bufPos = 0;
	int length = strlen(cmd);
	char* args[5];
	args[0] = arg1;
	args[1] = arg2;
	args[2] = arg3;
	args[3] = arg4;
	args[4] = arg5;
	unsigned char inQuotes = 0;
	for(x = 0; x < length; x++){
		if(bufPos >= 256){
			while(x < length && cmd[x++] != ' ');
			curArg++;
			bufPos = 0;
			if(curArg >= 5){
				curArg = 4;
				break;
			}
			continue;
		}
		if(cmd[x] == '\''){
			if(inQuotes == 1) inQuotes = 0;
			else inQuotes = 1;
		}else if(cmd[x] == ' ' && inQuotes == 0){
			curArg++;
			bufPos = 0;
			if(curArg >= 5){
				curArg = 4;
				break;
			}
		}else{
			if(curArg == -1){
				command[bufPos++] = cmd[x];
			}else{
				args[curArg][bufPos++] = cmd[x];
			}
		}
	}
	void (*function)(int, char**) = getStrFunc(command);
	if(function == NULL){
		kprint("Unrecognized Command: \"");
		kprint(command);
		kprint("\"!\n");
		return;
	}else{
		function((curArg+1), args);
	}
}
