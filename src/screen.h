/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "stdint.h"
#include "defines.h"

/* Estructura para acceder a celda de memoria de video */
typedef struct str_cell {
    uint8_t ascii;
    uint8_t colour;
} cell;

void print(const uint8_t* text, uint32_t x, uint32_t y, uint8_t attr);
void print_dec(uint64_t number, uint32_t size, uint32_t x, uint32_t y, uint8_t attr);
void print_hex(uint64_t number, uint32_t size, uint32_t x, uint32_t y, uint8_t attr);
void print_box(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t character, uint8_t attr );

#endif  /* !__SCREEN_H__ */
