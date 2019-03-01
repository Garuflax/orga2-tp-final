/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "i386.h"
#include "defines.h"
#include "syscall.h" 

void task();

uint8_t random(uint8_t* n);

void task() {
  uint64_t message = syscall(3,0,0,0);
  uint8_t master = message & 0xFF;
  uint8_t n = (message >> 8);
  uint8_t id = syscall(8,0,0,0);
  uint8_t x = random(&n) % VIDEO_COLUMNS;
  uint8_t y = random(&n) % VIDEO_ROWS;
  message = id | ((uint64_t)x << 5) | ((uint64_t)y << 11);
  while(syscall(4,master,message,0)){}//CUIDADO CON CASO x = 0,y = 0 y id = 0
  syscall(6,id,0,0);
  while(1){
    message = ((uint64_t)(random(&n) % 4) << 5) | id;//CUIDADO CON CASO dir = 0 y id = 0
    while(syscall(4,master,message,0)){}
    message = 0;
    while(!message){
      message = syscall(3,0,0,0);
    }
    if(message == 2){
      syscall(1,0,0,0);
    }
  }
}

uint8_t random(uint8_t* n){
  uint8_t previous = *n;
  *n = (previous*11+7);
  return previous;
}