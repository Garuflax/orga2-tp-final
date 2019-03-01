; ** por compatibilidad se omiten tildes **
; ==================================================================================
; TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==================================================================================
; definicion de rutinas de atencion de interrupciones

%include "isr.mac"

BITS 64

extern fin_intr_pic1
extern get_current_pcb
extern get_current_ds
extern next
extern remove_current_task
extern add_key
extern manage_syscall

global isr32
global isr33
global isr255

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20

isr32:
    PUSHALL
    call fin_intr_pic1
    ; Save state
    call get_current_pcb
    mov rbx,[rsp + 120]
    mov [rax + 136],rbx ; Guardo rip
    mov rbx,[rsp + 136]
    mov [rax + 128],rbx ; Guardo rflags
    mov rbx, [rsp]
    mov [rax + 120], rbx ; Guardo r15
    mov rbx, [rsp + 8]
    mov [rax + 112], rbx ; Guardo r14
    mov rbx, [rsp + 16]
    mov [rax + 104], rbx ; Guardo r13
    mov rbx, [rsp + 24]
    mov [rax + 96], rbx ; Guardo r12
    mov rbx, [rsp + 32]
    mov [rax + 88], rbx ; Guardo r11
    mov rbx, [rsp + 40]
    mov [rax + 80], rbx ; Guardo r10
    mov rbx, [rsp + 48]
    mov [rax + 72], rbx ; Guardo r9
    mov rbx, [rsp + 56]
    mov [rax + 64], rbx ; Guardo r8
    mov rbx, [rsp + 144]
    mov [rax + 56], rbx ; Guardo rsp
    mov rbx, [rsp + 112]
    mov [rax + 48], rbx ; Guardo rbp
    mov rbx, [rsp + 72]
    mov [rax + 40], rbx ; Guardo rdi
    mov rbx, [rsp + 64]
    mov [rax + 32], rbx ; Guardo rsi
    mov rbx, [rsp + 80]
    mov [rax + 24], rbx ; Guardo rdx
    mov rbx, [rsp + 88]
    mov [rax + 16], rbx ; Guardo rcx
    mov rbx, [rsp + 96]
    mov [rax + 8], rbx ; Guardo rbx
    mov rbx, [rsp + 104]
    mov [rax], rbx ; Guardo rax
    POPALL
    sub rsp, 8 ; Alineo pila
    call next
    mov [rsp + 16],ax ; Cargo CS
    call get_current_ds
    mov [rsp + 40],ax ; Cargo SS
    ; Load state
    call get_current_pcb
    add rsp, 8 ; Desalineo pila
    mov rbx, [rax + 136]
    mov [rsp], rbx ; Cargo rip
    mov rbx, [rax + 128]
    mov [rsp + 16], rbx ; Cargo rflags
    mov rbx, [rax + 56]
    mov [rsp + 24], rbx ; Cargo rsp
    mov rbx, [rax]
    push rbx
    mov rbp, [rax + 48]
    mov rbx, [rax + 8]
    mov rcx, [rax + 16]
    mov rdx, [rax + 24]
    mov rdi, [rax + 40]
    mov rsi, [rax + 32]
    mov r8, [rax + 64]
    mov r9, [rax + 72]
    mov r10, [rax + 80]
    mov r11, [rax + 88]
    mov r12, [rax + 96]
    mov r13, [rax + 104]
    mov r14, [rax + 112]
    mov r15, [rax + 120]
    mov ax, [rsp + 40]
    mov ds,ax ; Cargo DS
    pop rax
    iretq

isr33:
	PUSHALL ; PILA ALINEADA
    call fin_intr_pic1
    in al, 0x60
    xor rdi, rdi
    mov dil, al
    call add_key
    POPALL
    iretq

isr255:
	PUSHALL ; PILA ALINEADA
	call manage_syscall
	mov [rsp + 104], rax
	POPALL
	iretq