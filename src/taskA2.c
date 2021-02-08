/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 

void handler(void);

void task() {
    char* message = "wooohoooooo";
   
    syscall_setHandler(handler);

    while(1) {  syscall_talk(message); }
}

void handler() {
    
    syscall_informAction(Up);
}
