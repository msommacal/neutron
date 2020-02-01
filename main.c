#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "game.h"

int main(int argc, char const *argv[]) {
	struct Game* g;

	do {
		g = init_game();

		do {
			do {
				// The player chooses one of his piece
				choose_piece(g);

				// Get the possible directions for the piece
				is_piece_movable(g);

				// The player chooses a direction for the selected piece
				choose_piece_direction(g);
			} while (-1 == g->chosen_piece_direction);
		
			// The piece is moved to the chosen square
			move_piece(g);

			// It's the other player's turn
			change_current_player(g);
		
			// Check if the player can move the neutron
			if (!is_neutron_movable(g)) {
				g->winner = get_other_player(g);
				break;
			}

			// The player chooses a direction for the neutron
			choose_neutron_direction(g);

			// The neutron is moved to the chosen square
			move_neutron(g);

			// Check if there is a winner (neutron)
			if (0 == g->neutron->i) {
				g->winner = PLAYER_A;
				break;
			} else if (SIZE_BOARD-1 == g->neutron->i) {
				g->winner = PLAYER_B;
				break;
			}

			// Check if the player can move one of his piece
			if (!has_movable_piece(g)) {
				g->winner = get_other_player(g);
			}
		} while (EMPTY == g->winner);

		// The players can begin a new game
		choose_game_over(g);

		// We free the current game
		clear_game(g);
	} while (1);

	return 0;
}