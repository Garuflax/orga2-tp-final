/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"

void inicializar_idt() {
    idt_entry* idt = (idt_entry*) IDT_POS;
    //Limpio memoria
    limpiar_pagina(IDT_POS >> 12);

    uint64_t exception_offsets[21] = {(uint64_t) &isr0,(uint64_t) &isr1,(uint64_t) &isr2,(uint64_t) &isr3,(uint64_t) &isr4,(uint64_t) &isr5,(uint64_t) &isr6,(uint64_t) &isr7,(uint64_t) &isr8,(uint64_t) &isr9,(uint64_t) &isr10,(uint64_t) &isr11,(uint64_t) &isr12,(uint64_t) &isr13,(uint64_t) &isr14,(uint64_t) &isr15,(uint64_t) &isr16,(uint64_t) &isr17,(uint64_t) &isr18,(uint64_t) &isr19,(uint64_t) &isr20};

    //Lleno excepciones
    for (int i = 0; i < 21; ++i){
        idt[i].offset_0_15 = (exception_offsets[i] & 0xFFFF);
        idt[i].segsel = CODE_0;
        idt[i].attr = 0x8E00;
        idt[i].offset_16_31 = ((exception_offsets[i] >> 16) & 0xFFFF);
        idt[i].offset_32_63 = ((exception_offsets[i] >> 32) & 0xFFFF);
    }
    //Lleno interrupciones externas
    idt[32].offset_0_15 = ((uint64_t) &isr32 & 0xFFFF);
    idt[32].segsel = CODE_0;
    idt[32].attr = 0x8E00;
    idt[32].offset_16_31 = (((uint64_t) &isr32 >> 16) & 0xFFFF);
    idt[32].offset_32_63 = (((uint64_t) &isr32 >> 32) & 0xFFFF);
    
    idt[33].offset_0_15 = ((uint64_t) &isr33 & 0xFFFF);
    idt[33].segsel = CODE_0;
    idt[33].attr = 0x8E00;
    idt[33].offset_16_31 = (((uint64_t) &isr33 >> 16) & 0xFFFF);
    idt[33].offset_32_63 = (((uint64_t) &isr33 >> 32) & 0xFFFF);

    //Lleno syscall
    idt[255].offset_0_15 = ((uint64_t) &isr255 & 0xFFFF);
    idt[255].segsel = CODE_0;
    idt[255].attr = 0xEE00;
    idt[255].offset_16_31 = (((uint64_t) &isr255 >> 16) & 0xFFFF);
    idt[255].offset_32_63 = (((uint64_t) &isr255 >> 32) & 0xFFFF);
    
    //Lleno descriptor IDT
    idt_desc.limit = IDT_SIZE - 1;
    idt_desc.base = (uint64_t) idt;
}