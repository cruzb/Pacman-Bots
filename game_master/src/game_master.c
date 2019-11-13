#include "game_master.h"
#include "N64/N64.h"
#include "robot/robot.h"
#include "string.h"
#include "led/color_util.h"
#include "sleep/sleep.h"
#include "path/path.h"
#include "rgb_util/rgb_util.h"
#include "CMSIS/mss_assert.h"

typedef enum state {
	calibrate,
	reset,
	start,
	advance,
	won,
	lost,
	idle,
	kill
} state_t;

typedef struct player_state
{
	char player_type;
	cell_t pos;
	direction_t dir;
	color_t color;
	color_t color_swap;
	char prev_occupancy;
	path_t path;
} player_state_t;

#define NUM_PLAYERS 1

volatile uint32_t *LED_ADDR;
volatile uint32_t *N64_ADDR;

struct N64Inst_t n64;
struct Robot players[NUM_PLAYERS];

led_inst_t led_inst;
grid_t grid_inst;
grid_t grid_inst_init;

uint32_t piece_update_period_usecs = 1000000;
uint32_t led_update_period_usecs = 1000000;

player_state_t pacman;
player_state_t ghost;
color_t char_colors[255];
state_t prev_state;
state_t state;

int num_rounds_ghosts_still;

char* pacman_occupancy[] =
{
	"111111DDDDDDDDDDDDD",
	"11111DDDDDDDDDDDDD1",
	"1111DDDDCCDDDDDD111",
	"111DDDDDCCDDDD11111",
	"11DDDDDDDDDD1111111",
	"DDDDDDDDDD111111111",
	"DDDDDDDDDD111111111",
	"1DDDDDDDDDDD1111111",
	"11DDDDDDDDDDDD11111",
	"111DDDDDDDDDDDDD111",
	"1111DDDDDDDDDDDDD11",
	"11111DDDDDDDDDDDDDD",
	NULL
};

void eat_cherry(cell_t dot);
void eat_powerup(cell_t dot);

__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{
	N64GetState(&n64);
	//N64PrintState(&inst, 1);
}

void set_char_color_pair(char_color_pair_t* char_color_pair) {
	char_colors[(uint8_t)(char_color_pair->character)] = char_color_pair->color;
}

void set_char_color_pairs(char_color_pair_t* char_color_pairs) {
	while(char_color_pairs->character) {
		set_char_color_pair(char_color_pairs++);

	}
}

color_t get_char_color(char character) {
	return char_colors[(uint8_t)character];
}

void set_colors_grid(led_inst_t* led_inst, grid_t* grid_inst, size_t color_buf_idx)
{
	size_t row, col;
	for(row = 0; row < grid_inst->num_rows; ++row) {
		for(col = 0; col < grid_inst->num_cols; ++col) {
			size_t strip_index = led_grid_idx_to_strip_idx(led_inst, row, col);
			uint8_t occ = grid_get_occupancy(grid_inst, grid_cell(row,col));
			color_t color = get_char_color(occ);
			led_set_color(led_inst, strip_index, color_buf_idx, color);
		}
	}
}

void player_init(player_state_t* player, char player_type) {
	player->pos = grid_find(&grid_inst, player_type);
	player->color = get_char_color(player_type);
	player->color_swap = get_char_color('D');
	player->path = path();
	player->dir = NONE;
	player->player_type = player_type;
	player->prev_occupancy = 'D';
}

void master_robot_rcv(const struct Robot *robot)
{
	robot_id_t rid = RobotGetId(robot);
	if (RobotGetId(players) == rid)
	{
		if (is_powerup(robot->data.seen_color) && grid_get_occupancy(&grid_inst, pacman.pos) == 'U')
			eat_powerup(pacman.pos);
		else if (is_cherry(robot->data.seen_color) && grid_get_occupancy(&grid_inst, pacman.pos) == 'C')
			eat_cherry(pacman.pos);
	}
	//RobotPrint(&robot);
}

void send_robot_cmd(robot_cmd_t cmd)
{
	struct Robot toSend = players[0];
	toSend.data.cmd = cmd;
	RobotBroadcast(&toSend);
}

void eat_dot(cell_t dot)
{
	grid_set_occupancy(&grid_inst, dot, '0');
	size_t ledIdx = led_grid_idx_to_strip_idx(&led_inst, dot.row, dot.col);
	led_set_color(&led_inst, ledIdx, 0, COLOR_BLACK);
}

void eat_cherry(cell_t dot) {
	grid_set_occupancy(&grid_inst, dot, '0');
	size_t ledIdx = led_grid_idx_to_strip_idx(&led_inst, dot.row, dot.col);
	led_set_color(&led_inst, ledIdx, 0, COLOR_BLACK);
	// UP TO 5 random dots are eaten
	int i;
	for (i = 0; i < 5; i++)
	{
		size_t row = rand() % grid_inst.num_rows;
		size_t col = rand() % grid_inst.num_cols;
		cell_t cell = (cell_t){ row, col };
		if (grid_get_occupancy(&grid_inst, cell) == 'D')
			eat_dot(cell);
	}
}

void eat_powerup(cell_t dot) {
	grid_set_occupancy(&grid_inst, dot, '0');
	size_t ledIdx = led_grid_idx_to_strip_idx(&led_inst, dot.row, dot.col);
	led_set_color(&led_inst, ledIdx, 0, COLOR_BLACK);
	// ghost stay still for 5 rounds
	num_rounds_ghosts_still = 5;
}

void move_piece(player_state_t* player, cell_t new_pos)
{
	if (grid_cell_match(player->pos, new_pos)) {
		return;
	}

	led_set_color_grid(&led_inst, player->pos, 0, player->color_swap);
	player->color_swap = get_char_color(grid_get_occupancy(&grid_inst, new_pos));
	led_set_color_grid(&led_inst, new_pos, 0, player->color);
	char new_occupancy = grid_get_occupancy(&grid_inst, new_pos);
	if(player->player_type == 'P') {
		// check for eating stuff
		char occ = player->prev_occupancy;
		grid_set_occupancy(&grid_inst, player->pos, occ);
		//char occ = grid_get_occupancy(&grid_inst, player->pos);
		switch(occ) {
			case 'D' : eat_dot(player->pos);
					   break;
			case 'P' : eat_dot(player->pos);
					   break;
			case 'U' : eat_powerup(player->pos);
					   break;
			case 'C' : eat_cherry(player->pos);
					   break;
			default:   break;
		}
		grid_set_occupancy(&grid_inst, new_pos, 'P');
	}
	else {
		grid_set_occupancy(&grid_inst, new_pos, grid_get_occupancy(&grid_inst, player->pos));
		grid_set_occupancy(&grid_inst, player->pos, player->prev_occupancy);
		//grid_swap_occupancy(&grid_inst, new_pos, player->pos);
	}
	player->prev_occupancy = new_occupancy;
	player->pos = new_pos;
}

void GameInit(char** occupancy, char_color_pair_t* char_color_pairs, volatile uint32_t *led_addr, volatile uint32_t *n64_addr)
{
	LED_ADDR = led_addr;
	N64_ADDR = n64_addr;
	grid_init(&grid_inst_init, occupancy);
	grid_init(&grid_inst, NULL);
	// initialize led_inst
	set_char_color_pairs(char_color_pairs);
	led_init(&led_inst, grid_inst_init.num_rows, grid_inst_init.num_cols, 2, led_addr);
	N64Init(&n64, N64_ADDR);
	ServerInit(&master_robot_rcv);
	send_robot_cmd(STOP);
	memset(players, 0, sizeof(robot));
	RobotSetId(players, PLAYER);
	num_rounds_ghosts_still = 0;
	state = idle;
	prev_state = kill;
	while (1)
	{
		int init = (state != prev_state);
		prev_state = state;
		switch(state) {
			case calibrate : GameCalibrate(init);
							 break;
			case reset 	 : 	 GameReset(init);
						     break;
			case start	 :   GameStart(init);
						     break;
			case advance :   GameAdvance(init);
						     break;
			case won	 :   GameWon(init);
						     break;
			case lost	 :   GameLost(init);
						     break;
			case idle	 :   GameIdle(init);
						     break;
			case kill	 :   GameKill(init);
						     return;
			default 	 :   ASSERT(0);
		}
	}
}

void GameCalibrate(int init) {
	if(init) {
		//Send cmd to robot
		send_robot_cmd(CALIBRATE);
	}

	led_set_colors(&led_inst, 0, led_inst.num_leds, 0, (color_t){64, 50, 0});
	led_send(&led_inst, 0, 1000000);
	if(N64ButtonPressed(&n64, BUTTON_START)) {
		state = reset;
	}
	else {
		state = calibrate;
	}
}

void GameReset(int init) {
	send_robot_cmd(STOP);
	grid_destroy(&grid_inst);
	grid_inst = grid_copy(&grid_inst_init);

	set_colors_grid(&led_inst, &grid_inst, 0);

	// init piece position led_inst
	player_init(&pacman, 'P');
	player_init(&ghost, 'G');

	//ServerInit(NULL);
	state = start;
}

void GameStart(int init)
{
	if (init)
		led_rand_show(&led_inst, 0, led_inst.num_leds, 1, 1000000, 2000000);
	else
	{
		if (N64DDir(&n64) != NONE)
		{
			state = advance;
			send_robot_cmd(START);
		}
		else
		{
			state = start;
		}
	}
}

void GameAdvance(int init)
{
	// get player direction from n64
	if(timer_done())
	{
		uint32_t delay = piece_update_period_usecs * 2;
		// nextDelay = piece_update_period_usecs / 2;
		// update player position based on controller input
		direction_t direction = N64DDir(&n64);
		if(direction != NONE) {
			//if (direction != pacman.dir)
				//nextDelay = piece_update_period_usecs * 2;
			pacman.dir = direction;
		}
		/*if (pacman.dir == LEFT || pacman.dir == RIGHT)
		{
			nextDelay = piece_update_period_usecs * 2;
			delay = nextDelay;
		}*/

		// Don't move if there's a wall or out of bounds
		cell_t pos_new = pacman.pos;
		switch (pacman.dir)
		{
			case UP: 	pos_new.col += 1;
						break;
			case DOWN: 	pos_new.col -= 1;
						break;
			case LEFT:  pos_new.row += 1;
						break;
			case RIGHT: pos_new.row -= 1;
						break;
			case NONE : break;
		}
		if(!grid_cell_occupied(&grid_inst, pos_new) && grid_cell_inbounds(&grid_inst, pos_new)) {
			array_cell_t_destroy(&(ghost.path));
			ghost.path = path_get(&grid_inst, ghost.pos, pos_new);
			move_piece(&pacman, pos_new);
		}

		if (!array_cell_t_empty(&(ghost.path)) && !num_rounds_ghosts_still)
		{
			//cell_t pos_new = array_cell_t_pop(&(ghost.path));
			//move_piece(&ghost, pos_new);
		}

		num_rounds_ghosts_still = num_rounds_ghosts_still ? num_rounds_ghosts_still - 1 : 0;

		timer_set(delay, NULL);
	}
	led_send(&led_inst, 0, led_update_period_usecs);

	if(N64ButtonPressed(&n64, BUTTON_R)) {
		state = kill;
	}
	else if(N64ButtonPressed(&n64, BUTTON_START)) {
		state = calibrate;
	}
	/*else if (grid_cell_match(ghost.pos, pacman.pos)) {
		state = lost;
	}
	else if(grid_count(&grid_inst, 'D') == 0) {
		state = won;
	}*/ else {
		state = advance;
	}
}

void GameWon(int init)
{
	send_robot_cmd(STOP);
	led_rand_show(&led_inst, 0, led_inst.num_leds, 0, 1000000, 3000000);
	state = idle;
}

void GameLost(int init)
{
	send_robot_cmd(STOP);
	led_set_colors(&led_inst, 0, led_inst.num_leds, 0, COLOR_RED);
	led_send(&led_inst, 0, 3000000);
	state = idle;
}

void GameIdle(int init) {
	if(init) {
		grid_init(&grid_inst, pacman_occupancy);
		set_colors_grid(&led_inst, &grid_inst, 0);
	}
	if(N64ButtonPressed(&n64, BUTTON_START)) {
		state = calibrate;
	}
	else if(N64ButtonPressed(&n64, BUTTON_Z)) {
		state = kill;
	}
	else {
		state = idle;
	}
	led_send(&led_inst, 0, 1000000);
}

void GameKill(int init)
{
	send_robot_cmd(STOP);
	led_set_colors(&led_inst, 0, led_inst.num_leds, 0, COLOR_BLACK);
	led_send(&led_inst, 0, 0);
	N64Disable(&n64);
	led_destroy(&led_inst);
	grid_destroy(&grid_inst);
	grid_destroy(&grid_inst_init);
	state = kill;
}
