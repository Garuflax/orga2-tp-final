/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

void inicializar_sched(){
	for (int i = 0; i < MAX_TASKS; ++i){
		process_info[i].state = -1;
	}
	actual_process = 0;
}

uint16_t next(){
	++process_info[actual_process].turns;
	if(process_info[actual_process].state == 1){
		process_info[actual_process].state = 0;
	}
	actual_process = (actual_process + 1) % MAX_TASKS;
	while(process_info[actual_process].state != 0 && process_info[actual_process].state != 2){
		actual_process = (actual_process + 1) % MAX_TASKS;
	}
	if(process_info[actual_process].state == 2){
		start_task(actual_process);
	}
	process_info[actual_process].state = 1;
	load_rsp0(actual_process);
	lcr3(get_cr3(actual_process));
	uint16_t res = CODE_3;
	if(process_info[actual_process].permissions >> 15){
		res = CODE_0;
	}
	return res;
}

uint16_t get_current_ds(){
	uint16_t res = DATA_3;
	if(process_info[actual_process].permissions >> 15){
		res = DATA_0;
	}
	return res;
}

int add_task(uint8_t type,uint16_t permissions,uint16_t initial_message){
	int res = 0;
	uint8_t i = 0;
	//Busco slot para crear tarea
	while((i < MAX_TASKS) && (process_info[i].state != -1)){
		++i;
	}
	if (i == MAX_TASKS){
		res = 1;//Error, no hay espacio para nueva tarea
	}else{
		process_info[i].state = 2;//Estado New
		process_info[i].type = type;
		process_info[i].turns = 0;
		process_info[i].permissions = permissions;
		process_info[i].message = initial_message;
	}
	return res;
}

void remove_task(uint64_t id){
	process_info[id].state = -1;
}

void remove_current_task(){
	remove_task(actual_process);
}

void start_task(uint64_t id){
	uint64_t relative_pos = PAGE_SIZE*process_info[id].type;
	char* task_code = (char*) (TASKS + relative_pos);
	lcr3(KERNEL_PML4);
	mmu_inicializarMemoriaTarea(id,task_code);
	inicilizar_pcb(id);
}

void inicilizar_pcb(uint64_t id){
	pcb* p = get_pcb(id);
	p->rax = 0;
	p->rbx = 0;
	p->rcx = 0;
	p->rdx = 0;
	p->rsi = 0;
	p->rdi = 0;
	p->rbp = 0;
	if(process_info[actual_process].permissions >> 15){
		p->rsp = get_rsp0(id);
	}else{
		p->rsp = TASK_START_ADDR + TASK_SIZE;
	}
	p->r8 = 0;
	p->r9 = 0;
	p->r10 = 0;
	p->r11 = 0;
	p->r12 = 0;
	p->r13 = 0;
	p->r14 = 0;
	p->r15 = 0;
	p->rflags = INITIAL_FLAGS;
	p->rip = TASK_START_ADDR;
}

pcb* get_current_pcb(){
	return get_pcb(actual_process);
}

pcb* get_pcb(uint64_t id){
	return (pcb*) (INICIO_PAGINAS_LIBRES_KERNEL + (id*TASK_KERNEL_SIZE) + PCB_POS);
}

void load_rsp0(uint64_t id){
	tss* t = (tss*) TSS;
	t->rsp0 = get_rsp0(id);
}

uint64_t get_rsp0(uint64_t id){
	return INICIO_PAGINAS_LIBRES_KERNEL + ((id+1)*TASK_KERNEL_SIZE);
}

uint64_t get_cr3(uint64_t id){
	return INICIO_PAGINAS_LIBRES_KERNEL + (id*TASK_KERNEL_SIZE);
}