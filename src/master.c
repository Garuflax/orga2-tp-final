/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "i386.h"
#include "defines.h"
#include "syscall.h" 

typedef struct str_cell{
	uint8_t alive;
	uint8_t hp;
	uint8_t x;
	uint8_t y;
} cell;

void task();

void create_cell(cell* cells,uint8_t id,uint8_t hp,uint8_t n);

void move_cell(cell* c,cell* food);

void add_food(cell* food,uint8_t x,uint8_t y,uint8_t hp);

void manage_keyboard(uint8_t* hp,cell* cells,cell* food,uint8_t id,uint8_t n,uint8_t* x,uint8_t* y);

void task() {
	syscall(7,4,((uint64_t)VIDEO_COLUMNS << 48) + ((uint64_t)VIDEO_ROWS << 32) + 20,0);
	uint8_t id = syscall(8,0,0,0);
	uint8_t base_hp = 10;
	uint8_t random = 0;
	uint8_t index = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	cell cells[MAX_TASKS];
	cell food[MAX_TASKS];
	for (int i = 0; i < MAX_TASKS; ++i){
		cells[i].alive = 0;
		food[i].alive = 0;
	}
    while(1){
    	manage_keyboard(&base_hp,cells,food,id,random,&x,&y);
    	if(cells[index].alive){
    		syscall(5,index,0,0);
    		move_cell(&cells[index],food);
    	}
    	index = (index + 1) % MAX_TASKS;
    	++random;
    }
}

void create_cell(cell* cells,uint8_t id,uint8_t hp,uint8_t n){
	//PRECONDICION ESTA LIBRE EL INBOX
	if(!syscall(2,2,0x18,id+(n << 8))){
		uint64_t message = 0;
		while(!message){
		  message = syscall(3,0,0,0);
		}
		uint8_t pid = message & 0x1F;
		uint8_t x = (message >> 5) & 0x3F;
		uint8_t y = (message >> 11) & 0x1F;
		cells[pid].alive = 1;
		cells[pid].hp = hp;
		cells[pid].x = x;
		cells[pid].y = y;
		syscall(7,4,0x0001000100000020,((uint64_t)x << 48) | ((uint64_t)y << 32) | 0x20);
	}
}

void move_cell(cell* c,cell* food){
	uint64_t message = 0;
	while(!message){
	  message = syscall(3,0,0,0);
	}
	uint8_t pid = message & 0x1F;
	uint8_t dir = (message >> 5) & 0x03;
	syscall(7,4,0x0001000100000020,((uint64_t)c->x << 48) | ((uint64_t)c->y << 32));
	switch(dir){
		case 0 :
		c->y = (c->y - 1 + VIDEO_ROWS) % VIDEO_ROWS;
		break;
		case 1 :
		c->x = (c->x + 1) % VIDEO_COLUMNS;
		break;
		case 2 :
		c->y = (c->y + 1) % VIDEO_ROWS;
		break;
		case 3 :
		c->x = (c->x - 1 + VIDEO_COLUMNS) % VIDEO_COLUMNS;
		break;
	}
	for (int i = 0; i < MAX_TASKS; ++i){
		if(food[i].alive && food[i].x == c->x && food[i].y == c->y){
			food[i].alive = 0;
			c->hp = food[i].hp;
		}
	}
	if(--c->hp){
		message = 1;
		syscall(7,4,0x0001000100000020,((uint64_t)c->x << 48) | ((uint64_t)c->y << 32) | 0x20);
		syscall(6,pid,0,0);
	}else{
		c->alive = 0;
		message = 2;
	}
	while(syscall(4,pid,message,0)){}
}

void add_food(cell* food,uint8_t x,uint8_t y,uint8_t hp){
	uint8_t index = 0;
	while(index < MAX_TASKS && food[index].alive){
		++index;
	}
	if(index < MAX_TASKS){
		food[index].alive = 1;
		food[index].hp = hp;
		food[index].x = x;
		food[index].y = y;
		syscall(7,4,0x0001000100000003,((uint64_t)x << 48) | ((uint64_t)y << 32) | 0x04);
	}
}

void manage_keyboard(uint8_t* hp,cell* cells,cell* food,uint8_t id,uint8_t n,uint8_t* x,uint8_t* y){
	uint8_t key = syscall(9,0,0,0);
	switch(key){
	  case KEY_RIGHT :
	  *hp = (*hp < 100) ? *hp + 1 : 100;
	  break;
	  case KEY_LEFT :
	  *hp = (*hp > 1) ? *hp - 1 : 1;
	  break;
	  case KEY_C :
	  create_cell(cells,id,*hp,n);
	  break;
	  case KEY_F :
	  add_food(food,*x,*y,*hp);
	  break;
	  case KEY_W :
	  *y = (*y - 1 + VIDEO_ROWS) % VIDEO_ROWS;
	  break;
	  case KEY_D :
	  *x = (*x + 1) % VIDEO_COLUMNS;
	  break;
	  case KEY_S :
	  *y = (*y + 1) % VIDEO_ROWS;
	  break;
	  case KEY_A :
	  *x = (*x - 1 + VIDEO_COLUMNS) % VIDEO_COLUMNS;
	  break;
	}
}