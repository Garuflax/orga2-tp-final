/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

void inicializar_tss(){
	tss* t = (tss*) TSS;
	t->reserved0 = 0x00000000;
	t->rsp0 = 0x0000000000000000;
	t->rsp1 = 0x0000000000000000;
	t->rsp2 = 0x0000000000000000;
	t->reserved1 = 0x0000000000000000;
	t->ist1 = 0x0000000000000000;
	t->ist2 = 0x0000000000000000;
	t->ist3 = 0x0000000000000000;
	t->ist4 = 0x0000000000000000;
	t->ist5 = 0x0000000000000000;
	t->ist6 = 0x0000000000000000;
	t->ist7 = 0x0000000000000000;
	t->reserved2 = 0x0000000000000000;
	t->reserved3 = 0x0000;
	t->iomap = 0xFFFF;
}