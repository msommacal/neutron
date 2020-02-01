#if ! defined (MENU)
#define MENU

#include <stdbool.h>

#define ENTER_KEYCODE 32 	// Space bar
#define UP_KEYCODE 122		// Z
#define DOWN_KEYCODE 115	// S

#define MAX_SIZE_MENU_MAIN_ITEMS 10
#define MAX_SIZE_MENU_AUX_ITEMS 10

struct MenuItem {
	char* text;
	int value;
};

struct Menu {
	char* question;
	struct MenuItem* main_items[MAX_SIZE_MENU_MAIN_ITEMS];
	int nb_main_items;
	struct MenuItem* aux_items[MAX_SIZE_MENU_AUX_ITEMS];
	int nb_aux_items;
};

void* init_menu_item(char* text, int value);
void debug_menu_item(struct MenuItem* mi);
void clear_menu_item(struct MenuItem* mi);

void* init_menu(char* question);
void debug_menu(struct Menu* m);
void add_menu_item(struct Menu* m, struct MenuItem* mi, bool is_main_item);
void clear_menu(struct Menu* m);

int getch(void);
void print_menu_item_arrow(int real_position, int arrow_position);
int print_menu(struct Menu* m);

#endif
