/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "i386.h"
#include "defines.h"
#include "syscall.h" 

void task() {
  char heart = 10;
  char music = 10;
  char spellCost = 1;
  uint64_t manager = syscall(3,0,0,0);
  uint64_t id = syscall(8,0,0,0);
  while(syscall(4,manager,id + 0x80,0)){}
  uint16_t message;
  uint8_t action = 0;
  uint8_t key;
  while(action != 0xFF){
    message = 0;
    while(!message){
      message = syscall(3,0,0,0) & 0xFFFF;
    }
    action = message >> 8;
    switch(action){
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
      if(heart > 10){
        heart = 10;
      }
      while(syscall(4,manager,heart,0)){}
      break;
      case 8 :
      action = 0;
      while(!action){
        key = syscall(9,0,0,0);
        switch(key){
          case KEY_UP :
          action = 0x10;
          break;
          case KEY_RIGHT :
          action = 0x11;
          break;
          case KEY_DOWN :
          action = 0x12;
          break;
          case KEY_LEFT :
          action = 0x13;
          break;
          case KEY_W :
          action = 0x20;
          break;
          case KEY_D :
          action = 0x21;
          break;
          case KEY_S :
          action = 0x22;
          break;
          case KEY_A :
          action = 0x23;
          break;
        }
        if(((action >> 5) & 1) && (music >= spellCost)){ // CHECK IF YOU CAN DO SPELL
          music -= spellCost;
        }
      }
      
      message = action;
      while(syscall(4,manager,message,0)){}
      break;
    }
  }
  syscall(1,0,0,0);
}
