/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "i386.h"
#include "defines.h"
#include "syscall.h" 

void task() {
	char name[8] = "DUNGEON";
	char os[3] = "OS";
    char author[15] = "LINARI FACUNDO";
    char date[10] = "2018-2019";
    char play[5] = "PLAY";
    char instructions[13] = "INSTRUCTIONS";
    char floor[6] = "FLOOR";
    char text[3][21] = { "Arrow keys: movement", "WASD: spell", "PRESS ENTER TO PLAY"};
    
    /*Create menu*/
    syscall(7,4,0x0050003200000020,0);
   	syscall(7,1,(uint64_t)author,0x000100300000000E);
   	syscall(7,1,(uint64_t)date,0x004600300000000E);
   	syscall(7,1,(uint64_t)name,0x0024000900000002);
   	syscall(7,1,(uint64_t)os,0x0026000A00000004);
   	syscall(7,1,(uint64_t)play,0x002200140000000F);
   	syscall(7,1,(uint64_t)instructions,0x002200190000000F);

   	char cursorState = 1;
   	syscall(7,4,0x0001000100000010,0x002000140000000F);

   	uint16_t seed = 0;

   	uint64_t key = 0;
    while(key != KEY_ENTER){
    	key = syscall(9,0,0,0);
    	++seed;
    	if(key == KEY_UP || key == KEY_DOWN){
    		if(cursorState == 1){
    			syscall(7,4,0x0001000100000020,0x0020001400000000);
    			syscall(7,4,0x0001000100000010,0x002000190000000F);
    			++cursorState;
    		}else{
    			syscall(7,4,0x0001000100000020,0x0020001900000000);
    			syscall(7,4,0x0001000100000010,0x002000140000000F);
    			--cursorState;
    		}
    	}
    }
    
    if(cursorState == 2){
    	//Clean
    	syscall(7,4,0x0050003200000020,0);
    	for (int i = 0; i < 3; ++i){
    		uint64_t attr =  0x0009000000000000 + ((uint64_t)(i*5+5) << 32) + 0x0F;
    		syscall(7,1,(uint64_t)text[i],attr);
    	}
    	key = 0;
    	while(key != KEY_ENTER){
	    	key = syscall(9,0,0,0);
	    	++seed;	
	    }
    }
    //Clean
    syscall(7,4,0x0050003200000020,0);
    //Game area
   	syscall(7,4,0x0020002000000020,0x0018000900000010);
   	//Text area
   	syscall(7,4,0x0050000700000020,0x0000002B00000040);
   	//HUD
   	syscall(7,4,0x0001000100000003,0x0005000500000004);
   	syscall(7,2,10,0x0007000500000204);
   	syscall(7,4,0x000100010000002F,0x0009000500000004);
   	syscall(7,2,10,0x000A000500000204);
   	syscall(7,4,0x000100010000000E,0x0043000500000003);
   	syscall(7,2,10,0x0045000500000203);
   	syscall(7,4,0x000100010000002F,0x0047000500000003);
   	syscall(7,2,10,0x0048000500000203);
   	syscall(7,1,(uint64_t)floor,0x002400050000000F);
    //Create game manager
    syscall(2,1,0x1E,seed);
    syscall(1,0,0,0);
}
