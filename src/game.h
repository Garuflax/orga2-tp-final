/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

extern uint8_t actual_key;
extern uint8_t next_key;
extern int8_t keys[];

int add_key(int8_t key);

int8_t get_key();

uint64_t manage_syscall(uint64_t syscall_number,uint64_t param1,uint64_t param2,uint64_t param3);

uint64_t print_syscall(uint64_t param1,uint64_t param2,uint64_t param3);

#endif  /* !__GAME_H__ */
