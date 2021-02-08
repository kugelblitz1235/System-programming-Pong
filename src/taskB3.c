/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 

void handler(void);

void task() {
    char* message = "YEEEEEEEEE";
    
    syscall_setHandler(handler);
    //seteaba dos veces el handler, era para demostrar que matabamos correctamente a las tareas
    //syscall_setHandler(handler);

    while(1) { syscall_talk(message); }
}

void handler() {
    syscall_informAction(Down);
}
