/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

void limpiar_pagina(uint64_t page){
	char* pos = (char*) (page << 12);
	for (int i = 0; i < 0x1000; ++i){
        pos[i] = 0;
    }
}

void mmu_inicializarMemoriaTarea(uint64_t id,char* task_code){
	uint64_t pos_k = INICIO_PAGINAS_LIBRES_KERNEL + id*TASK_KERNEL_SIZE;
	uint64_t pos_t = INICIO_PAGINAS_LIBRES_TASK + id*TASK_SIZE;
	uint32_t pag = pos_k >> 12;
	//Limpio memoria
	for (int i = 0; i < TASK_KERNEL_SIZE / PAGE_SIZE; ++i){
		limpiar_pagina(pag + i);
	}
	//Mapeo kernel
	uint64_t dir = 0;
	while(dir < INICIO_PAGINAS_LIBRES_TASK){
		mmu_mappear_pagina(dir,pos_k,dir,0,1);
		dir += PAGE_SIZE;
	}
	//Mapeo area tarea
	mmu_mappear_pagina(TASK_START_ADDR,pos_k,pos_t,1,1);
	mmu_mappear_pagina(TASK_START_ADDR+PAGE_SIZE,pos_k,pos_t+PAGE_SIZE,1,1);
	//Copiar codigo
	char* inicio = (char*) pos_t;
	for (int i = 0; i < PAGE_SIZE; ++i){
		inicio[i] = task_code[i];
	}
}

void mmu_mappear_pagina(uint64_t virtual, uint64_t cr3, uint64_t fisica, uint8_t user, uint8_t writable){
	uint64_t pml4e = (virtual >> 39) & 0x1FF;
	uint64_t pdpte = (virtual >> 30) & 0x1FF;
	uint64_t pde = (virtual >> 21) & 0x1FF;
	uint64_t pte = (virtual >> 12) & 0x1FF;
	uint64_t pos = cr3 & 0xFFFFFFFFFFFFF000;
	paging_entry* pml4 = (paging_entry*) pos;
	paging_entry* pdpt = (paging_entry*) (pos + PAGE_SIZE + (pml4e << 30));
	paging_entry* pd = (paging_entry*) ((uint64_t) pdpt + PAGE_SIZE + (pdpte << 21));
	paging_entry* pt = (paging_entry*) ((uint64_t) pd + PAGE_SIZE + (pde << 12));
	if(!pml4[pml4e].p){
		pml4[pml4e].p = 1;
		pml4[pml4e].r_w = 1;
		pml4[pml4e].u_s = 1;
		pml4[pml4e].physical_address = ((uint64_t) pdpt) >> 12;
	}
	if(!pdpt[pdpte].p){
		pdpt[pdpte].p = 1;
		pdpt[pdpte].r_w = 1;
		pdpt[pdpte].u_s = 1;
		pdpt[pdpte].physical_address = ((uint64_t) pd) >> 12;	
	}
	if(!pd[pde].p){
		pd[pde].p = 1;
		pd[pde].r_w = 1;
		pd[pde].u_s = 1;
		pd[pde].physical_address = ((uint64_t) pt) >> 12;	
	}
	pt[pte].p = 1;
	pt[pte].r_w = writable;
	pt[pte].u_s = user;
	pt[pte].physical_address = fisica >> 12;
}

void mmu_unmappear_pagina(uint64_t virtual, uint64_t cr3){
	uint64_t pml4e = (virtual >> 39) & 0x1FF;
	uint64_t pdpte = (virtual >> 30) & 0x1FF;
	uint64_t pde = (virtual >> 21) & 0x1FF;
	uint64_t pte = (virtual >> 12) & 0x1FF;
	uint64_t pos = cr3 & 0xFFFFFFFFFFFFF000;
	paging_entry* pml4 = (paging_entry*) pos;
	paging_entry* pdpt = (paging_entry*) (pos + PAGE_SIZE + (pml4e << 30));
	paging_entry* pd = (paging_entry*) ((uint64_t) pdpt + PAGE_SIZE + (pdpte << 21));
	paging_entry* pt = (paging_entry*) ((uint64_t) pd + PAGE_SIZE + (pde << 12));
	if(pml4[pml4e].p && pdpt[pdpte].p && pd[pde].p){
		pt[pte].p = 0;
	}
}