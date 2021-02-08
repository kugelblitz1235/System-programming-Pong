/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "gdt.h"
#include "mmu.h"
#include "defines.h"

tss tss_initial;
tss tss_idle;


void tss_init() {  
  // Tarea inicial
  gdt[INITIAL_TASK].base_0_15 = ((uint32_t)&tss_initial);
  gdt[INITIAL_TASK].base_23_16 = ((uint32_t)&tss_initial)>>16;
  gdt[INITIAL_TASK].base_31_24 = ((uint32_t)&tss_initial)>>24;

  tss_initial.cr3 = rcr3();
  tss_initial.ss0 = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_initial.ss  = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_initial.cs = SEG_SELECTOR(KERNEL_CODE_SEGMENT);
  tss_initial.ds = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_initial.gs = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_initial.fs = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_initial.es = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_initial.eip = IDLE_CODE;
  tss_initial.esp = KERNEL_STACK;
  tss_initial.esp0 = KERNEL_STACK;
  tss_initial.eflags = 0x202;
  tss_initial.unused0 = 0;
  tss_initial.unused1 = 0;
  tss_initial.unused2 = 0;
  tss_initial.unused3 = 0;
  tss_initial.unused4 = 0;
  tss_initial.unused5 = 0;
  tss_initial.unused6 = 0;
  tss_initial.unused7 = 0;
  tss_initial.unused8 = 0;
  tss_initial.unused9 = 0;
  tss_initial.unused10 = 0;

  // Tareas usuario
  for (int i = 0; i < 6; i++)
  {
    // Apuntamos el descriptor a la TSS
    gdt[i+21].base_0_15 = ((uint32_t)&(tareas[i]));
    gdt[i+21].base_23_16 = ((uint32_t)&(tareas[i]))>>16;
    gdt[i+21].base_31_24 = ((uint32_t)&(tareas[i]))>>24;
    
    // Selectores de segmento de la TSS
    tareas[i].cs = SEG_SELECTOR(USER_CODE_SEGMENT) | 0b11;  // RPL 11
    tareas[i].ss = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[i].ds = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[i].gs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[i].fs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[i].es = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[i].ss0 = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
    tareas[i].eflags = 0x202;
    tareas[i].cr3 = mapa_paginas_fisicas.directorios[i] & (~(0xFFF));
    
    // Reservados
    tareas[i].unused0 = 0;
    tareas[i].unused1 = 0;
    tareas[i].unused2 = 0;
    tareas[i].unused3 = 0;
    tareas[i].unused4 = 0;
    tareas[i].unused5 = 0;
    tareas[i].unused6 = 0;
    tareas[i].unused7 = 0;
    tareas[i].unused8 = 0;
    tareas[i].unused9 = 0;
    tareas[i].unused10 = 0;
  }

  // Handlers usuario
  for (int i = 0; i < 6; i++)
  {
    gdt[i+27].base_0_15 = ((uint32_t)&(handlers[i]));
    gdt[i+27].base_23_16 = ((uint32_t)&(handlers[i]))>>16;
    gdt[i+27].base_31_24 = ((uint32_t)&(handlers[i]))>>24;

    //selectores de segmento en la tss
    handlers[i].cs = SEG_SELECTOR(USER_CODE_SEGMENT) | 0b11;  // RPL 11
    handlers[i].ss = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[i].ds = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[i].gs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[i].fs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[i].es = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[i].ss0 = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
    handlers[i].eflags = 0x202;
    handlers[i].cr3 = mapa_paginas_fisicas.directorios[i] & (~(0xFFF));
    // Reservados
    handlers[i].unused0 = 0;
    handlers[i].unused1 = 0;
    handlers[i].unused2 = 0;
    handlers[i].unused3 = 0;
    handlers[i].unused4 = 0;
    handlers[i].unused5 = 0;
    handlers[i].unused6 = 0;
    handlers[i].unused7 = 0;
    handlers[i].unused8 = 0;
    handlers[i].unused9 = 0;
    handlers[i].unused10 = 0;

  }
  
}

void tss_init_idle(){
  gdt[IDLE_TASK].base_0_15 = ((uint32_t)&tss_idle);
  gdt[IDLE_TASK].base_23_16 = ((uint32_t)&tss_idle)>>16;
  gdt[IDLE_TASK].base_31_24 = ((uint32_t)&tss_idle)>>24;

  tss_idle.cr3 = rcr3();
  tss_idle.ss0 = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_idle.ss  = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_idle.cs = SEG_SELECTOR(KERNEL_CODE_SEGMENT);
  tss_idle.ds = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_idle.gs = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_idle.fs = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_idle.es = SEG_SELECTOR(KERNEL_DATA_SEGMENT);
  tss_idle.eip = IDLE_CODE;
  tss_idle.esp = KERNEL_STACK;
  tss_idle.esp0 = KERNEL_STACK;
  tss_idle.eflags = 0x202;
  tss_idle.unused0 = 0;
  tss_idle.unused1 = 0;
  tss_idle.unused2 = 0;
  tss_idle.unused3 = 0;
  tss_idle.unused4 = 0;
  tss_idle.unused5 = 0;
  tss_idle.unused6 = 0;
  tss_idle.unused7 = 0;
  tss_idle.unused8 = 0;
  tss_idle.unused9 = 0;
  tss_idle.unused10 = 0;


}

void init_task_tss(uint8_t slot_tarea){
    // Punteros de la TSS
    tareas[slot_tarea].cs = SEG_SELECTOR(USER_CODE_SEGMENT) | 0b11;  // RPL 11
    tareas[slot_tarea].ss = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[slot_tarea].ds = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[slot_tarea].gs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[slot_tarea].fs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[slot_tarea].es = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    tareas[slot_tarea].eip = TASK_CODE;
    tareas[slot_tarea].esp = TASK_CODE + 7*1024-4; // + 7kb
    tareas[slot_tarea].esp0 = mapa_paginas_fisicas.SS0_tareas[slot_tarea]+2*1024-4;
    tareas[slot_tarea].eflags = 0x202;
}

void init_handler_tss(uint8_t slot_tarea,f_handler_t f_handler){
    // Punteros de la TSS
    handlers[slot_tarea].cs = SEG_SELECTOR(USER_CODE_SEGMENT) | 0b11;  // RPL 11
    handlers[slot_tarea].ss = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[slot_tarea].ds = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[slot_tarea].gs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[slot_tarea].fs = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[slot_tarea].es = SEG_SELECTOR(USER_DATA_SEGMENT) | 0b11;  // RPL 11
    handlers[slot_tarea].eip = (uint32_t)f_handler;
    handlers[slot_tarea].esp = TASK_CODE + 8*1024-4; // + 8kb
    handlers[slot_tarea].esp0 = mapa_paginas_fisicas.SS0_handlers[slot_tarea]+4*1024-4;
    handlers[slot_tarea].eflags = 0x202;
}

//======================================================= KERNEL.c


uint16_t tss_init_task(uint8_t player,uint8_t type,uint8_t task_index){

  //Copy the code
  mmu_copy_task_code(player,type,task_index);

	init_task_tss(task_index);

  return (21 + task_index)<<3;
}
