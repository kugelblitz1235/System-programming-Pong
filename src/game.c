/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "sched.h"
#include "screen.h"

int8_t directions[2][3] = {
	{
		-1, 0, 1
	},
	{
		1, 0, -1
	}
};

Player players[PLAYERS];
Player last_players[PLAYERS];


InfoPanel players_info[PLAYERS];

void new_ball(uint8_t player,uint8_t ball_type){

	uint8_t available_ball = 0;

	while(	available_ball < MAX_BALLS &&
			players[player].balls[available_ball].alive != FALSE){

		available_ball++;
	}

	if(available_ball != MAX_BALLS && players[player].balls_left > 0){
		uint16_t task_selector = tss_init_task(player,ball_type,MAX_BALLS*player+available_ball);//tss selector en la gdt
		
		set_task_selector(MAX_BALLS*player+available_ball,task_selector);

		players[player].balls_type[available_ball] = ball_type;
		players[player].balls[available_ball].alive = TRUE;
		players[player].balls[available_ball].direction = (player == 0) ? DIR_LEFT : DIR_RIGHT;
		players[player].balls[available_ball].inverted_action = FALSE;

		Point position;
		position.x = players[player].paddle.x + ((player == PLAYER_A) ? 1 : -1);
		position.y = players[player].paddle.y+PADDLE_HEIGHT/2;
		players[player].balls[available_ball].position = position;

		players[player].balls_actions[available_ball] = Center;
		players[player].balls_left--;

		players[player].balls_msg[available_ball][0] = 0;
	}
	last_players[player].balls[available_ball].position = players[player].balls[available_ball].position;
	last_players[player].balls[available_ball].alive = players[player].balls[available_ball].alive;
	
}

void move_paddle(uint8_t player,paddle_action_t action){

	if(player != PLAYER_A && player != PLAYER_B)
		return;

	players[player].paddle_action = action;


	update_game();
}

void game_init(){
	game_state_init();
	game_board_init();
	game_panel_init();
	draw_screen();
}

void game_panel_init(){

	//Init panel structures for both players
	players_info[PLAYER_A].location = (Point){1,MAP_HEIGHT+1};
	players_info[PLAYER_A].bg_color = C_BG_LIGHT_RED;
	players_info[PLAYER_A].task_slot_color = C_BG_WHITE | C_FG_RED;

	players_info[PLAYER_B].location = (Point){MAP_WIDTH/2+1,MAP_HEIGHT+1};
	players_info[PLAYER_B].bg_color = C_BG_LIGHT_BLUE;
	players_info[PLAYER_B].task_slot_color = C_BG_WHITE | C_FG_BLUE;

	for(uint8_t p = 0;p < PLAYERS;p++){
		players_info[p].width = MAP_WIDTH/2 - 2;
		players_info[p].height = VIDEO_FILS-MAP_HEIGHT-2;
		players_info[p].score = (Point){1,1};
		players_info[p].balls_left = (Point){1,2};
		players_info[p].active_task_type[0] = (Point){1,4};
		players_info[p].active_task_type[1] = (Point){1,5};
		players_info[p].active_task_type[2] = (Point){1,6};
		players_info[p].task_slot[0] = (Point){21,1};
		players_info[p].task_slot[1] = (Point){23,1};
		players_info[p].task_slot[2] = (Point){25,1};
		players_info[p].active_task_msg[0] = (Point){14,4};
		players_info[p].active_task_msg[1] = (Point){14,5};
		players_info[p].active_task_msg[2] = (Point){14,6};
	}
	

	//Set black background for both players
	for(uint8_t y = MAP_HEIGHT;y < VIDEO_FILS;y++){
		for(uint8_t x = 0;x < MAP_WIDTH;x++){
			game_board[y][x] = (ca){' ',C_BG_BLACK};
		}
	}

	//Set background for each player's panel
	for(uint8_t p = 0;p < PLAYERS;p++){
		Point loc = players_info[p].location;
		for(uint8_t y = 0;y < players_info[p].height;y++){
			for(uint8_t x = 0;x < players_info[p].width;x++){
				game_board[loc.y+y][loc.x+x] = (ca){' ',players_info[p].bg_color};
			}
		}
	}
}

void game_state_init() {

	//Init the initial location of players paddles
	players[PLAYER_A].paddle = (Point){ 0,PADDLE_INITIAL_Y };
	players[PLAYER_B].paddle = (Point){ MAP_WIDTH - 1,PADDLE_INITIAL_Y };
	last_players[PLAYER_A].paddle = players[PLAYER_A].paddle;
	last_players[PLAYER_B].paddle = players[PLAYER_B].paddle;

	for (uint8_t p = 0; p < PLAYERS; p++) {
		//Initially player has the initial amount of balls for use
		players[p].balls_left = INITIAL_BALLS;
		players[p].score = 0;
		players[p].paddle_action = PADDLE_STILL;
		//Initially all balls are deactivated
		for (uint8_t t = 0; t < MAX_BALLS; t++) {
			players[p].balls[t].alive = false;
			last_players[p].balls[t].alive = false;
		}
	}


	for (uint8_t y = 0; y < MAP_HEIGHT; y++) {
		for (uint8_t x = 0; x < MAP_WIDTH; x++) {
			game_board[y][x].c = ' ';
			game_board[y][x].c = ' ';
		}
	}

}

void game_board_init(){

    //white
	for(uint8_t y = 0;y < MAP_HEIGHT;y++){
		for(uint8_t x = 0;x < MAP_WIDTH;x++){
			game_board[y][x] = (ca){' ',C_BG_WHITE};
		}
	}
	//gray
	for(uint8_t y = 0;y < MAP_HEIGHT;y++){
		for(uint8_t x = 1;x < MAP_WIDTH-1;x++){
			game_board[y][x] = (ca){' ',C_BG_LIGHT_GREY};
		}
	}

	//player red and blue paddles
	for (uint8_t p = 0; p < PLAYERS; p++) {
		Paddle* paddle = &players[p].paddle;
		for (uint8_t i = 0; i < PADDLE_HEIGHT; i++) {
			game_board[paddle->y+i][paddle->x] = (ca){PADDLE_SPRITE,(p == PLAYER_A) ? PLAYER_A_PADDLE_COLOR : PLAYER_B_PADDLE_COLOR};
		}
	}

}

void update_game(){
	if (scheduler.ticks % 2 == 0) {
		update_paddles();
	}
	update_balls();
	update_game_board();
	update_game_panel();
}

void update_paddles(){
	for(uint8_t p = 0;p < PLAYERS;p++){
		last_players[p].paddle = players[p].paddle;

		if(players[p].paddle_action == MOVE_PADDLE_UP){
			if(players[p].paddle.y > 0)
				players[p].paddle.y--;
			players[p].paddle_action = MOVE_PADDLE_DOWN;
		}else if(players[p].paddle_action == MOVE_PADDLE_DOWN){
			if((players[p].paddle.y+PADDLE_HEIGHT) < MAP_HEIGHT)
				players[p].paddle.y++;
			players[p].paddle_action = MOVE_PADDLE_UP;
		}

		players[p].paddle_action = PADDLE_STILL;

	}
}

void update_balls() {
	for (uint8_t p = 0; p < PLAYERS; p++) {
		for (uint8_t t = 0; t < MAX_BALLS; t++) {
			last_players[p].balls[t] = players[p].balls[t];
			//Check if actual ball is in game
			if (players[p].balls[t].alive) {

				//action is either Up=1, Center=2, Down=3
				e_action_t action = players[p].balls_actions[t];

				Ball* ball = &players[p].balls[t];

				uint8_t inverted_action = ball->inverted_action;

				//Move
				if (ball->direction == DIR_LEFT)
					ball->position.x--;
				else
					ball->position.x++;

				//Move ball vertically using the action and the inverted action flag
				players[p].balls[t].position.y += directions[inverted_action][action - 1];


				//XChecking y collision first is needed so the x collision check works correctly after

				//Ball hit horizontal top wall
				if (ball->position.y == -1) {
					ball->position.y = 1;
					ball->inverted_action = !ball->inverted_action;
				}//Ball hit horizontal bottom wall
				else if (ball->position.y == MAP_HEIGHT) {
					ball->position.y = MAP_HEIGHT - 2;
					ball->inverted_action = !ball->inverted_action;
				}

				//Ball is in the player 0 goal zone
				if (ball->position.x == 0) {
					//Ball was rejected by player 0 paddle
					if (players[PLAYER_A].paddle.y <= ball->position.y && ball->position.y < (players[PLAYER_A].paddle.y + PADDLE_HEIGHT)){
						ball->position.x = 2;
						ball->direction = (ball->direction == DIR_LEFT) ? DIR_RIGHT : DIR_LEFT;
					}//Ball got into goal zone succesfully, player 1 scores a point
					else {
						players[PLAYER_B].score++;
						ball->alive = false;
					}
				}//Ball is in the player 1 goal zone
				else if (ball->position.x == MAP_WIDTH - 1) {
					//Ball was rejected by player 1 paddle
					if (players[PLAYER_B].paddle.y <= ball->position.y && ball->position.y < (players[PLAYER_B].paddle.y + PADDLE_HEIGHT)) {
						ball->position.x = (MAP_WIDTH-1)-2;
						ball->direction = (ball->direction == DIR_LEFT) ? DIR_RIGHT : DIR_LEFT;
					}//Ball got into goal zone succesfully, player 0 scores a point
					else {
						players[PLAYER_A].score++;
						ball->alive = false;
					}
				}

				players[p].balls_actions[t] = Center;

				//Ball died now
				if(ball->alive == false){
					kill_ball(p,t);
				}
			}
		}
	}
}

void update_game_panel(){
	for(uint8_t p = 0;p < PLAYERS;p++){
		Point loc = players_info[p].location;
		for(uint8_t y = 0;y < players_info[p].height;y++){
			for(uint8_t x = 0;x < players_info[p].width;x++){
				game_board[loc.y+y][loc.x+x] = (ca){' ',players_info[p].bg_color};
			}
		}

		//Draw "Puntos: XXXXX" to the screen
		char* score_text = "Puntos: ";
		screen_print(
			score_text,
			loc.x+players_info[p].score.x,
			loc.y+players_info[p].score.y,
			C_BG_WHITE | C_FG_BLACK
		);
		screen_print_dec(
			players[p].score,
			5,
			loc.x + players_info[p].score.x + 8,
			loc.y + players_info[p].score.y,
			C_BG_WHITE | C_FG_BLACK
		);

		//Draw "Pelotas: XX" to the screen
		char* balls_left_text = "Pelotas: ";
		screen_print(
			balls_left_text,
			loc.x+players_info[p].balls_left.x,
			loc.y+players_info[p].balls_left.y,
			C_BG_WHITE | C_FG_BLACK
		);
		screen_print_dec(
			players[p].balls_left,
			2,
			loc.x + players_info[p].balls_left.x + 9,
			loc.y + players_info[p].balls_left.y,
			C_BG_WHITE | C_FG_BLACK
		);

		for(uint8_t b = 0;b < MAX_BALLS;b++){
			if(players[p].balls[b].alive == true){
				//Draw "<slot number>: Tarea <task owner><task type>" to the screen
				char* task_text = "B:Tarea PT";
				task_text[0] = b+'0';
				task_text[8] = (p == 0) ? 'A' : 'B';
				task_text[9] = players[p].balls_type[b] + '0';
				screen_print(
					task_text,
					loc.x+players_info[p].active_task_type[b].x,
					loc.y+players_info[p].active_task_type[b].y,
					C_BG_WHITE | C_FG_BLACK
				);

				//Print task message
				screen_print(
					players[p].balls_msg[b],
					loc.x+players_info[p].active_task_msg[b].x,
					loc.y+players_info[p].active_task_msg[b].y,
					(C_BG_WHITE | C_FG_BLACK)
				);
			}

			//Draw "O" or "X" showing slot is 
			//occupied or not by an active task
			char* task_slot_text = players[p].balls[b].alive ? "O" : "X";
			uint8_t color = 
				players[p].balls[b].alive ? 
					players_info[p].task_slot_color : 
					(C_BG_WHITE | C_FG_BLACK);

			screen_print(
				task_slot_text,
				loc.x+players_info[p].task_slot[b].x,
				loc.y+players_info[p].task_slot[b].y,
				color
			);

		}
	}
}

void update_game_board() {
	uint8_t game_running = (players[PLAYER_A].balls_left > 0 || players[PLAYER_B].balls_left > 0);

	for (uint8_t p = 0; p < PLAYERS; p++) {
		for (uint8_t t = 0; t < MAX_BALLS; t++) {
			game_running |= (players[p].balls[t].alive);
		}
	}

	if(game_running){

		//last player red and blue balls
		for (uint8_t p = 0; p < PLAYERS; p++) {
			for (uint8_t t = 0; t < MAX_BALLS; t++) {
				Ball* ball = &last_players[p].balls[t];
				if(ball->alive){
					game_board[ball->position.y][ball->position.x] = (ca){EMPTY_CELL_SPRITE,C_BG_LIGHT_GREY};
				}
			}
		}

		//last player red and blue paddles
		for (uint8_t p = 0; p < PLAYERS; p++) {
			Paddle* paddle = &last_players[p].paddle;
			for (uint8_t i = 0; i < PADDLE_HEIGHT; i++) {
				game_board[paddle->y+i][paddle->x] = (ca){EMPTY_CELL_SPRITE,C_BG_WHITE};
			}
		}

		//player red and blue balls
		for (uint8_t p = 0; p < PLAYERS; p++) {
			ca ball_draw;

			if(p == PLAYER_A){
				ball_draw = (ca){BALL_SPRITE,PLAYER_A_BALL_COLOR};
			}else{
				ball_draw = (ca){BALL_SPRITE,PLAYER_B_BALL_COLOR};
			}

			for (uint8_t t = 0; t < MAX_BALLS; t++) {
				Ball* ball = &players[p].balls[t];
				if(ball->alive){
					game_board[ball->position.y][ball->position.x] = ball_draw;
				}
			}
		}

		//player red and blue paddles
		
		for (uint8_t p = 0; p < PLAYERS; p++) {
			Paddle* paddle = &players[p].paddle;
			for (uint8_t i = 0; i < PADDLE_HEIGHT; i++) {
				game_board[paddle->y+i][paddle->x] = (ca){PADDLE_SPRITE,(p == PLAYER_A) ? PLAYER_A_PADDLE_COLOR : PLAYER_B_PADDLE_COLOR};
			}
		}
		
	}else{
		char* winning_text_A[8] = { 
			"  _____   _                             __  __          __ _            ",
			" |  __ \\ | |                           /_ | \\ \\        / /(_)            ",
			" | |__) || |  __ _  _   _   ___  _ __   | |  \\ \\  /\\  / /  _  _ __   ___ ",
			" |  ___/ | | / _` || | | | / _ \\| '__|  | |   \\ \\/  \\/ /  | || '_ \\ / __|",
			" | |     | || (_| || |_| ||  __/| |     | |    \\  /\\  /   | || | | |\\__ \\",
			" |_|     |_| \\__,_| \\__, | \\___||_|     |_|     \\/  \\/    |_||_| |_||___/",
			"                     __/ |                                               ",
			"                    |___/                                                "
		};
		char* winning_text_B[8] = {
			" _____   _                             ___   __          __ _             ",
			"|  __ \\ | |                           |__ \\  \\ \\        / /(_)            ",
			"| |__) || |  __ _  _   _   ___  _ __     ) |  \\ \\  /\\  / /  _  _ __   ___ ",
			"|  ___/ | | / _` || | | | / _ \\| '__|   / /    \\ \\/  \\/ /  | || '_ \\ / __|",
			"| |     | || (_| || |_| ||  __/| |     / /_     \\  /\\  /   | || | | |\\__ \\",
			"|_|     |_| \\__,_| \\__, | \\___||_|    |____|     \\/  \\/    |_||_| |_||___/",
			"                    __/ |                                                 ",
			"                   |___/                                                 "
		};
		char* draw_text[6] = {
			"______                        ",
			"|  __ \\                       ",
			"| |  | | _ __  __ _ __      __",
			"| |  | || '__|/ _` |\\ \\ /\\ / /",
			"| |__| || |  | (_| | \\ V  V / ",
			"|_____/ |_|   \\__,_|  \\_/\\_/  "
		};
        for(uint8_t y = MAP_HEIGHT/2-6;y < MAP_HEIGHT/2+6;y++){
			for(uint8_t x = 2;x < MAP_WIDTH-2;x++){
				game_board[y][x] = (ca){' ',C_FG_BLACK};
			}
		}

		for(uint8_t y = MAP_HEIGHT/2-5;y < MAP_HEIGHT/2+5;y++){
			for(uint8_t x = 3;x < MAP_WIDTH-3;x++){
				game_board[y][x] = (ca){' ',C_BG_RED};
			}
		}

        if(players[PLAYER_A].score > players[PLAYER_B].score){
			for(uint8_t y = 0;y < 8;y++){
				screen_print(winning_text_A[y],3,y+MAP_HEIGHT/2-4,C_FG_BLACK|C_BG_RED);
			}
        }else if(players[PLAYER_A].score < players[PLAYER_B].score){
			for(uint8_t y = 0;y < 8;y++){
				screen_print(winning_text_B[y],3,y+MAP_HEIGHT/2-4,C_FG_BLACK|C_BG_RED);
			}
        }else{
			for(uint8_t y = 0;y < 6;y++){
				screen_print(draw_text[y],25,y+MAP_HEIGHT/2-4,C_FG_BLACK|C_BG_RED);
			}
        }
	}
}

// Graphics routines
//*****************************************************************************//

void screen_print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        game_board[y][x].c = (uint8_t) text[i];
        game_board[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void screen_print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        game_board[y][x + size - i - 1].c = letras[resto];
        game_board[y][x + size - i - 1].a = attr;
    }
}

void screen_print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
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
        game_board[y][x + size - i - 1].c = hexa[i];
        game_board[y][x + size - i - 1].a = attr;
    }
}


void key_pressed_handler(uint8_t key_code){
  if(scheduler.debug_mode == TRUE && scheduler.on_interrupt == TRUE && key_code != key_y)
  	return;

  switch(key_code){
  
  case key_w:
  		move_paddle(PLAYER_A,MOVE_PADDLE_UP);
  	break;
  
  case key_s:
  		move_paddle(PLAYER_A,MOVE_PADDLE_DOWN);
  	break;
  
  case key_i:
  		move_paddle(PLAYER_B,MOVE_PADDLE_UP);
  	break;
  
  case key_k:
  		move_paddle(PLAYER_B,MOVE_PADDLE_DOWN);
  	break;
  
  case key_z:
  		new_ball(PLAYER_A,TYPE_1);
  	break;	
  
  case key_x:
  		new_ball(PLAYER_A,TYPE_2);
  	break;
  	
  case key_c:
  		new_ball(PLAYER_A,TYPE_3);
  	break;
  	
  case key_b:
  		new_ball(PLAYER_B,TYPE_1);
  	break;
  
  case key_n:
  		new_ball(PLAYER_B,TYPE_2);
  	break;
  
  case key_m:
  		new_ball(PLAYER_B,TYPE_3);
  	break;
  
  case key_y:
  		if(scheduler.debug_mode == TRUE){
  			//si esta habilitado el modo debug
  			
  			if(scheduler.on_interrupt == TRUE){
  				//si fue interrupido el juego por una excepcion
  				scheduler.on_interrupt=FALSE;	
  			}else{
  				//sino hubo una interrupcion desabilitamos el modo debug
  				scheduler.debug_mode = FALSE;
  			}
  				
  		} else {
			//si no estaba el modo debug habilitado, lo habilitamos
			scheduler.debug_mode = TRUE;
		}
  	break;
  		
  }
}
//setea el flag del scheduler interrup que vamos a 
//utilizar en el modo debug
void set_interrupt(){
	if(scheduler.debug_mode == TRUE)
		scheduler.on_interrupt=TRUE;
}

char* interruptions[20]={
						"Divide Error Exception #0",
						"Debug Exception #1",
						"Non Maskable interrupt #2",
						"Breakpoint Exception #3",
						"Overflow Exception #4",
						"BOUND Range Exceeded Exception #5",
						"Invalid Opcode Exception #6",
						"Device Not Available Exception #7",
						"Doble Fault Exception #8",
						"Coprocessor Segment Overrun #9",
						"TSS invalido #10",
						"Segmento No Presente #11",
						"Stack Fault #12",
						"General Protection Fault #13",
						"Page Fault #14",
						"No idea",
						"x87 FPU Floating Point Error #16",
						"Alignment Check Exception #17",
						"Machine Check Exception #18",
						"SIMD Floating Point Exception #19"
					};

void set_exception_type(uint32_t* eXXregs,
						 uint8_t exception_type,					 
						 uint16_t* segments,
						 uint32_t eflags ){

	//aca deberiamos obtener los crX con las funciones dadas por la catedra
	uint32_t crX[4] = {rcr0(), rcr2(),rcr3(),rcr4()};

	//dibujamos el modo debug

	if(scheduler.debug_mode == TRUE && scheduler.on_interrupt == TRUE){
		//si esta activado el modo debug y ocurrio una interrupcion
		draw_debug_panel(eXXregs, segments,crX,eflags, interruptions[exception_type]);
	}

}

void kill_ball(uint8_t player,uint8_t ball){
	players[player].balls[ball].alive = false;

	sched_kill_task(MAX_BALLS*player+ball);
}

void set_task_handler(f_handler_t f_handler){
	
	//This task has already called setHandler
	//Has to die
	if(scheduler.handler_ptr[scheduler.current_task] != 0){
		kill_actual_task();
	}else
	{
		set_handler_ptr(f_handler);
	}
	
}


void set_ball_action(e_action_t action){
	players[scheduler.current_task/MAX_BALLS].balls_actions[scheduler.current_task%MAX_BALLS] = action;

}


void set_ball_msg(char* message){

	uint8_t i = 0;
	while(i < MAX_MSG_LENGTH && message[i] != 0){
		players[scheduler.current_task/MAX_BALLS].balls_msg[scheduler.current_task%MAX_BALLS][i] = message[i];
		i++;
	}
	players[scheduler.current_task/MAX_BALLS].balls_msg[scheduler.current_task%MAX_BALLS][i] = 0;
}

void get_ball_position(uint32_t* esp0){
    //x = ebx = esp[0]
    //y = ecx = esp[1]

	uint8_t player = scheduler.current_task/MAX_BALLS;
	uint8_t ball = scheduler.current_task%MAX_BALLS;

	Point position = players[player].balls[ball].position;
	int delta_x = position.x-players[player].paddle.x;
	
	if(delta_x < 0)
		delta_x *= -1;

	esp0[0] = delta_x;
	esp0[1] = position.y+1;
}

void kill_actual_task(){
	Ball* ball = &players[scheduler.current_task/MAX_BALLS].balls[scheduler.current_task%MAX_BALLS];
	// La seteamos muerta para la logica  del juego
	ball->alive = false;
	// Matamos la tarea
	sched_kill_task(scheduler.current_task);
	// La borramos del tablero
	game_board[ball->position.y][ball->position.x] = (ca){EMPTY_CELL_SPRITE,C_BG_LIGHT_GREY};
}

void check_bad_handler(){
	if(scheduler.is_current_handler == TRUE){
		kill_actual_task();
	}
}