#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include "Node.h"

int main()
{
	Node *node_head = read_config("/Users/wang/work/others/ssh_manager/machine.conf");

	ITEM **node_items;
	int node_count = 0;
	int i;
	int c;

	MENU *menu;
	WINDOW *window;

	Node *node = node_head;

	while(node)
	{
		node_count ++;
		node = node->next;
	}

	node_items = (ITEM **)calloc(node_count + 1, sizeof(ITEM **));
	node = node_head;

	for (i = 0; i < node_count; i ++)
	{
		node_items[i] = new_item(node->item_name, node->ip);
		node = node->next;
	}
	node_items[i] = (ITEM *)NULL;

	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);

	menu = new_menu(node_items);
	window = newwin(10, 40, 4, 4);

	keypad(window, TRUE);

	set_menu_win(menu, window);
	set_menu_sub(menu, derwin(window, 6, 38, 3, 1));
	set_menu_mark(menu, ">");

	box(window, 0, 0);
	mvwaddch(window, 2, 0, ACS_LTEE);
	mvwhline(window, 2, 1, ACS_HLINE, 38);
	mvwaddch(window, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
	refresh();

	while ((c = wgetch(window)) != KEY_F(1))
	{
		switch (c)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
		}
	}

	unpost_menu(menu);
	free_menu(menu);

	for (i = 0; i < node_count; i ++)
	{
		free_item(node_items[i]);
	}
	endwin();
}
