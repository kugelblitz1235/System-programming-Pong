/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

#define PAGE_INDEX_DIR(virtual) (virtual>>22)<<2
#define PAGE_INDEX_TABLE(virtual) ((virtual<<10)>>22)<<2

typedef struct str_pde {
    uint8_t   present:1;
    uint8_t   rw:1;
    uint8_t   us:1;
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   a:1;
    uint8_t   x:1;
    uint8_t   ps:1;
    uint8_t   g:1;
    uint8_t   avl:3;
    uint32_t  base_tabla:20;
} __attribute__((__packed__, aligned (4))) PDE;

typedef struct str_pte{
    uint8_t   present:1;
    uint8_t   rw:1;
    uint8_t   us:1;
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   a:1;
    uint8_t   d:1;
    uint8_t   pat:1;
    uint8_t   g:1;
    uint8_t   avl:3;
    uint32_t  base_pagina:20;
} __attribute__((__packed__, aligned (4))) PTE;

void mmu_init();

uint32_t mmu_nextFreeKernelPage();

uint32_t mmu_nextFreeTaskPage();

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs);

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3);

uint32_t mmu_initTaskDir(uint8_t slot_tarea);

uint32_t mmu_initKernelDir();

void mmu_init_task_structures();

void mmu_copy_task_code(uint8_t player,uint8_t type,uint8_t slot_tarea);

// Custom struct
typedef struct {
	uint32_t tareas[6];
	uint32_t SS0_tareas[6];
	uint32_t SS0_handlers[6];
	uint32_t directorios[6]; // Puntero a directorio en la gdt
	//uint8_t tareas_cargadas[6];
	//uint8_t handlers_cargadas[6];
}Mapa_paginas_fisicas_t;

Mapa_paginas_fisicas_t mapa_paginas_fisicas;

#endif	/* !__MMU_H__ */




