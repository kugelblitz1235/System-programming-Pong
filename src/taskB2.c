/* * por compatibilidad se omiten tildes *
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/
 
#include "syscall.h"
 
#define ACTION              (0x08000000+1024*5+300)
 
#define G_SEED_ADDRESS              (0x08000000+1024*5+320)
 
void handler(void);
 
void task() {
    syscall_setHandler(handler);
 

    uint32_t* g_seed_ptr = (uint32_t*)G_SEED_ADDRESS;
    uint32_t g_seed = *g_seed_ptr;

    uint32_t x;
    uint32_t y;

    uint32_t* action_ptr =  (uint32_t*)ACTION;
    char*message1 = "No mentira por acaa";
    char*message2 = "Por acaa";
    uint8_t msj=0;
	

		

    while(1) {
        if(msj==30)msj=0;
        g_seed = (214013*g_seed+2531011);
        *g_seed_ptr = g_seed;
 
        syscall_where(&x,&y);
        if(msj>=0 && msj<15)syscall_talk(message1);
        else syscall_talk(message2);
        
        *action_ptr = ((g_seed>>16)&0x7FFF)%3+1;
        msj++;
    }
}
 
void handler() {
    
    uint32_t* action_ptr =  (uint32_t*)ACTION;  
    
    syscall_informAction(*action_ptr);
}