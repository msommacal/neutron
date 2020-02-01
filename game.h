#if ! defined (GAME)
#define GAME

#include <stdbool.h>

#define NB_PIECES 5
#define NB_DIRECTIONS 8
#define SIZE_BOARD 5
#define I_NEUTRON 2
#define J_NEUTRON 2

#define SIZE_BOARD_STRING 2000
#define SIZE_LABEL_STRING 10
#define SIZE_PLAYER_STRING 100
#define SIZE_SQUARE_STRING  30
#define SIZE_INTEGER_STRING 12

#define EMPTY 0
#define PLAYER_A 1
#define PLAYER_B 2
#define NEUTRON 3

#define COLOR_EMPTY "\033[;90m"
#define COLOR_PLAYER_A "\033[;44m"
#define COLOR_PLAYER_B "\033[;41m"
#define COLOR_NEUTRON "\033[;42m"
#define COLOR_RESET "\033[0m"
#define PLAYER_A_TEXT_COLOR "\033[;34m"
#define PLAYER_B_TEXT_COLOR "\033[;31m"
#define NEUTRON_TEXT_COLOR "\033[;32m"

#define NORTH 0
#define NORTH_EAST 1
#define EAST 2
#define SOUTH_EAST 3
#define SOUTH 4
#define SOUTH_WEST 5
#define WEST 6
#define NORTH_WEST 7

#define ADD_LABEL 1
#define REMOVE_LABEL -1
#define NONE 0

struct Square {
	int i;
	int j;
	int owner;
	char label[SIZE_LABEL_STRING];
};

void* init_square(int i, int j, int owner);

void debug_square(struct Square* s);

void clear_square(struct Square* s);

struct Game {
	struct Square* board[SIZE_BOARD][SIZE_BOARD];
	struct Square* squares_player_A[SIZE_BOARD];
	struct Square* squares_player_B[SIZE_BOARD];
	struct Square* neutron;

	int chosen_game_over;
	int chosen_piece;
	int chosen_piece_direction;
	int chosen_neutron_direction;
	int current_player;
	bool movable_pieces[NB_PIECES];
	bool movable_directions[NB_DIRECTIONS];
	int winner;
};

int get_other_player(struct Game* g);

void change_current_player(struct Game* g);

void get_player_string(struct Game* g, char* player_string);

void get_board_string(struct Game* g, char* board_string);

void get_square_string(struct Square* s, char* square_string, bool show_label);

void debug_game(struct Game* g);

bool is_empty(struct Square* s);

void get_winner_string(struct Game* g, char* winner_string);

void choose_game_over(struct Game* g);

void add_piece_labels(struct Game* g);

void remove_piece_labels(struct Game* g);

void* init_game(void);

void set_square_label(struct Square* s, char* label, int action);

void follow_direction(struct Game* g, struct Square* src, int direction, struct Square** dest, int action);

bool is_square_movable(struct Game* g, struct Square* s);

bool is_neutron_movable(struct Game* g);

bool is_piece_movable(struct Game* g);

bool has_movable_piece(struct Game* g);

void add_direction_labels(struct Game* g, struct Square* s);

void remove_direction_labels(struct Game* g, struct Square* s);

void choose_piece(struct Game* g);

void choose_piece_direction(struct Game* g);

void choose_neutron_direction(struct Game* g);

void move_square(struct Square** src, struct Square* dest);

void move_neutron(struct Game* g);

void move_piece(struct Game* g);

void clear_game(struct Game* g);

#endif
