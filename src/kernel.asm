; ** por compatibilidad se omiten tildes **
; ==================================================================================
; TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==================================================================================

;;
;; Defines
;; -------------------------------------------------------------------------- ;;

SEL_CODE_32 equ 0x0008
SEL_CODE_64 equ 0x0010
SEL_DATA equ 0x0020
SEL_TSS equ 0x0030
IDT_SIZE equ 0x1000


extern deshabilitar_pic
extern resetear_pic
extern habilitar_pic
extern inicializar_idt
extern inicializar_tss
extern inicializar_sched
extern add_task

global start
global idt_desc
global process_info
global actual_process
global actual_key
global next_key
global keys

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 25 colour text
    mov ax, 0x0003
    int 0x10 ; set mode 03h
    xor bx, bx
    mov ax, 0x1112
    int 0x10 ; load 8x8 font (Queda video a 80 X 50)
    
    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    lgdt [gdt_descriptor_protected]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    ; Saltar a modo protegido
    jmp SEL_CODE_32:protegido
    
BITS 32
protegido:

    ; Cargar segmento de datos
    mov ax, SEL_DATA
    mov ds, ax
    mov ss, ax

    ; Limpiar al resto
    xor ax, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Limpiar parte en donde colocar estructuras de paginacion
    mov eax, 0x00080000
    mov ecx, 0x00001000
    .limpiar:
    mov dword [eax+ecx*4-4], 0
    loop .limpiar

    ; Crear mapa de paginacion
    mov dword [0x00080000], 0x00081003 ; PML4E
    mov dword [0x00081000], 0x00082003 ; PDPTE
    mov dword [0x00082000], 0x00083003 ; PDE (Kernel)
    mov dword [0x00082008], 0x00084003 ; PDE (Tasks)
    mov eax, 0x00083000
    mov ebx, 0x001FF003
    mov ecx, 0x00000200
    mov edx, 0x00084000
    mov edi, 0x003FF003
    .llenar:
    mov dword [eax + ecx*8 - 8], ebx ; PTE (Kernel)
    mov dword [edx + ecx*8 - 8], edi ; PTE (Tasks)
    sub ebx, 0x00001000
    sub edi, 0x00001000
    loop .llenar

    ; Setear el bit PAE del registro CR4
    mov eax, cr4
    or eax, 0x00000020
    mov cr4, eax

    ; Inicializar CR3 con la direccion fisica en que inicia el Page Map Directory Level 4 (PLM4)
    mov eax, 0x00080000
    mov cr3, eax

    ; Leer MSR EFER
    mov ecx, 0xC0000080
    rdmsr

    ; Setear el bit LME del MSR EFER
    or eax, 0x00000100
    wrmsr

    ; Setear el bit PG del registro CR0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Saltar a codigo 64
    jmp SEL_CODE_64:largo
    
BITS 64
largo:

    mov eax,0x80000008
    cpuid

    ; Setear pila del kernel
    mov rsp, 0x0000000000080000

    ; Inicializar la IDT
    call inicializar_idt

    ; Cargar IDT
    lidt [idt_desc]

    ; Configurar controlador de interrupciones
    call deshabilitar_pic
    call resetear_pic
    call habilitar_pic

    ; Inicializar scheduler
    call inicializar_sched

    ; Inicializar tss
    call inicializar_tss

    ; Cargar tss
    mov ax, SEL_TSS
    ltr ax

    ; Cargar tarea master
    mov rdi, 0
    mov rsi, 0x001F
    mov rdx, 0
    call add_task

    ; Habilitar interrupciones
    sti
    
    mov rax, 0xFFFFFFFFFFFFFFFF
    mov rbx, 0xFFFFFFFFFFFFFFFF
    mov rcx, 0xFFFFFFFFFFFFFFFF
    mov rdx, 0xFFFFFFFFFFFFFFFF
    mov rdi, 0xFFFFFFFFFFFFFFFF
    mov rsi, 0xFFFFFFFFFFFFFFFF
    mov r8, 0xFFFFFFFFFFFFFFFF
    mov r9, 0xFFFFFFFFFFFFFFFF
    mov r10, 0xFFFFFFFFFFFFFFFF
    mov r11, 0xFFFFFFFFFFFFFFFF
    mov r12, 0xFFFFFFFFFFFFFFFF
    mov r13, 0xFFFFFFFFFFFFFFFF
    mov r14, 0xFFFFFFFFFFFFFFFF
    mov r15, 0xFFFFFFFFFFFFFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;

align 16

gdt_protected:
    dq 0x0000000000000000 ;Descriptor nulo
    dq 0x00CF9A000000FFFF ;Descriptor codigo nivel 0 readable 32 bit
    dq 0x00209A0000000000 ;Descriptor codigo nivel 0 readable 64 bit
    dq 0x0020FA0000000000 ;Descriptor codigo nivel 3 readable 64 bit
    dq 0x00CF92000000FFFF ;Descriptor datos nivel 0 writable
    dq 0x0000F20000000000 ;Descriptor datos nivel 3 writable
    dq 0x0000890850000067 ;Descriptor TSS low
    dq 0x0000000000000000 ;Descriptor TSS high

gdt_descriptor_protected:
    dw gdt_descriptor_protected - gdt_protected - 1      ;Limit
    dd gdt_protected                                     ;Base

idt_desc: times 10 db 0

process_info: times 256 db 0 ; Array of 32 pi

actual_process: db 0

actual_key: db 0

next_key: db 0

keys: times 256 db 0