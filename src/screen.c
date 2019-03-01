/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void print(const uint8_t* text, uint32_t x, uint32_t y, uint8_t attr) {
    cell* pos = (cell*) (VIDEO + (uint64_t)((x + y * VIDEO_COLUMNS)*2));
    for (int i = 0; text[i] != 0; ++i){
        pos[i].ascii = text[i];
        pos[i].colour = attr;
    }
}

void print_dec(uint64_t number, uint32_t size, uint32_t x, uint32_t y, uint8_t attr) {
    cell* pos = (cell*) (VIDEO + (uint64_t)((x + y * VIDEO_COLUMNS)*2));
    uint8_t letras[10] = "0123456789";
    for (uint32_t i = 1; i <= size; ++i){
        uint32_t resto  = number % 10;
        number /= 10;
        pos[size - i].ascii = letras[resto];
        pos[size - i].colour = attr;
    }
}

void print_hex(uint64_t number, uint32_t size, uint32_t x, uint32_t y, uint8_t attr) {
    cell* pos = (cell*) (VIDEO + (uint64_t)((x + y * VIDEO_COLUMNS)*2));
    uint8_t letras[16] = "0123456789ABCDEF";
    for (uint32_t i = 1; i <= size; ++i){
        uint32_t resto  = number % 16;
        number /= 16;
        pos[size - i].ascii = letras[resto];
        pos[size - i].colour = attr;
    }
}

void print_box(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t character, uint8_t attr ) {
    cell* video = (cell*) VIDEO;
    uint32_t pos = x + y * VIDEO_COLUMNS;
    for (uint32_t i = 0; i < height; ++i){
        for (uint32_t j = 0; j < width; ++j){
            video[pos + i*VIDEO_COLUMNS + j].ascii = character;
            video[pos + i*VIDEO_COLUMNS + j].colour = attr;
        }
    }
}