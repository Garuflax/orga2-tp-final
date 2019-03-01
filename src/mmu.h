/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"

typedef struct str_paging_entry {
    uint8_t   p:1;
    uint8_t   r_w:1;
    uint8_t   u_s:1;
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   a:1;
    uint8_t   ign:1;
    uint8_t   ps:1;
    uint8_t  ignored_bits:4;
    uint64_t  physical_address:52;
} __attribute__((__packed__, aligned (4))) paging_entry;

void limpiar_pagina(uint64_t page);

// Supone que se esta usando un cr3 que "ve todo"
void mmu_inicializarMemoriaTarea(uint64_t id,char* task_code);

void mmu_mappear_pagina(uint64_t virtual, uint64_t cr3, uint64_t fisica, uint8_t user, uint8_t writable);

void mmu_unmappear_pagina(uint64_t virtual, uint64_t cr3);

#endif	/* !__MMU_H__ */