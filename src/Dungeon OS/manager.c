/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#include "i386.h"
#include "defines.h"
#include "syscall.h"

void task();

uint64_t random(uint64_t n);

void task() {
  uint64_t id = syscall(8,0,0,0);
  uint64_t seed = syscall(3,0,0,0);
  uint32_t characters[10];
  char floor = 0;
  char field[30][30];
  for (int i = 0; i < 30; ++i){
    for (int j = 0; j < 30; ++j){
      field[i][j] = -1;
    }
  }
  for (int i = 0; i < 10; ++i){
    characters[i] = 0;
  }
  syscall(2,2,0x09,id);
  uint64_t message = 0;
  while(!message){
    message = syscall(3,0,0,0);
  }
  characters[0] = (message - 0x80) + 0x800F0F00;
  char exit;
  while(floor < 10){
    exit = 0;
    syscall(7,2,floor,0x002A00050000010F);
    syscall(7,4,0x001E001E000000B0,0x0019000A00000026);
    //GET PLAYER POSITION
    char px = (characters[0] >> 16) & 0xFF;
    char py = (characters[0] >> 8) & 0xFF;
    field[(uint8_t)py][(uint8_t)px] = 1;
    syscall(7,4,0x0001000100000002,((uint64_t)(px + 0x19) << 48) | ((uint64_t)(py+ 0x0A) << 32) | 0x2F);
    //GENERATE EXIT POSITION
    seed = random(seed);
    char y = seed;
    seed = random(seed);
    char x = seed;
    if(x == px || y == py){
      x = (x + 1) % 30;
    }
    field[(uint8_t)y][(uint8_t)x] = 0;
    syscall(7,4,0x00010001000000F0,((uint64_t)(x + 0x19) << 48) | ((uint64_t)(y+ 0x0A) << 32) | 0x26);
    //GENERATE ENEMIES
    for (int i = 0; i < floor; ++i){
      syscall(2,3,0x08,id);
      message = 0;
      while(!message){
        message = syscall(3,0,0,0);
      }
      uint16_t enemy = message - 0x80;
      characters[i+1] = enemy;
      while(field[(uint8_t)y][(uint8_t)x] != -1){
        seed = random(seed);
        y = seed;
        seed = random(seed);
        x = seed;
      }
      while(syscall(4,enemy,x+0x0100,0)){}
      while(syscall(4,enemy,y+0x0200,0)){}
      field[(uint8_t)y][(uint8_t)x] = i + 2;
      syscall(7,4,0x0001000100000058,((uint64_t)(x + 0x19) << 48) | ((uint64_t)(y+ 0x0A) << 32) | 0x2F);
      characters[i+1] = (1 << 31) | (x << 16) | (y << 8) | characters[i+1];
    }
    uint8_t index = 0;
    //MAIN LOOP
    while(!exit){
      if(characters[index] >> 31){ //BUG con enemigos
        x = ((characters[index] >> 16) & 0xFF);
        y = ((characters[index] >> 8) & 0xFF);
        uint8_t pid = characters[index] & 0xFF;
        while(syscall(4,pid,0x0800,0)){}
        message = 0;
        while(!(message & 0x30)){
          message = syscall(3,0,0,0);
          if(message & 0x40){
            while(syscall(4,pid,((characters[0] >> 16) & 0xFF) + 0x0500,0)){}
          }else if(message & 0x80){
            while(syscall(4,pid,((characters[0] >> 8) & 0xFF) + 0x0600,0)){}
          }
        }
        uint8_t dir = message & 0x0F;
        uint8_t type = (index > 0) ? 88: 2;
        if((message >> 4) & 1){
          uint8_t newx = x;
          uint8_t newy = y;
          switch(dir){
            case 0 :
            if(y > 0){
              --newy;
            }
            break;
            case 1 :
            if(x < 29){
              ++newx;
            }
            break;
            case 2 :
            if(y < 29){
              ++newy;
            }
            break;
            case 3 :
            if(x > 0){
              --newx;
            }
            break;
          }
          if((x != newx) || (y != newy)){
            if(!field[newy][newx]){
              if(index == 0){
                exit = 1;
              }
            }else if(field[newy][newx] == -1){
              field[(uint8_t)x][(uint8_t)y] = -1;
              syscall(7,4,0x00010001000000B0,((uint64_t)(x + 0x19) << 48) | ((uint64_t)(y+ 0x0A) << 32) | 0x26);
              x = newx;
              y = newy;
              field[(uint8_t)x][(uint8_t)y] = index + 1;
              syscall(7,4,0x0001000100000000 + type,((uint64_t)(x + 0x19) << 48) | ((uint64_t)(y+ 0x0A) << 32) | 0x2F);
              characters[index] = (characters[index] & 0xFF) | (x << 16) | (y << 8) | (1 << 31);
            }else{
              //ATTACK!
            }
            while(syscall(4,pid,x+0x0100,0)){}
            while(syscall(4,pid,y+0x0200,0)){}
          }
        }else{
          //ACA LA IDEA ERA HACER UN ATAQUE MAGICO DIRECCIONADO PERO POR QUEDARME CORTO DE BYTES SOLO IMPLEMENTO CURACION
          while(syscall(4,pid,0x0501,0)){}//MAL
        }
      }
      index = (index + 1) % 10;
    }
    //CLEAN
    for (int i = 0; i < 30; ++i){
      for (int j = 0; j < 30; ++j){
        field[i][j] = -1;
      }
    }
    for (int i = 1; i < 10; ++i){
      if(characters[i] >> 31){
        uint8_t pid = characters[i] & 0xFF;
        while(syscall(4,pid,0xFF00,0)){}
        characters[i] = 0;
      }
    }
    if(exit == 2){
      floor = 9;
    }
    ++floor;
  }
  //PRINT GAME OVER TODO
  syscall(1,0,0,0);
}

uint64_t random(uint64_t n){
  return (n*11+7)%30;
}