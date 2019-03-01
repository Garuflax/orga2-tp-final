/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "game.h"

int add_key(int8_t key){
	int res = 1;
	if(!(keys[next_key] || !key)){
		res = 0;
		keys[next_key] = key;
		next_key = (next_key + 1) % KEY_BUFFER_SIZE;
	}
	return res;
}

int8_t get_key(){
	int8_t res = 0;
	if (keys[actual_key]){
		res = keys[actual_key];
		keys[actual_key] = 0;
		actual_key = (actual_key + 1) % KEY_BUFFER_SIZE;
	}
	return res;
}

uint64_t manage_syscall(uint64_t syscall_number,uint64_t param1,uint64_t param2,uint64_t param3){
	uint64_t res = 0;
	switch(syscall_number){
		case 2 :
		if((process_info[actual_process].permissions >> 2) & 1){
			res = add_task(param1,param2,param3); // create task
		}else{
			exception();
		}
		break;
		case 3 :
		res = process_info[actual_process].message; // read
		process_info[actual_process].message = 0;
		break;
		case 4 :
		if(((process_info[actual_process].permissions >> 3) & 1) && (param1 < MAX_TASKS)){ // write
			if(!process_info[param1].message){
				uint16_t message = param2;
				process_info[param1].message = message;
			}else{
				res = 1; // no se pudo escribir porque tiene un mensaje por leer
			}
		}else{
			exception();	
		}
		break;
		case 5 :
		if(((process_info[actual_process].permissions >> 4) & 1) && (param1 < MAX_TASKS)){ // unblock
			if(process_info[param1].state == 3){
				process_info[param1].state = 0; // Pasa a Ready
			}else{
				res = 1; // no se pudo desbloquear porque no lo estaba
			}
		}else{
			exception();	
		}
		break;
		case 6 :
		if(((process_info[actual_process].permissions >> 4) & 1) && (param1 < MAX_TASKS)){ // block
			if((process_info[param1].state != -1) && (process_info[param1].state != 2)){
				process_info[param1].state = 3; // Pasa a Blocked
			}else{
				res = 1; // no se pudo bloquear porque no existe la task o es new
			}
		}else{
			exception();	
		}
		break;
		case 7 :
		if((!((process_info[actual_process].permissions >> 1) & 1) || print_syscall(param1,param2,param3))){ // print
			exception();
		}
		break;
		case 8 :
		res = actual_process; // get id
		break;
		case 9 :
		if(process_info[actual_process].permissions & 1){
			res = get_key(); // get key
		}else{
			exception();
		}
		break;
		default :
		exception();
	}
   return res;
}

uint64_t print_syscall(uint64_t param1,uint64_t param2,uint64_t param3){
	uint64_t res = 0;
	uint8_t* text;
	uint32_t x,y,size,width,height;
	uint8_t character,attr;
	switch(param1){
		case 1 :
		text = (uint8_t*) param2; // DANGER! Usando puntero dado por tarea
		x = param3 >> 48;
		y = (param3 >> 32) & 0xFFFF;
		attr = param3 & 0xFF;
		if(x < VIDEO_COLUMNS && y < VIDEO_ROWS){
			print(text, x, y, attr);
		}else{
			res = 1;
		}
		break;
		case 2 :
		size = (param3 >> 8) & 0xFF;
		x = param3 >> 48;
		y = (param3 >> 32) & 0xFFFF;
		attr = param3 & 0xFF;
		if(x < VIDEO_COLUMNS && y < VIDEO_ROWS){
			print_dec(param2, size, x, y, attr); // Numero grande escribe fuera de la memoria de video
		}else{
			res = 1;
		}
		break;
		case 3 :
		size = (param3 >> 8) & 0xFF;
		x = param3 >> 48;
		y = (param3 >> 32) & 0xFFFF;
		attr = param3 & 0xFF;
		if(x < VIDEO_COLUMNS && y < VIDEO_ROWS){
			print_hex(param2, size, x, y, attr); // Numero grande escribe fuera de la memoria de video
		}else{
			res = 1;
		}
		break;
		case 4 :
		width = param2 >> 48;
		height = (param2 >> 32) & 0xFFFF;;
		character = param2 & 0xFF;;
		x = param3 >> 48;
		y = (param3 >> 32) & 0xFFFF;
		attr = param3 & 0xFF;
		if(x + width <= VIDEO_COLUMNS && y + height <= VIDEO_ROWS){
			print_box( x, y, width, height, character, attr);
		}else{
			res = 1;
		}
		break;
		default :
		res = 1;
	}
	return res;
}