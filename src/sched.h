/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"

typedef struct{
	uint8_t current_task;
	uint8_t is_current_handler;
	uint8_t debug_mode;
	uint32_t ticks;
	uint16_t task_selectors[6];
	uint16_t handler_selectors[6];
	f_handler_t handler_ptr[6];
	uint8_t on_interrupt;
} Scheduler;

Scheduler scheduler;

void sched_init();

int16_t sched_nextTask();

void set_handler_ptr(f_handler_t handler_ptr);

void set_task_selector(uint8_t task_index,uint16_t task_selector);
void set_handler_selector(uint8_t handler_index,uint16_t handler_selector);

uint16_t get_task_selector(uint8_t task_index);
uint16_t get_handler_selector(uint8_t handler_index);

void sched_kill_task(uint8_t task_index);

void activate_debug_mode();

#endif	/* !__SCHED_H__ */
