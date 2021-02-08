; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

%define key_w 0x11
%define key_s 0x1F
%define key_z 0x2C
%define key_x 0x2D
%define key_c 0x2E
%define key_i 0x17
%define key_k 0x25
%define key_b 0x30
%define key_n 0x31
%define key_m 0x32
%define key_y 0x15


%define PLAYER_A          0
%define PLAYER_B          1
%define MOVE_PADDLE_UP    0
%define MOVE_PADDLE_DOWN  1
%define NEW_BALL_TYPE_1   0
%define NEW_BALL_TYPE_2   1
%define NEW_BALL_TYPE_3   2

%define IDLE_TASK 20

BITS 32

sched_task_offset:     dd 0xFFFF
sched_task_selector:   dw 0xFFFFFFFF

int_pred_msj db     'Interrupcion:',0

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask

;; Screen
extern screen_drawBox
extern print
extern print_dec
extern print_hex
extern draw_debug_panel
extern set_exception_type
;; Player's paddles actions
extern key_pressed_handler

;; Game
extern set_task_handler
extern set_ball_action
extern set_ball_msg
extern get_ball_position
extern kill_ball
extern kill_actual_task
extern check_bad_handler
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

extern set_interrupt
%macro ISR 1
global _isr%1



_isr%1:
  ;xchg bx,bx
    ;ocurrio una excepcion entonces seteamos la interrupcion para el modo debug
    call set_interrupt
   
    ;si ocurrio una excepcion en modo debug 
    ;{"cs","ds","es","fs","gs","ss"}
    sub esp, 48
    mov word[esp], cs         ;cs
    mov word[esp + 2], ds     ;ds
    mov word[esp + 4], es   ;es
    mov word[esp + 6], fs     ;fs
    mov word[esp + 8], gs     ;gs
    mov word [esp + 10], ss    ;ss
    mov dword [esp + 12], eax     ;eax
    mov dword [esp + 16], ebx     ;ebx
    mov dword [esp + 20], ecx     ;ecx
    mov dword [esp + 24], edx     ;edx
    mov dword[esp + 28], esi     ;esi
    mov dword[esp + 32], edi     ;edi 
    mov dword[esp + 36], ebp     ;ebp
    mov dword[esp + 40], esp
    mov eax, [esp + 56] ;Guardamos eip en eax
    mov dword [esp + 44],eax ;eip
    
    pushfd                ;pusheo los 32 bits de eflags al stack
    sub esp,3*4
    ;segmentos
    mov [esp],esp
    add dword [esp],28
    ;type
    mov byte[esp + 4],%1
    ;registers
    mov [esp + 8],esp
    add dword [esp + 8],16 
    call set_exception_type
    add esp,3*4
    popfd                 ;por los eflags
    add esp,48         ;por los registros
    ; Matamos a la tarea
    call kill_actual_task    
    ; Llamamos a la siguiente
    ; call sched_nextTask
    mov ax, IDLE_TASK<<3
    ;asumo que el indice siempre va a ser válido
    str dx
    cmp ax,dx
    je .end ;This will never happen because we are on a handler
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]
  .end:
    iret
%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
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

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
;para saltar a la tarea
offset: dd 0    ;32 bits
selector: dw 0  ;16 bits

%define NULL_SELECTOR 0x0

global _isr32
_isr32:
  pushad ;preservo los registros
  call pic_finish1
  ;Chequeamos si el handler no hizo informAction
  call check_bad_handler
  ;informamos al pic que atendimos la interrupción
  call sched_nextTask
  ;asumo que el indice siempre va a ser válido
  str dx
  cmp ax,dx
  je .only_idle_task
  mov [selector], ax
  ;xchg bx,bx
  jmp far [offset]
  jmp .end
  .only_idle_task:
  call nextClock
  .end:
  popad
  iret
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
  pushad
  call pic_finish1
  xor eax, eax
  in al, 0x60
  mov dl,al
  shr dl,7
  cmp dl,0x01
  je .end

  ;Se solto la tecla 
  mov dl,al
  and dl,0x80
  cmp dl,0x80
  je .end
  push eax
  call key_pressed_handler
  add esp,4
  .end:
  popad
  iret
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr47

  _isr47:
  mov edx,eax
  and edx,0x80000000
  cmp edx,0x80000000
  jne .end
  and eax, 0xFFFF
  cmp eax, 0x0001
  je .talk
  cmp eax, 0x0002
  je .where
  cmp eax, 0x0003
  je .setHandler
  cmp eax, 0xFFFF
  je .informAction
  ;si no es una syscall valida termina
  jmp .end
  .talk:
    ;eax = 0x80000001
    ;ebx= char* m
    pushad
    push ebx
    call set_ball_msg
    pop ebx
    popad
    jmp .end
  .where:
    ;ebx = uint32_t x
    ;ecx = uint32_t y
    ;eax = 0x80000002
    sub esp,8
    pushad
    mov eax,esp
    add eax,8*4
    push eax
    call get_ball_position
    add esp,4
    popad
    pop ebx
    pop ecx
 
    jmp .end
  
  .setHandler:
    ;ebx = f_handler_t *f
    push ebx
    call set_task_handler
    pop ebx
    jmp .end
  
  .informAction:
  ;eax=0x8000FFFF
  ;ebx = e_action action
    cmp ebx,3
    jg .kill_task
    cmp ebx,1
    jl .kill_task
    push ebx
    call set_ball_action
    pop ebx
    call sched_nextTask
    ;asumo que el indice siempre va a ser válido
    str dx
    cmp ax,dx
    je .end ;This will never happen because we are on a handler
    mov [selector], ax
    jmp far [offset]
  .end:
  iret

  .kill_task:
    ; Matamos a la tarea
    call kill_actual_task    
    ; Llamamos a la siguiente
    ; call sched_nextTask
    mov ax, IDLE_TASK<<3
    ;asumo que el indice siempre va a ser válido
    str dx
    cmp ax,dx
    je .end ;This will never happen because we are on a handler
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]
    iret
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret

