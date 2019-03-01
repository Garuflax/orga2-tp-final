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
  breakpoint();
}

uint64_t random(uint64_t n){
  return (n*11+7)%30;
}