/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* Misc */
/* -------------------------------------------------------------------------- */
#define SEG_SELECTOR(x) x << 3
/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 33

#define GDT_IDX_NULL_DESC 0

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */
#define KERNEL_CODE_SEGMENT 14
#define USER_CODE_SEGMENT 15
#define KERNEL_DATA_SEGMENT 16
#define USER_DATA_SEGMENT 17
#define SCREEN_SEGMENT 18
#define INITIAL_TASK 19
#define IDLE_TASK 20
#define TASK_A_1 21
#define TASK_A_2 22
#define TASK_A_3 23 
#define TASK_B_1 24
#define TASK_B_2 25 
#define TASK_B_3 26
#define HANDLERS_A_1 27
#define HANDLERS_A_2 28
#define HANDLERS_A_3 29
#define HANDLERS_B_1 30
#define HANDLERS_B_2 31
#define HANDLERS_B_3 32

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE             		0x08000000 /* direccion virtual del codigo */
#define IDLE_CODE             		0x1C000
#define INITIAL_TASK_CODES_ADDRESS 	0x10000
/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR          		0x0002B000
#define KERNEL_PAGE_TABLE_0      		0x0002C000
#define size_dir 						1024
#define size_table 						1024
#define INICIO_PAGINAS_LIBRES_KERNEL 	0x00100000
#define INICIO_PAGINAS_LIBRES_TASK	 	0x00400000
#define PAGE_SIZE						4096
#define KERNEL_STACK                    0x2A000
#endif  /* !__DEFINES_H__ */


typedef void (*f_handler_t)();
