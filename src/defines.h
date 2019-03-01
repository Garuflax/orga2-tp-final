/* ** por compatibilidad se omiten tildes **
===================================================================================
 TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
===================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* IDT */
/* -------------------------------------------------------------------------- */
#define IDT_POS               0x00000000 /* Comienzo de la memoria (piso IVT)*/
#define IDT_SIZE              0x00001000 /* 256 entradas de 128 bits */
#define IDT_ENTRIES_AMOUNT    256

/* Selectores */
/* -------------------------------------------------------------------------- */
#define CODE_0                0x0010 /* Código 64 bits readable nivel 0 */
#define CODE_3                0x001B /* Código 64 bits readable nivel 3 */
#define DATA_0                0x0020 /* Datos writable nivel 0 */
#define DATA_3                0x002B /* Datos writable nivel 3 */

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR              0x00007C00 /* direccion fisica de comienzo del bootsector */
#define KERNEL                  0x00001000 /* direccion fisica de comienzo del kernel */
#define TASKS                   0x00008000 /* comienzo codigo de tareas */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
#define KERNEL_PML4             0x00080000 /* direccion fisica de pml4 global */
#define TSS                     0x00085000 /* direccion fisica de comienzo de TSS */

/* Manejo memoria */
/*----------------------------------------------------------------------------*/
#define PAGE_SIZE                      0x1000   /* size en bytes de una page */
#define INICIO_PAGINAS_LIBRES_KERNEL   0x100000 /* Comienza area libre de kernel */
#define INICIO_PAGINAS_LIBRES_TASK     0x200000 /* Comienza area libre de tasks */

/* Tareas */
/*----------------------------------------------------------------------------*/
#define MAX_TASKS               32                 /* cantidad maxima de tareas */
#define TASK_KERNEL_SIZE        0x8000             /* cantidad que ocupa del area libre de kernel una tarea */
#define PCB_POS                 0x6000             /* posicion relativa en area del kernel de la tarea del PCB */
#define TASK_SIZE               0x2000             /* cantidad que ocupa del area libre de tareas una tarea */
#define TASK_START_ADDR         0x200000           /* dir virtual en donde comienzan las tareas */
#define INITIAL_FLAGS           0x0000000000000202 /* estado inicial de los flags al iniciar la tarea */

/* Video */
/*----------------*/
#define VIDEO_ROWS 50
#define VIDEO_COLUMNS 80

/* Teclado */
/*--------------------------*/
#define KEY_BUFFER_SIZE 256

/* Scancodes */
/*----------------*/
#define KEY_A 0x1E
#define KEY_B 0x30
#define KEY_C 0x2E
#define KEY_D 0x20
#define KEY_E 0x12
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_I 0x17
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_M 0x32
#define KEY_N 0x31
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_Q 0x10
#define KEY_R 0x13
#define KEY_S 0x1F
#define KEY_T 0x14
#define KEY_U 0x16
#define KEY_V 0x2F
#define KEY_W 0x11
#define KEY_X 0x2D
#define KEY_Y 0x15
#define KEY_Z 0x2C

#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0A
#define KEY_0 0x0B

#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define KEY_F3 0x3D
#define KEY_F4 0x3E
#define KEY_F5 0x3F
#define KEY_F6 0x40
#define KEY_F7 0x41
#define KEY_F8 0x42
#define KEY_F9 0x43
#define KEY_F10 0x44
#define KEY_F11 0x57
#define KEY_F12 0x58

#define KEY_ESC 0x01
#define KEY_ENTER 0x1C
#define KEY_SPACE 0x39
#define KEY_TAB 0x0F
#define KEY_BACKSPACE 0x0E
#define KEY_CAPSLOCK 0x3A

#define KEY_UP 0x48 
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D

#endif  /* !__DEFINES_H__ */