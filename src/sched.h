/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "mmu.h"
#include "tss.h"

typedef struct str_pcb{
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rsp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rflags;
	uint64_t rip;
} __attribute__((__packed__)) pcb;

typedef struct str_pi{
	int8_t state; // -1: Free, 0: Ready, 1: Running, 2: New, 3: Blocked 
	uint8_t type;
	uint16_t turns;
	uint16_t permissions;
	uint16_t message;
} __attribute__((__packed__)) pi;

extern pi process_info[];
extern int8_t actual_process;

void inicializar_sched();

uint16_t next();

uint16_t get_current_ds();

int add_task(uint8_t type,uint16_t permissions,uint16_t initial_message);

void remove_task(uint64_t id);

void remove_current_task();

void start_task(uint64_t id);

void inicilizar_pcb(uint64_t id);

pcb* get_current_pcb();

pcb* get_pcb(uint64_t id);

void load_rsp0(uint64_t id);

uint64_t get_rsp0(uint64_t id);

uint64_t get_cr3(uint64_t id);

#endif	/* !__SCHED_H__ */
