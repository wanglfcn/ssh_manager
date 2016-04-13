#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <sys/ioctl.h>
#include "Node.h"

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{
	char result[200];
	Node *node_head = read_config("/Users/wang/work/others/ssh_manager/machine.conf");

	ITEM **item_list;
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
	struct ttysize ts;
	ioctl(0, TIOCGSIZE, &ts);
	window = newwin(ts.ts_lines - 8, ts.ts_cols - 8, 4, 4);

	keypad(window, TRUE);

	set_menu_win(menu, window);
	set_menu_sub(menu, derwin(window, ts.ts_lines - 12, ts.ts_cols - 12, 3, 1));
	set_menu_format(menu, ts.ts_lines - 12, 1);
	set_menu_mark(menu, " > ");

	box(window, 0, 0);
	print_in_middle(window, 1, 0, ts.ts_cols, "Machine List", COLOR_PAIR(1));
	mvwaddch(window, 2, 0, ACS_LTEE);
	mvwhline(window, 2, 1, ACS_HLINE, ts.ts_cols - 5);
	mvwaddch(window, 2, ts.ts_cols - 1, ACS_RTEE);

	post_menu(menu);
	wrefresh(window);

	mvprintw(LINES - 2, 0, "q to exit");
	refresh();

	while ((c = wgetch(window)) != 'q')
	{
		switch (c)
		{
			case KEY_DOWN:
			case 'j':
				{
					menu_driver(menu, REQ_DOWN_ITEM);
					ITEM *cur = current_item(menu);
					break;
				}
			case 'k':
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case 'i':
			case 'l':
			{
				ITEM *cur = current_item(menu);
				Node *node = get_node_by_index(cur->index, node_head);
				if (node->type == GROUP)
				{
					expand_node(node, 1);
					unpost_menu(menu);
					item_list = get_item_list(node_head);
					set_menu_items(menu, item_list);
					post_menu(menu);
					int index = get_group_index(node);
					for (int i = 0; i < index; i++) {
						menu_driver(menu, REQ_DOWN_ITEM);
					}
					wrefresh(window);
				}
				break;
			}
			case 'o':
			case 'h':
			{
				ITEM *cur = current_item(menu);
				Node *node = get_node_by_index(cur->index, node_head);
				if (node->type != NODE)
				{
					expand_node(node, 0);
					unpost_menu(menu);
					item_list = get_item_list(node_head);
					set_menu_items(menu, item_list);
					post_menu(menu);
					int index = get_group_index(node);
					for (int i = 0; i < index; i++) {
						menu_driver(menu, REQ_DOWN_ITEM);
					}
					wrefresh(window);
				}
				break;
			}
			case KEY_ENTER:
			case 'e':
			{
				ITEM *cur = current_item(menu);
				Node *node = get_node_by_index(cur->index, node_head);
				sprintf(result, "%s@%s\t%s", node->user_name, node->ip, node->password);
				mvprintw(LINES - 1, 0, result);
				refresh();
			}
		}
		wrefresh(window);
	}

	free_node(node_head);

	unpost_menu(menu);
	free_menu(menu);

	endwin();
	printf("%s\n", result);
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
