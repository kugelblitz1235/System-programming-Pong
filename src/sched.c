/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "defines.h"
#include "screen.h"

void sched_init() {
	scheduler.current_task = 0;
	scheduler.is_current_handler = FALSE;
	scheduler.debug_mode = FALSE;
	scheduler.ticks = 0;
	scheduler.on_interrupt = FALSE;
	// iniciamos el arreglo de las tareas 
	// con el indice de 
	// la entrada en la gdt de la tarea idle
	// y el arreglo de selectores de los handles
	// con los selectores de cada handler (empiezan en 27)
	for (uint8_t i = 0;i < 6;i++) {
		scheduler.task_selectors[i] = IDLE_TASK<<3;
		scheduler.handler_selectors[i] = (i+27)<<3;
		scheduler.handler_ptr[i] = 0;
	}
}

int16_t sched_nextTask() {
	
	if (scheduler.debug_mode && scheduler.on_interrupt) {
		return IDLE_TASK<<3;
	} else {


		scheduler.current_task = (scheduler.ticks) % 7;
		if (scheduler.current_task == 6) {
			// Fin de vuelta: actualizar el juego, dibujar pantalla y cargar tareas
			update_game();
			draw_screen();

			scheduler.ticks++;
			
			return IDLE_TASK<<3;
		} else if (scheduler.handler_ptr[scheduler.current_task] != 0 && !scheduler.is_current_handler) {
			// Tarea: hay handler
			scheduler.is_current_handler = TRUE;
		
			//Copio contexto de la tarea al handler
			handlers[scheduler.current_task] = tareas[scheduler.current_task];
			init_handler_tss(scheduler.current_task,scheduler.handler_ptr[scheduler.current_task]);
			return scheduler.handler_selectors[scheduler.current_task];
		} else {
			// Tarea: no hay handler
			scheduler.is_current_handler = FALSE;
			scheduler.ticks++;

			return scheduler.task_selectors[scheduler.current_task];
		}
	}
}

void set_task_selector(uint8_t task_index,uint16_t task_selector){
	scheduler.task_selectors[task_index] = task_selector;
}

void set_handler_selector(uint8_t handler_index,uint16_t handler_selector){
	scheduler.handler_selectors[handler_index] = handler_selector;
}

uint16_t get_task_selector(uint8_t task_index){
	return scheduler.task_selectors[task_index];
}

uint16_t get_handler_selector(uint8_t handler_index){
	return scheduler.handler_selectors[handler_index];
}

void sched_kill_task(uint8_t task_index){
	scheduler.task_selectors[task_index] = IDLE_TASK<<3;
	scheduler.handler_ptr[task_index] = 0;
}

void set_handler_ptr(f_handler_t handler_ptr){
	scheduler.handler_ptr[scheduler.current_task] = handler_ptr;
}