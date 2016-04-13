#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Node.h"

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{
	Node *node_head = read_config("/Users/wang/work/others/ssh_manager/machine.conf");

	ITEM **item_list;
	int i;
	int c;

	MENU *menu;
	WINDOW *window;

	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);

	item_list = get_item_list(node_head);
	menu = new_menu(item_list);
	window = newwin(10, 40, 4, 4);

	keypad(window, TRUE);

	set_menu_win(menu, window);
	set_menu_sub(menu, derwin(window, 6, 38, 3, 1));
	set_menu_format(menu, 5, 1);
	set_menu_mark(menu, ">");

	box(window, 0, 0);
	print_in_middle(window, 1, 0, 40, "My Menu", COLOR_PAIR(1));
	mvwaddch(window, 2, 0, ACS_LTEE);
	mvwhline(window, 2, 1, ACS_HLINE, 38);
	mvwaddch(window, 2, 39, ACS_RTEE);

	post_menu(menu);
	wrefresh(window);

	mvprintw(LINES - 2, 0, "F1 to exit");
	refresh();

	while ((c = wgetch(window)) != KEY_F(1))
	{
		switch (c)
		{
			case KEY_DOWN:
				{
					menu_driver(menu, REQ_DOWN_ITEM);
					ITEM *cur = current_item(menu);
					char buf[80];
					sprintf(buf, "cur index:%d", cur->index);
					mvprintw(LINES - 1, 0, buf);
					refresh();
					break;
				}
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
		}
		wrefresh(window);
	}

	free_node(node_head);

	unpost_menu(menu);
	free_menu(menu);

	endwin();
	return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
