/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 

void handler(void);

void task() {
    char* message1 = "Trabajo muy duro";
    char* message2 ="como un esclavo";
    
    syscall_setHandler(handler);
    uint8_t msj=0;
    while(1) { 
    
        if(msj==100)msj=0;
        //alternando mensajes
        if(msj>=0 && msj<50)syscall_talk(message1);
        else syscall_talk(message2);
        
        msj++;
    }
}

void handler() {
    
    syscall_informAction(Center);
}

