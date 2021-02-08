/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#define Codigo0 14
#define Codigo3 15
#define Dato0 16
#define Dato3 17

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [KERNEL_CODE_SEGMENT] =(gdt_entry) {
        // Codigo kernel
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1010,           /* type         */
        (uint8_t)     0x01,           /* s = exec     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [USER_CODE_SEGMENT]=(gdt_entry) {
        // Codigo user
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1010,           /* type         */
        (uint8_t)     0x01,           /* s = exec     */
        (uint8_t)     0x03,           /* dpl = 3      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [KERNEL_DATA_SEGMENT]=(gdt_entry) {
        // Datos Kernel
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b0010,           /* type         */
        (uint8_t)     0x01,           /* s = data     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [USER_DATA_SEGMENT]=(gdt_entry) {
        // Datos User
        (uint16_t)    0xA2FF,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b0010,         /* type         */
        (uint8_t)     0x01,           /* s = data     */
        (uint8_t)     0x03,           /* dpl = 3      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [SCREEN_SEGMENT]=(gdt_entry) {
        //segmento de pantalla
        (uint16_t)    0x1F3F,         /* limit[0:15]  */
        (uint16_t)    0x8000,         /* base[0:15]   */
        (uint8_t)     0x0B,           /* base[23:16]  */
        (uint8_t)     0b0010,           /* type         */
        (uint8_t)     0x01,           /* s = data     */
        (uint8_t)     0x03,           /* dpl = 3      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    //entradas tss 19-32 
    [INITIAL_TASK]=(gdt_entry) {
        //Tarea Inicial
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [IDLE_TASK]=(gdt_entry) {
        //TAREA IDLE
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0,         /* base[0:15]   */
        (uint8_t)     0x0,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [TASK_A_1]=(gdt_entry) {
        //TAREA 1A
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [TASK_A_2]=(gdt_entry) {
        //TAREA 2A
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [TASK_A_3]=(gdt_entry) {
        //TAREA 3A
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [TASK_B_1]=(gdt_entry) {
        //TAREA 1B
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [TASK_B_2]=(gdt_entry) {
        //TAREA 2B
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [TASK_B_3]=(gdt_entry) {
        //TAREA 3B
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [HANDLERS_A_1]=(gdt_entry) {
        //HANDLER 1A
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [HANDLERS_A_2]=(gdt_entry) {
        //HANDLER 2A
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [HANDLERS_A_3]=(gdt_entry) {
        //HANDLER 3A
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [HANDLERS_B_1]=(gdt_entry) {
        //HANDLER 1B
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0         */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    [HANDLERS_B_2]=(gdt_entry) {
        //HANDLER 2B
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
        [HANDLERS_B_3]=(gdt_entry) {
        //HANDLER 3B
        (uint16_t)    0x67,          /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0b1001,           /* type         */
        (uint8_t)     0x00,           /* s = system     */
        (uint8_t)     0x00,           /* dpl = 0      */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x01,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g = 0            */
        (uint8_t)     0x00,           /* base[31:24]  */
    }  
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
