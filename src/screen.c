/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
    for (c = cInit; c < cInit+cSize; c++) {
          p[f][c].c = character;
          p[f][c].a = attr;
    }}
}

void draw_screen(){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    for(uint32_t y = 0;y < VIDEO_FILS;y++){
        for(uint32_t x = 0;x < VIDEO_COLS;x++){
            if(!(p[y][x].c == game_board[y][x].c && p[y][x].a == game_board[y][x].a)){
                p[y][x] = game_board[y][x];
            }
        }
    }
}

//******************************* DEBUG STUFF ***************************
/*
typedef struct{
    Location location;
    Location eXXregs[9];
    Location segments[6];
    Location crX[4];
    Location eflags;
    Location stack_content;
    char* eXXregs_text[9];
    char* segments_text[6];
    char* crX_text[4];
}DebugPanel;
*/
DebugPanel debug_panel;

void init_debug_panel_config(){

    debug_panel.location = (Location){27,1};
    debug_panel.height = 40;
    debug_panel.width = 30;

    for(uint8_t i = 0;i < 9;i++)
        debug_panel.eXXregs[i] = (Location){2,5+i*2};
    for(uint8_t i = 0;i < 6;i++)
        debug_panel.segments[i] = (Location){3,debug_panel.eXXregs[8].y+2+i*2};

    for(uint8_t i = 0;i < 4;i++)
        debug_panel.crX[i] = (Location){16,6+i*2};

    debug_panel.eflags = (Location){3,debug_panel.segments[5].y+3};

    debug_panel.stack_content = (Location){debug_panel.crX[0].x,debug_panel.segments[1].y+1};


    char* eXXregs_text[9] = {"eax","ebx","ecx","edx","esi","edi","ebp","esp","eip"};
    char* segments_text[6] = {"cs","ds","es","fs","gs","ss"};
    char* crX_text[4] = {"cr0","cr2","cr3","cr4"};
    
    debug_panel.eflags_text = "eflags";
    
    for(uint8_t i = 0;i < 9;i++)
        debug_panel.eXXregs_text[i] = eXXregs_text[i];
    for(uint8_t i = 0;i < 6;i++)
        debug_panel.segments_text[i] = segments_text[i];
    for(uint8_t i = 0;i < 4;i++)
        debug_panel.crX_text[i] = crX_text[i];
}

void draw_debug_panel(uint32_t* eXXregs,//{"eax","ebx","ecx","edx","esi","edi","ebp","esp","eip"}
                       uint16_t* segments,//{"cs","ds","es","fs","gs","ss"}
                       uint32_t* crX,//{"cr0","cr2","cr3","cr4"}
                       uint32_t eflags,
                       char* exception){

    Location loc = debug_panel.location;

    screen_drawBox( loc.y,
                    loc.x,
                    debug_panel.height,
                    debug_panel.width,
                    ' ',
                    C_BG_BLACK );

    screen_drawBox( loc.y+1,
                    loc.x+1,
                    1,
                    debug_panel.width-2,
                    ' ',
                    C_BG_RED );

    screen_drawBox( loc.y+3,
                    loc.x+1,
                    debug_panel.height-4,
                    debug_panel.width-2,
                    ' ',
                    C_BG_LIGHT_GREY );

    print(exception,
        loc.x+1,
        loc.y+1,
        C_BG_RED | C_FG_WHITE);
    

    //imprimo los eflags 
    print(debug_panel.eflags_text,
            loc.x+debug_panel.eflags.x,
            loc.y+debug_panel.eflags.y,
            C_BG_LIGHT_GREY | C_FG_BLACK
        );

    //imprimo los eflags 
    print_hex(eflags,
            8,
            loc.x+debug_panel.eflags.x+7,
            loc.y+debug_panel.eflags.y,
            C_BG_LIGHT_GREY | C_FG_WHITE
        );


    //imprime los registros de proposito general
    for(uint8_t i = 0;i < 9;i++){
        print(  debug_panel.eXXregs_text[i],
                loc.x+debug_panel.eXXregs[i].x,
                loc.y+debug_panel.eXXregs[i].y,
                C_BG_LIGHT_GREY | C_FG_BLACK );

        print_hex(  eXXregs[i],
                    8,
                    loc.x+debug_panel.eXXregs[i].x+4,
                    loc.y+debug_panel.eXXregs[i].y,
                    C_BG_LIGHT_GREY | C_FG_WHITE );
    }

    //imprime los registros de segmento
    for(uint8_t i = 0;i < 6;i++){
        print(  debug_panel.segments_text[i],
                loc.x+debug_panel.segments[i].x,
                loc.y+debug_panel.segments[i].y,
                C_BG_LIGHT_GREY | C_FG_BLACK );

        print_hex(  segments[i],
                    4,
                    loc.x+debug_panel.segments[i].x+3,
                    loc.y+debug_panel.segments[i].y,
                    C_BG_LIGHT_GREY | C_FG_WHITE );
    }

    //imprime los registros de control
    for(uint8_t i = 0;i < 4;i++){
        print(  debug_panel.crX_text[i],
                loc.x+debug_panel.crX[i].x,
                loc.y+debug_panel.crX[i].y,
                C_BG_LIGHT_GREY | C_FG_BLACK );

        print_hex(  crX[i],
                    8,
                    loc.x+debug_panel.crX[i].x+4,
                    loc.y+debug_panel.crX[i].y,
                    C_BG_LIGHT_GREY | C_FG_WHITE );
    }
    
    //imprime lo que hay en el stack
    print(  "stack",
            loc.x+debug_panel.stack_content.x,
            loc.y+debug_panel.stack_content.y,
            C_BG_LIGHT_GREY | C_FG_BLACK );

    uint32_t* stack_pointer = (uint32_t*)eXXregs[7];

    for(uint8_t i = 0;i < 5;i++){

        print_hex(  stack_pointer[i],
                    8,
                    loc.x+debug_panel.stack_content.x,
                    loc.y+debug_panel.stack_content.y+ 2+i,
                    C_BG_LIGHT_GREY | C_FG_WHITE );
    }
}
