/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "mmu.h"

typedef struct str_tss {
    uint32_t  reserved0;
    uint64_t  rsp0;
    uint64_t  rsp1;
    uint64_t  rsp2;  
    uint64_t  reserved1;
    uint64_t  ist1;
    uint64_t  ist2;
    uint64_t  ist3;
    uint64_t  ist4;
    uint64_t  ist5;
    uint64_t  ist6;
    uint64_t  ist7;
    uint64_t  reserved2;
    uint16_t  reserved3;
    uint16_t  iomap;
} __attribute__((__packed__)) tss;

void inicializar_tss();

#endif  /* !__TSS_H__ */
