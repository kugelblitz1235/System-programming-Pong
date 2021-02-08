/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 

#define RES              (0x08000000+1024*5+300)
#define UP             	 (0x08000000+1024*5+304)

void handler(void);

void task() {
    char* message1 = "a la cargaaaa";
    char* message2= "desvio mortal";
	syscall_setHandler(handler);

    uint32_t* res = (uint32_t*) RES;

    uint32_t last_x,last_y;
	uint32_t x,y;

    while(1) {
		x = last_x;
		y = last_y;

		while(x == last_x && y == last_y){
			syscall_where(&x,&y);	
		}

		if(x < 60) {
			*res = 0;
		} else {
			*res = 1;
		}
		*res==0 ? syscall_talk(message1) : syscall_talk(message2); 	
			

		last_x = x;
		last_y = y;
    }


	syscall_where(&x,&y);

}

void handler() {
	uint32_t* res = (uint32_t*) RES;

	if (*res == 0) {
		syscall_informAction(Center);
	} else {
		syscall_informAction(Down);
}
}