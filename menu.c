#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "menu.h"

void* init_menu_item(char* text, int value) {
	struct MenuItem* mi;
	mi = (struct MenuItem*) malloc(sizeof(struct MenuItem));

	mi->text = strdup(text);
	mi->value = value;

	return mi;
}

void debug_menu_item(struct MenuItem* mi) {
	printf("MenuItem (text: %s, value: %d)\n", mi->text, mi->value);
}

void clear_menu_item(struct MenuItem* mi) {
	free(mi->text);
	free(mi);
}



void* init_menu(char* question) {
	struct Menu* m;
	m = (struct Menu*) malloc(sizeof(struct Menu));

	m->question = strdup(question);
	m->nb_main_items = 0;
	m->nb_aux_items = 0;

	return m;
}

void debug_menu(struct Menu* m) {
	printf("Menu (question: %s, nb_main_items: %d, nb_aux_items: %d)\n", m->question, m->nb_main_items, m->nb_aux_items);
	int i;

	for (i=0;i<m->nb_main_items;i++) {
		printf(" - ");debug_menu_item(m->main_items[i]);
	}

	for (i=0;i<m->nb_aux_items;i++) {
		printf(" - ");debug_menu_item(m->aux_items[i]);
	}
}

void add_menu_item(struct Menu* m, struct MenuItem* mi, bool is_main_item) {
	if (is_main_item) {
		m->main_items[m->nb_main_items] = mi;
		m->nb_main_items += 1;
	} else {
		m->aux_items[m->nb_aux_items] = mi;
		m->nb_aux_items += 1;
	}
}

void clear_menu(struct Menu* m) {
	free(m->question);
	free(m);
}

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void print_menu_item_arrow(int real_position, int arrow_position) {
	if (real_position == arrow_position) {
		printf(" > ");	
	} else {
		printf("   ");
	}
}

int print_menu(struct Menu* m) {
	system("clear");

	int key_pressed = 0;
	int position = 0;

	int i;

	while (key_pressed != ENTER_KEYCODE) {
		system("clear");
		
		printf("\n");
		printf("%s\n", m->question);
		printf("\n");

		for (i=0;i<m->nb_main_items;i++) {
			print_menu_item_arrow(i,position);printf("%s\n", m->main_items[i]->text);			
		}

		printf("\n");

		for (i=0;i<m->nb_aux_items;i++) {
			print_menu_item_arrow(m->nb_main_items+i,position);printf("%s\n", m->aux_items[i]->text);			
		}

		printf("\n");

		key_pressed = getch();

		if (key_pressed == DOWN_KEYCODE && position != m->nb_main_items+m->nb_aux_items-1) {
			position++;
		} else if (key_pressed == UP_KEYCODE && position != 0) {
			position--;
		}
	}

	if (position < m->nb_main_items) {
		return m->main_items[position]->value;
	} else {
		return m->aux_items[position-m->nb_main_items]->value;
	}
}