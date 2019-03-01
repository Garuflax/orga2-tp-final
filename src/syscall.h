/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "stdint.h"

#define LS_INLINE static __inline __attribute__((always_inline))

LS_INLINE uint64_t syscall(uint64_t syscall_number,uint64_t param1,uint64_t param2,uint64_t param3) {
    uint64_t ret;
    __asm __volatile(
        "mov %1, %%rdi \n"
        "mov %2, %%rsi \n"
        "mov %3, %%rdx \n"
        "mov %4, %%rcx \n"
        "int $0xFF     \n"
        : "=r" (ret)
        : "m" (syscall_number), "m" (param1), "m" (param2), "m" (param3)
        : "rdi", "rsi", "rdx", "rcx"
    );
    return ret;
}

#endif  /* !__SYSCALL_H__ */
