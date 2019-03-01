/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "i386.h"
#include "defines.h"
#include "syscall.h"

void task() {
  uint64_t manager = syscall(3,0,0,0);
  uint64_t id = syscall(8,0,0,0);
  char heart = 3;
  uint8_t x,y;
  uint16_t message = 0;
  uint8_t action = 0;
  while(syscall(4,manager,id + 0x80,0)){}
  while(action != 0xFF){
    while(!message){
      message = syscall(3,0,0,0) & 0xFFFF;
    }
    action = message >> 8;
    switch(action){
      case 1 :
      x = message & 0xFF;
      break;
      case 2 :
      y = message & 0xFF;
      break;
      case 4 :
      message = message & 0xFF;
      heart -= message;
      if(heart < 0){
        heart = 0;
      }
      while(syscall(4,manager,heart,0)){}
      break;
      case 5 :
      message = message & 0xFF;
      heart += message;
      if(heart > 3){
        heart = 3;
      }
      while(syscall(4,manager,heart,0)){}
      break;
      case 8 :
      while(syscall(4,manager,0x40,0)){}
      while(!message){
        message = syscall(3,0,0,0) & 0xFFFF;
      }
      message = message & 0xFF;
      if (message > x){
        message = 0x11;
      }else if(message < x){
        message = 0x13;
      }else{
        while(syscall(4,manager,0x80,0)){}
        while(!message){
          message = syscall(3,0,0,0) & 0xFFFF;
        }
        message = message & 0xFF;
        if (message > y){
          message = 0x12;
        }else {
          message = 0x10;
        }
      }
      while(syscall(4,manager,message,0)){}
      break;
    }
  }
  syscall(1,0,0,0);
}