/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "defines.h"
#include "screen.h"

unsigned int proxima_pagina_libre_kernel;

unsigned int proxima_pagina_libre_task;

void mmu_init(){
	proxima_pagina_libre_kernel = INICIO_PAGINAS_LIBRES_KERNEL;
	proxima_pagina_libre_task = INICIO_PAGINAS_LIBRES_TASK;
}		

uint32_t mmu_nextFreeTaskPage() {
	unsigned int pagina_libre = proxima_pagina_libre_task;
	proxima_pagina_libre_task += PAGE_SIZE;
	return pagina_libre;
}

uint32_t mmu_nextFreeKernelPage() {
	unsigned int pagina_libre = proxima_pagina_libre_kernel;
	proxima_pagina_libre_kernel += PAGE_SIZE;
	return pagina_libre;
}


void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs) {
	uint32_t index_dir_page = PAGE_INDEX_DIR(virtual); 
	uint32_t index_tab_page= PAGE_INDEX_TABLE(virtual); 

	PDE* PDE_entry=(PDE*) (((cr3>>12)<<12 )+ index_dir_page); //limpiamos los flags de cr3 y nos quedamos con la direccion
	if(!PDE_entry->present){
		//si la entrada de tabla no esta definida 		
		PTE* new_table=(PTE*)mmu_nextFreeKernelPage();
		PDE_entry->present=1;
		PDE_entry->rw=1;
		PDE_entry->us=1;
		PDE_entry->pwt=0;
		PDE_entry->pcd=0;
		PDE_entry->a=0;
		PDE_entry->x=0;
		PDE_entry->ps=0;
		PDE_entry->g=0;
		PDE_entry->avl=0;
		//ponemos la direccion de la tabla en la entrada del directorio
		PDE_entry->base_tabla=(((uint32_t)new_table)>>12);

	}
	PTE* PTE_entry = (PTE*) (((PDE_entry->base_tabla)<<12) + index_tab_page);
	//seteamos los atributos
	union
	{
		uint32_t attrs;
		PTE PTE_struct;
	} un;
	un.attrs = attrs;
	(*PTE_entry) = un.PTE_struct;
	//colocamos la direccion fisica
	PTE_entry->base_pagina= phy>>12;

	tlbflush();

}

uint32_t mmu_unmapPage(uint32_t virtual, uint32_t cr3) {
	uint32_t index_dir_page = PAGE_INDEX_DIR(virtual); 
	uint32_t index_tab_page= PAGE_INDEX_TABLE(virtual);

	PDE* PDE_entry=(PDE*) (((cr3>>12)<<12 )+ index_dir_page);

	//accedemos a PTE_entry
	PTE* PTE_entry = (PTE*) (((PDE_entry->base_tabla)<<12 )+ index_tab_page);
	//ponemos el bit present en 0
	PTE_entry->present=0;

	tlbflush();

    return 0;
}

void mmu_copy_task_code(uint8_t player,uint8_t type,uint8_t slot_tarea){

	uint32_t dir_tarea_kernel = INITIAL_TASK_CODES_ADDRESS + type*0x2000 + player*0x6000;

	uint32_t task_code_page = mapa_paginas_fisicas.tareas[slot_tarea];

	// Mapeo la primer pagina de codigo para copiar el codigo de la tarea desde el kernel
	mmu_mapPage(task_code_page, rcr3(), task_code_page, PAG_P|PAG_RW|PAG_S);
	
	// Mapeo la segunda pagina de codigo para copiar el codigo de la tarea desde el kernel
	mmu_mapPage(task_code_page+0x1000, rcr3(), task_code_page+0x1000, PAG_P|PAG_RW|PAG_S);

	// Copiar el codigo de la tarea
	for (int i = 0; i < (2*4096); ++i) {

		((uint8_t*) task_code_page)[i] = ((uint8_t*) dir_tarea_kernel)[i];
	}

	// Desmapeo en dir del kernel
	mmu_unmapPage(task_code_page, rcr3());

	// Desmapeo en dir del kernel
	mmu_unmapPage(task_code_page+0x1000, rcr3());

	tlbflush();

}

uint32_t mmu_initTaskDir(uint8_t slot_tarea) {
	// player tiene que ser 0 o 1
	// type tiene que ser 0, 1 o 2

	PDE* dir = (PDE*) mapa_paginas_fisicas.directorios[slot_tarea];
	for(int i =0;i<size_dir;++i){
		//limpiamos cada entrada 
		dir[i].present = 0;
	}
	PTE* kernel_table = (PTE*) KERNEL_PAGE_TABLE_0;

	// Entrada de la tabla con las paginas del kernel
	dir[0].present=1;
	dir[0].rw=0;
	dir[0].us=1;
	dir[0].pwt=0;
	dir[0].pcd=0;
	dir[0].a=0;
	dir[0].x=0;
	dir[0].ps=0;
	dir[0].g=0;
	dir[0].avl=0;
	dir[0].base_tabla=(uint32_t)(kernel_table)>>12;

	// Mapear la nueva tarea
	uint32_t task_code_page = mapa_paginas_fisicas.tareas[slot_tarea];
	mmu_mapPage(TASK_CODE, (uint32_t)dir, task_code_page, PAG_P|PAG_RW|PAG_US);
	//Mapeo la segunda pagina de la nueva tarea
	mmu_mapPage(TASK_CODE+0x1000, (uint32_t)dir, task_code_page+0x1000, PAG_P|PAG_RW|PAG_US);

	//tlbflush();

	return (uint32_t)dir;
}

uint32_t mmu_initKernelDir() {
	PDE* dir = (PDE *) KERNEL_PAGE_DIR;
	for(int i =0;i<size_dir;++i){
		//limpiamos cada entrada 
		dir[i].present = 0;
	}
	// Entrada de la tabla de kernel (id mapping)
	dir[0].present=1;
	dir[0].rw=1;
	dir[0].us=0;
	dir[0].pwt=0;
	dir[0].pcd=0;
	dir[0].a=0;
	dir[0].x=0;
	dir[0].ps=0;
	dir[0].g=0;
	dir[0].avl=0;
	dir[0].base_tabla=KERNEL_PAGE_TABLE_0>>12;
	
	//inicializamos las entradas de cada tabla
	PTE* table= (PTE*) KERNEL_PAGE_TABLE_0;
	for(uint32_t i =0; i<size_table;++i){
		table[i].present=1;
		table[i].rw=1;
		table[i].us=0;
		table[i].pwt=0;
		table[i].pcd=0;
		table[i].a=0;
		table[i].d=0;
		table[i].pat=0;
		table[i].g=0;
		table[i].avl=0;
		table[i].base_pagina=i;
	}
	
    return 0;
}

void mmu_init_task_structures() {
	for (uint8_t i = 0; i < 6; i++) {
		mapa_paginas_fisicas.tareas[i] =	mmu_nextFreeTaskPage();
		mmu_nextFreeTaskPage(); // Esto es porque cada tarea tiene 2 paginas

		mapa_paginas_fisicas.directorios[i] =	mmu_nextFreeKernelPage();

		mapa_paginas_fisicas.SS0_tareas[i] =	mmu_nextFreeKernelPage();
		
		mapa_paginas_fisicas.SS0_handlers[i] =	mapa_paginas_fisicas.SS0_tareas[i];

		mmu_initTaskDir(i);
	}
	
}





