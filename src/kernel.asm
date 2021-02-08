; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC
extern screen_drawBox
extern pic_reset
extern pic_enable
extern idt_inicializar
extern IDT_DESC
extern mmu_init
extern mmu_initKernelDir
extern mmu_initTaskDir
extern mmu_init_task_structures
extern tss_init
extern tss_init_idle
extern sched_init
extern init_task_dir
extern init_handler_dir

extern game_init;
extern game_board_init;

extern init_debug_panel_config;

%define KERNEL_CODE_SEGMENT 14
%define USER_CODE_SEGMENT 15
%define KERNEL_DATA_SEGMENT 16
%define USER_DATA_SEGMENT 17
%define SCREEN_SEGMENT 18
%define KERNEL_PAGE_DIR 0x2B000
%define INITIAL_TASK 19
%define IDLE_TASK 20
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    ; ax = 1003h -> para poder tener 16 colores de background
    mov ax, 1003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    ; Habilitar A20
    call A20_enable
    ; Cargar la GDT
    
    lgdt [GDT_DESC]    
    ; Setear el bit PE del registro CR0

    mov eax, cr0
    or eax, 0x0001
    mov cr0, eax

    ; Saltar a modo protegido
    jmp KERNEL_CODE_SEGMENT<<3:.offset

.offset:
BITS 32  
      
    ; Establecer selectores de segmentos
    mov ax, KERNEL_DATA_SEGMENT<<3   ; Segmento de datos del kernel
    ; mov es, ax
    ; mov fs, ax
    ; mov gs, ax
    mov ss, ax
    mov ds, ax

    ; Establecer la base de la pila
    mov esp, 0x2B000         ; setear la pila del kernel

    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 2, 0
    
    mov ax, SCREEN_SEGMENT<<3
    mov fs, ax
    
    ;blanco
    ;screen_drawBox_macro 40, 80, 0xF0, 0, 0
    ;gris
    ;screen_drawBox_macro 40, 78, 0x70, 0, 1
    ;negro
    ;screen_drawBox_macro 10, 80, 0x00, 40, 0
    ;rojo
    ;screen_drawBox_macro 8, 38, 0xC0, 41, 1
    ;azul
    ;screen_drawBox_macro 8, 38, 0x90, 41, 41
    ;player_red
    ;screen_drawBox_macro 8, 1, 0xC0, 16, 0
    ;player_blue
    ;screen_drawBox_macro 8, 1, 0x90, 16, 79

    ; Inicializar el manejador de memoria
    call mmu_init
    ; Inicializar el directorio de paginas
    call mmu_initKernelDir

   
    ; Cargar directorio de paginas
    mov eax, KERNEL_PAGE_DIR; direccion de directorio, en este caso coincide
                            ; pues pcd=0 y pwt=0

    mov cr3, eax            ; ponemos en cr3 la direccion del directorio
    ; Habilitar paginacion
    mov eax, cr0            
    or eax, (1<<31)         ;habilitamos paginacion
    mov cr0, eax    

    ; Inicializar estructuras del kernel para manejar tareas
    call mmu_init_task_structures

    ; Inicializar tss
    call tss_init
    ; Inicializar tss de la tarea Idle
    call tss_init_idle
    ; Inicializar el scheduler
    call sched_init
    ; Inicializar la IDT
    call idt_inicializar
    
    ; Cargar IDT
    lidt [IDT_DESC]

    ; Inicializar estructuras del juego
    call game_init
    call init_debug_panel_config
    ; int3
    ; mov edx, 0
    ; mov eax, 20
    ; mov ecx, 0
    ; div ecx

    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable
    

    ; Cargar tarea inicial
    mov ax, INITIAL_TASK << 3
    ltr ax
    ; Habilitar interrupciones
    sti
    ;xchg bx,bx
    ; Saltar a la primera tarea: Idle
    ; xchg bx,bx
    ;jmp IDLE_TASK<<3:0x0
    
    ;xchg bx,bx
    
    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
