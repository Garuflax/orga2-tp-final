/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  rutinas comunes para lectura y escritura de registros desde C
*/

#ifndef __i386_H__
#define __i386_H__

#include "stdint.h"

#define LS_INLINE static __inline __attribute__((always_inline))

LS_INLINE void lds(uint16_t sel);
LS_INLINE void les(uint16_t sel);
LS_INLINE void lss(uint16_t sel);
LS_INLINE void lcr0(uint64_t val);
LS_INLINE uint64_t rcr0(void);
LS_INLINE void lcr1(uint64_t val);
LS_INLINE uint64_t rcr1(void);
LS_INLINE void lcr2(uint64_t val);
LS_INLINE uint64_t rcr2(void);
LS_INLINE void lcr3(uint64_t val);
LS_INLINE uint64_t rcr3(void);
LS_INLINE void lcr4(uint64_t val);
LS_INLINE uint64_t rcr4(void);
LS_INLINE void tlbflush(void);
LS_INLINE void ltr(uint16_t sel);
LS_INLINE uint16_t rtr(void);
LS_INLINE void hlt(void);
LS_INLINE void sti(void);
LS_INLINE void cli(void);
LS_INLINE void breakpoint(void);
LS_INLINE void exception(void);

/*
 * Implementaciones
 */

LS_INLINE void lds(uint16_t sel) {
    __asm __volatile("movw %0,%%ds" : : "r" (sel));
}

LS_INLINE void les(uint16_t sel) {
    __asm __volatile("movw %0,%%es" : : "r" (sel));
}

LS_INLINE void lss(uint16_t sel) {
    __asm __volatile("movw %0,%%ss" : : "r" (sel));
}

LS_INLINE void lcr0(uint64_t val) {
    __asm __volatile("movq %0,%%cr0" : : "r" (val));
}

LS_INLINE uint64_t rcr0(void) {
    uint64_t val;
    __asm __volatile("movq %%cr0,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr1(uint64_t val) {
    __asm __volatile("movq %0,%%cr1" : : "r" (val));
}

LS_INLINE uint64_t rcr1(void) {
    uint64_t val;
    __asm __volatile("movq %%cr1,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr2(uint64_t val) {
    __asm __volatile("movq %0,%%cr2" : : "r" (val));
}

LS_INLINE uint64_t rcr2(void) {
    uint64_t val;
    __asm __volatile("movq %%cr2,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr3(uint64_t val) {
    __asm __volatile("movq %0,%%cr3" : : "r" (val));
}

LS_INLINE uint64_t rcr3(void) {
    uint64_t val;
    __asm __volatile("movq %%cr3,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr4(uint64_t val) {
    __asm __volatile("movq %0,%%cr4" : : "r" (val));
}

LS_INLINE uint64_t rcr4(void) {
    uint64_t cr4;
    __asm __volatile("movq %%cr4,%0" : "=r" (cr4));
    return cr4;
}

 LS_INLINE void tlbflush(void) {
    uint64_t cr3;
    __asm __volatile("movq %%cr3,%0" : "=r" (cr3));
     __asm __volatile("movq %0,%%cr3" : : "r" (cr3));
}

LS_INLINE void ltr(uint16_t sel) {
    __asm __volatile("ltr %0" : : "r" (sel));
}

LS_INLINE uint16_t rtr(void) {
    uint16_t sel;
    __asm __volatile("str %0" : "=r" (sel) : );
    return sel;
}

LS_INLINE void hlt(void) {
    __asm __volatile("hlt" : : );
}

LS_INLINE void sti(void) {
    __asm __volatile("sti" : : );
}

LS_INLINE void cli(void) {
    __asm __volatile("cli" : : : "memory");
}

LS_INLINE void breakpoint(void) {
    __asm __volatile("xchg %%bx, %%bx" : :);
}

LS_INLINE void exception(void) {
    __asm __volatile("int $0x03" : : : "memory", "cc");
}

#endif  /* !__i386_H__ */
