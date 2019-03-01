/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __IDT_H__
#define __IDT_H__

#include "stdint.h"
#include "defines.h"
#include "mmu.h"
#include "isr.h"

/* Struct de descriptor de IDT */
typedef struct str_idt_descriptor {
    uint16_t limit;
    uint64_t base;
} __attribute__((__packed__)) idt_descriptor;

/* Struct de una entrada de la IDT */
typedef struct str_idt_entry {
    uint16_t offset_0_15;
    uint16_t segsel;
    uint16_t attr;
    uint16_t offset_16_31;
    uint32_t offset_32_63;
    uint32_t reserved;
} __attribute__((__packed__)) idt_entry;//, aligned (8))) idt_entry;

extern idt_descriptor idt_desc;

void inicializar_idt();

#endif  /* !__IDT_H__ */
