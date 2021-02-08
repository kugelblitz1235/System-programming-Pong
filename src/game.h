/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

//Board constraints
#define MAP_WIDTH               80
#define MAP_HEIGHT 				40

//Amount of players in game
#define PLAYERS      			2
#define MAX_BALLS				3
#define INITIAL_BALLS			20

//Player's paddle constraints
#define PADDLE_HEIGHT			7
#define PADDLE_WIDTH			1
#define PADDLE_INITIAL_Y		MAP_HEIGHT/2 - PADDLE_HEIGHT/2

#define PLAYER_A				0
#define PLAYER_B				1	
#define PLAYER_A_BALL_COLOR 	C_FG_LIGHT_RED | C_BG_LIGHT_GREY
#define PLAYER_B_BALL_COLOR		C_FG_LIGHT_BLUE | C_BG_LIGHT_GREY
#define PLAYER_A_PADDLE_COLOR 	C_BG_LIGHT_RED
#define PLAYER_B_PADDLE_COLOR	C_BG_LIGHT_BLUE

#define TYPE_1   				0
#define TYPE_2   				1
#define TYPE_3   				2

#define MAX_MSG_LENGTH			20

#define BALL_SPRITE		  		'o'
#define PADDLE_SPRITE	  		' '
#define EMPTY_CELL_SPRITE		' '

#define false					0
#define true					1


#define key_w 0x11
#define key_s 0x1F
#define key_z 0x2C
#define key_x 0x2D
#define key_c 0x2E
#define key_i 0x17
#define key_k 0x25
#define key_b 0x30
#define key_n 0x31
#define key_m 0x32
#define key_y 0x15


typedef struct {
	int8_t x;
	int8_t y;
} Point;

typedef enum {
	DIR_LEFT = 0,
	DIR_RIGHT
} Direction;

typedef struct {
	uint8_t alive;
	Point position;
	Direction direction;
	uint8_t inverted_action;//Flag to know if ball has the actions inverted
}Ball;

typedef Point Paddle;

typedef enum{
	Up = 1,
	Center = 2,
	Down = 3
} e_action_t;

typedef enum{
	MOVE_PADDLE_UP = 0,
	MOVE_PADDLE_DOWN = 1,
	PADDLE_STILL = 2
} paddle_action_t;

typedef struct{
	Ball balls[MAX_BALLS];
	uint8_t balls_type[MAX_BALLS];
	char balls_msg[MAX_BALLS][MAX_MSG_LENGTH];
	e_action_t balls_actions[MAX_BALLS];
	Paddle paddle;
	uint8_t balls_left;
	uint8_t score;
	paddle_action_t paddle_action;
	uint8_t new_ball_type[MAX_BALLS];
}Player;

typedef struct{
	Point location;
	uint8_t width;
	uint8_t height;
	uint8_t bg_color;
	uint8_t task_slot_color;
	Point score;
	Point balls_left;
	Point active_task_type[MAX_BALLS];
	Point active_task_msg[MAX_BALLS];
	Point task_slot[MAX_BALLS];
}InfoPanel;

void game_init();
void game_state_init();
void game_board_init();
void game_panel_init();
void update_game();
void update_balls();
void update_paddles();
void update_game_board();
void update_game_panel();

void screen_print(const char* text, uint32_t x, uint32_t y, uint16_t attr);
void screen_print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr);
void screen_print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr);

void key_pressed_handler(uint8_t key_code);
void set_interrupt();
void set_exception_type(uint32_t* eXXregs,
						uint8_t exception_type,
						 uint16_t* segments,
						 uint32_t eflags);

void kill_ball(uint8_t player,uint8_t ball);

void kill_actual_task();

void check_bad_handler();

void set_task_handler(f_handler_t f_handler);
void set_ball_action(e_action_t action);
void set_ball_msg(char* message);
void get_ball_position(uint32_t* esp0);
#endif  /* !__GAME_H__ */
