#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "menu.h"

void* init_square(int i, int j, int owner) {
	struct Square *s;
	s = (struct Square*) malloc(sizeof(struct Square));

	s->i = i;
	s->j = j;
	s->owner = owner;
	strcpy(s->label, "       ");

	return s;
}

void debug_square(struct Square* s) {
	printf("Square (i: %d, j: %d, owner: %d, label: \"%s\")\n", s->i, s->j, s->owner, s->label);
}

void clear_square(struct Square* s) {
	free(s);
}

int get_other_player(struct Game* g) {
	if (PLAYER_A == g->current_player) {
		return PLAYER_B;
	} else {
		return PLAYER_A;
	}
}

void change_current_player(struct Game* g) {
	g->current_player = get_other_player(g);
}

void get_player_string(struct Game* g, char* player_string) {
	if (PLAYER_A == g->current_player) {
		strcpy(player_string, PLAYER_A_TEXT_COLOR);
		strcat(player_string, "Player A");
	} else {
		strcpy(player_string, PLAYER_B_TEXT_COLOR);
		strcat(player_string, "Player B");
	}

	strcat(player_string, COLOR_RESET);
}

void get_winner_string(struct Game* g, char* winner_string) {
	if (PLAYER_A == g->winner) {
		strcpy(winner_string, PLAYER_A_TEXT_COLOR);
		strcat(winner_string, "Player A");
	} else {
		strcpy(winner_string, PLAYER_B_TEXT_COLOR);
		strcat(winner_string, "Player B");
	}

	strcat(winner_string, COLOR_RESET);
}

void get_board_string(struct Game* g, char* board_string) {
	char square_string[SIZE_SQUARE_STRING];
	char integer_string[SIZE_INTEGER_STRING];
	int i,j;

	strcpy(board_string, "\0");

	for (i=4;i>=0;i--) {
		strcat(board_string, "   +");
		for (j=0;j<SIZE_BOARD;j++) strcat(board_string, "-------+");
		strcat(board_string, "\n");
	
		strcat(board_string, "   |");
		for (j=0;j<SIZE_BOARD;j++) {
			get_square_string(g->board[i][j], square_string, false);
			strcat(board_string, square_string);
			strcat(board_string, "|");
		};
		strcat(board_string, "\n");
	
		sprintf(integer_string, "%d", i+1);
		strcat(board_string, " ");
		strcat(board_string, integer_string);
		strcat(board_string, " |");
		for (j=0;j<SIZE_BOARD;j++) {
			get_square_string(g->board[i][j], square_string, true);
			strcat(board_string, square_string);
			strcat(board_string, "|");
		};
		strcat(board_string, "\n");
	
		// third sub-line
		strcat(board_string, "   |");
		for (j=0;j<SIZE_BOARD;j++) {
			get_square_string(g->board[i][j], square_string, false);
			strcat(board_string, square_string);
			strcat(board_string, "|");
		};
		strcat(board_string, "\n");
	}
	
	strcat(board_string, "   +");
	for (j=0;j<SIZE_BOARD;j++) strcat(board_string, "-------+");
	strcat(board_string, "\n");

	strcat(board_string, "    ");
	for (j=0;j<SIZE_BOARD;j++) {
		sprintf(integer_string, "%d", j+1);
		strcat(board_string, "   ");
		strcat(board_string, integer_string);
		strcat(board_string, "    ");
	}
	strcat(board_string, "\n\n ");
}

void get_square_string(struct Square* s, char* square_string, bool show_label) {
	if (s->owner == EMPTY) {
		strcpy(square_string, COLOR_EMPTY);
	} else if (s->owner == PLAYER_A) {
		strcpy(square_string, COLOR_PLAYER_A);
	} else if (s->owner == PLAYER_B) {
		strcpy(square_string, COLOR_PLAYER_B);
	} else if (s->owner == NEUTRON) {
		strcpy(square_string, COLOR_NEUTRON);
	}
	if (show_label) {
		strcat(square_string, s->label);
	} else {
		strcat(square_string, "       ");
	}
	strcat(square_string, COLOR_RESET);
}

void debug_game(struct Game* g) {
	int i,j;

	// squares from player A
	printf("PLAYER A:\n");
	for (i=0;i<SIZE_BOARD;i++) {
		debug_square(g->squares_player_A[i]);
	}

	// squares from player B
	printf("PLAYER B:\n");
	for (i=0;i<SIZE_BOARD;i++) {
		debug_square(g->squares_player_B[i]);
	}

	// neutron
	printf("NEUTRON\n");
	debug_square(g->neutron);

	printf("\n");
	
	// board
	for (i=0;i<SIZE_BOARD;i++) {
		printf("LINE %d:\n", i);
		for (j=0;j<SIZE_BOARD;j++) {
			debug_square(g->board[i][j]);
		}
	}
}

bool is_empty(struct Square* s) {
	return (s->owner == EMPTY);
}

void choose_game_over(struct Game* g) {
	struct Menu* m;
	struct MenuItem* menu_main_items[2];

	char board_string[SIZE_BOARD_STRING];
	char winner_string[SIZE_PLAYER_STRING];

	// Initialization of the menu items
	menu_main_items[0] = init_menu_item("Restart", 0);
	menu_main_items[1] = init_menu_item("Quit", -1);

	// Get the board string
	get_board_string(g, board_string);

	// Add the question of the menu
	get_winner_string(g, winner_string);
	strcat(board_string, "Congratulation! ");
	strcat(board_string, winner_string);
	strcat(board_string, " has won!");

	// Initialization of the menu
	m = init_menu(board_string);

	// Add the main items of the menu (the movable pieces)
	add_menu_item(m, menu_main_items[0], true);
	add_menu_item(m, menu_main_items[1], true);
	
	// Print the menu
	g->chosen_game_over = print_menu(m);

	// We free the menu
	clear_menu(m);
	clear_menu_item(menu_main_items[0]);
	clear_menu_item(menu_main_items[1]);

	if (-1 == g->chosen_game_over) {
		clear_game(g);
		exit(0);
	}
}

void add_piece_labels(struct Game* g) {
	char label = 'A';
	int i;

	if (PLAYER_A == g->current_player) {
		for (i=0;i<SIZE_BOARD;i++) {
			if (g->movable_pieces[i]) {
				sprintf(g->squares_player_A[i]->label, "   %c   ", label);
			}
			label++;
		}
	} else if (g->current_player == PLAYER_B) {
		for (i=0;i<SIZE_BOARD;i++) {
			if (g->movable_pieces[i]) {
				sprintf(g->squares_player_B[i]->label, "   %c   ", label);
			}
			label++;
		}
	}
}

void remove_piece_labels(struct Game* g) {
	int i;

	if (PLAYER_A == g->current_player) {
		for (i=0;i<SIZE_BOARD;i++) {
			strcpy(g->squares_player_A[i]->label, "       ");
		}
	} else if (g->current_player == PLAYER_B) {
		for (i=0;i<SIZE_BOARD;i++) {
			strcpy(g->squares_player_B[i]->label, "       ");
		}
	}
}

void* init_game(void) {
	struct Game* g;
	g = (struct Game*) malloc(sizeof(struct Game));

	int i,j;

	g->chosen_game_over = EMPTY;
	g->chosen_piece = EMPTY;
	g->chosen_piece_direction = EMPTY;
	g->chosen_neutron_direction = EMPTY;
	g->current_player = PLAYER_A;

	for (i=0;i<NB_PIECES;i++) {
		g->movable_pieces[i] = true;
	}

	for (i=0;i<NB_DIRECTIONS;i++) {
		g->movable_directions[i] = true;
	}

	g->winner = EMPTY;

	// First line
	for (j=0;j<SIZE_BOARD;j++) {
		g->board[0][j] = init_square(0,j,PLAYER_A);
		g->squares_player_A[j] = g->board[0][j];
	}

	// Middle lines
	for (i=1;i<SIZE_BOARD-1;i++) {
		for (j=0;j<SIZE_BOARD;j++) {
			g->board[i][j] = init_square(i,j,EMPTY);
		}
	}

	// Last line
	for (j=0;j<SIZE_BOARD;j++) {
		g->board[SIZE_BOARD-1][j] = init_square(SIZE_BOARD-1,j,PLAYER_B);
		g->squares_player_B[j] = g->board[SIZE_BOARD-1][j];
	}

	// Neutron
	g->board[I_NEUTRON][I_NEUTRON]->owner = NEUTRON;
	g->neutron = g->board[I_NEUTRON][J_NEUTRON];

	return g;
}

void set_square_label(struct Square* s, char* label, int action) {
	if (ADD_LABEL == action) {
		strcpy(s->label, label);
	} else if (REMOVE_LABEL == action) {
		strcpy(s->label, "       ");
	}
}

void follow_direction(struct Game* g, struct Square* src, int direction, struct Square** dest, int action) {
	int i = src->i;
	int j = src->j;

	switch (direction) {
		case NORTH:
			while (i < SIZE_BOARD-1 && is_empty(g->board[i+1][j])) {
				i++;
				set_square_label(g->board[i][j], "   N   ", action);
			}
			break;
		case NORTH_EAST:
			while (i < SIZE_BOARD-1 && j < SIZE_BOARD-1 && is_empty(g->board[i+1][j+1])) {
				i++;
				j++;
				set_square_label(g->board[i][j], "  N-E  ", action);
			}
			break;
		case EAST:
			while (j < SIZE_BOARD-1 && is_empty(g->board[i][j+1])) {
				j++;
				set_square_label(g->board[i][j], "   E   ", action);
			}
			break;
		case SOUTH_EAST:
			while (i > 0 && j < SIZE_BOARD-1 && is_empty(g->board[i-1][j+1])) {
				i--;
				j++;
				set_square_label(g->board[i][j], "  S-E  ", action);
			}
			break;
		case SOUTH:
			while (i > 0 && is_empty(g->board[i-1][j])) {
				i--;
				set_square_label(g->board[i][j], "   S   ", action);
			}
			break;
		case SOUTH_WEST:
			while (i > 0 && j > 0 && is_empty(g->board[i-1][j-1])) {
				i--;
				j--;
				set_square_label(g->board[i][j], "  S-W  ", action);
			}
			break;
		case WEST:
			while (j > 0 && is_empty(g->board[i][j-1])) {
				j--;
				set_square_label(g->board[i][j], "   W   ", action);
			}
			break;
		case NORTH_WEST:
			while (i < SIZE_BOARD-1 && j > 0 && is_empty(g->board[i+1][j-1])) {
				i++;
				j--;
				set_square_label(g->board[i][j], "  N-W  ", action);
			}
	}

	*dest = g->board[i][j];
}

bool is_square_movable(struct Game* g, struct Square* s) {
	struct Square* dest;
	int d;
	
	bool r = false;

	for (d=NORTH;d<=NORTH_WEST;d++) {
		follow_direction(g, s, d, &dest, NONE);
		g->movable_directions[d] = (dest != s);
		r = (r || (dest != s));
	}

	return r;
}

bool is_neutron_movable(struct Game* g) {
	return is_square_movable(g, g->neutron);
}

bool is_piece_movable(struct Game* g) {
	if (PLAYER_A == g->current_player) {
		return is_square_movable(g, g->squares_player_A[g->chosen_piece]);
	} else {
		return is_square_movable(g, g->squares_player_B[g->chosen_piece]);
	}
}

bool has_movable_piece(struct Game* g) {
	bool r = false;
	int i;

	if (PLAYER_A == g->current_player) {
		for (i=0;i<SIZE_BOARD;i++) {
			g->movable_pieces[i] = is_square_movable(g, g->squares_player_A[i]);
			r = (r || g->movable_pieces[i]);
		}
	} else {
		for (i=0;i<SIZE_BOARD;i++) {
			g->movable_pieces[i] = is_square_movable(g, g->squares_player_B[i]);
			r = (r || g->movable_pieces[i]);
		}
	}

	return r;
}

void add_direction_labels(struct Game* g, struct Square* s) {
	struct Square* dest;
	int d;

	set_square_label(s, "   X   ", ADD_LABEL);

	for (d=NORTH;d<=NORTH_WEST;d++) {
		follow_direction(g, s, d, &dest, ADD_LABEL);
	}
}

void remove_direction_labels(struct Game* g, struct Square* s) {
	struct Square* dest;
	int d;

	set_square_label(s, "       ", REMOVE_LABEL);

	for (d=NORTH;d<=NORTH_WEST;d++) {
		follow_direction(g, s, d, &dest, REMOVE_LABEL);
	}
}

void choose_piece(struct Game* g) {
	struct Menu* m;
	struct MenuItem* menu_main_items[NB_PIECES];
	struct MenuItem* menu_aux_items[1];

	char board_string[SIZE_BOARD_STRING];
	char player_string[SIZE_PLAYER_STRING];

	int i;

	// Initialization of the menu items
	menu_main_items[0] = init_menu_item("Piece A", 0);
	menu_main_items[1] = init_menu_item("Piece B", 1);
	menu_main_items[2] = init_menu_item("Piece C", 2);
	menu_main_items[3] = init_menu_item("Piece D", 3);
	menu_main_items[4] = init_menu_item("Piece E", 4);
	menu_aux_items[0] = init_menu_item("Quit", -1);

	// Add the labels on the pieces
	add_piece_labels(g);

	// Get the board string
	get_board_string(g, board_string);

	// Add the question of the menu
	get_player_string(g, player_string);
	strcat(board_string, player_string);
	strcat(board_string, ", please choose one of your piece:");

	// Initialization of the menu
	m = init_menu(board_string);

	// Add the main items of the menu (the movable pieces)
	for (i=0;i<NB_PIECES;i++) {
		if (g->movable_pieces[i] == true) {
			add_menu_item(m, menu_main_items[i], true);
		}
	}
	
	// Add the auxiliary item of the menu (Quit)
	add_menu_item(m, menu_aux_items[0], false);

	// Print the menu
	g->chosen_piece = print_menu(m);

	// Remove the labels
	remove_piece_labels(g);

	// We free the menu
	clear_menu(m);
	for (i=0;i<NB_PIECES;i++) {
		clear_menu_item(menu_main_items[i]);
	}
	clear_menu_item(menu_aux_items[0]);

	if (-1 == g->chosen_piece) exit(0);
}

void choose_piece_direction(struct Game* g) {
	struct Menu* m;
	struct MenuItem* menu_main_items[NB_DIRECTIONS];
	struct MenuItem* menu_aux_items[1];

	char board_string[SIZE_BOARD_STRING];
	char player_string[SIZE_PLAYER_STRING];

	int i;

	// Initialization of the menu items
	menu_main_items[0] = init_menu_item("North (N)", NORTH);
	menu_main_items[1] = init_menu_item("North-East (N-E)", NORTH_EAST);
	menu_main_items[2] = init_menu_item("East (E)", EAST);
	menu_main_items[3] = init_menu_item("South-East (S-E)", SOUTH_EAST);
	menu_main_items[4] = init_menu_item("South (S)", SOUTH);
	menu_main_items[5] = init_menu_item("South-West (S-W)", SOUTH_WEST);
	menu_main_items[6] = init_menu_item("West (W)", WEST);
	menu_main_items[7] = init_menu_item("North-West (N-W)", NORTH_WEST);
	menu_aux_items[0] = init_menu_item("Cancel", -1);

	// Add the labels on the directions
	if (PLAYER_A == g->current_player) {
		add_direction_labels(g, g->squares_player_A[g->chosen_piece]);
	} else {
		add_direction_labels(g, g->squares_player_B[g->chosen_piece]);
	}

	// Get the board string
	get_board_string(g, board_string);

	// Add the question of the menu
	get_player_string(g, player_string);
	strcat(board_string, player_string);
	strcat(board_string, ", please choose a direction for your piece:");

	// Initialization of the menu
	m = init_menu(board_string);

	// Add the main items of the menu (the movable directions)
	for (i=0;i<NB_DIRECTIONS;i++) {
		if (g->movable_directions[i] == true) {
			add_menu_item(m, menu_main_items[i], true);
		}
	}
	
	// Add the auxiliary item of the menu (Cancel)
	add_menu_item(m, menu_aux_items[0], false);

	// Print the menu
	g->chosen_piece_direction = print_menu(m);

	// Remove the labels
	if (PLAYER_A == g->current_player) {
		remove_direction_labels(g, g->squares_player_A[g->chosen_piece]);
	} else {
		remove_direction_labels(g, g->squares_player_B[g->chosen_piece]);
	}

	// We free the menu
	clear_menu(m);
	for (i=0;i<5;i++) clear_menu_item(menu_main_items[i]);
	for (i=0;i<1;i++) clear_menu_item(menu_aux_items[i]);
}

void choose_neutron_direction(struct Game* g) {
	struct Menu* m;
	struct MenuItem* menu_main_items[NB_DIRECTIONS];
	struct MenuItem* menu_aux_items[1];

	char board_string[SIZE_BOARD_STRING];
	char player_string[SIZE_PLAYER_STRING];

	int i;

	// Initialization of the menu items
	menu_main_items[0] = init_menu_item("North (N)", NORTH);
	menu_main_items[1] = init_menu_item("North-East (N-E)", NORTH_EAST);
	menu_main_items[2] = init_menu_item("East (E)", EAST);
	menu_main_items[3] = init_menu_item("South-East (S-E)", SOUTH_EAST);
	menu_main_items[4] = init_menu_item("South (S)", SOUTH);
	menu_main_items[5] = init_menu_item("South-West (S-W)", SOUTH_WEST);
	menu_main_items[6] = init_menu_item("West (W)", WEST);
	menu_main_items[7] = init_menu_item("North-West (N-W)", NORTH_WEST);
	menu_aux_items[0] = init_menu_item("Quit", -1);

	// Add the labels on the directions
	add_direction_labels(g, g->neutron);

	// Get the board string
	get_board_string(g, board_string);

	// Add the question of the menu
	get_player_string(g, player_string);
	strcat(board_string, player_string);
	strcat(board_string, ", please choose a direction for the ");
	strcat(board_string, NEUTRON_TEXT_COLOR);
	strcat(board_string, "neutron");
	strcat(board_string, COLOR_RESET);
	strcat(board_string, ":");

	// Initialization of the menu
	m = init_menu(board_string);

	// Add the main items of the menu (the movable directions)
	for (i=0;i<NB_DIRECTIONS;i++) {
		if (g->movable_directions[i] == true) {
			add_menu_item(m, menu_main_items[i], true);
		}
	}
	
	// Add the auxiliary item of the menu (Cancel)
	add_menu_item(m, menu_aux_items[0], false);

	// Print the menu
	g->chosen_neutron_direction = print_menu(m);

	// Remove the labels
	remove_direction_labels(g, g->neutron);

	// We free the menu
	clear_menu(m);
	for (i=0;i<5;i++) clear_menu_item(menu_main_items[i]);
	for (i=0;i<1;i++) clear_menu_item(menu_aux_items[i]);

	if (-1 == g->chosen_neutron_direction) exit(0);
}

void move_square(struct Square** src, struct Square* dest) {
	// remove a piece from a square
	int owner = (*src)->owner;
	(*src)->owner = EMPTY;

	// add a piece on a square
	dest->owner = owner;
	*src = dest;
}

void move_neutron(struct Game* g) {
	struct Square* dest;

	follow_direction(g, g->neutron, g->chosen_neutron_direction, &dest, NONE);

	move_square(&(g->neutron), dest);
}

void move_piece(struct Game* g) {
	struct Square* dest;
	struct Square* src;

	if (PLAYER_A == g->current_player) {
		src = g->squares_player_A[g->chosen_piece];
	} else {
		src = g->squares_player_B[g->chosen_piece];
	}

	follow_direction(g, src, g->chosen_piece_direction, &dest, NONE);

	move_square(&(src), dest);

	if (PLAYER_A == g->current_player) {
		g->squares_player_A[g->chosen_piece] = dest;
	} else {
		g->squares_player_B[g->chosen_piece] = dest;
	}
}

void clear_game(struct Game* g) {
	int i,j;
	for (i=0;i<SIZE_BOARD;i++) {
		for (j=0;j<SIZE_BOARD;j++) {
			clear_square(g->board[i][j]);
		}
	}

	free(g);
}