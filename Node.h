#ifndef NODE_H
#define NODE_H
#include <menu.h>
typedef enum{
	GROUP = 0,
	CHILD,
	NODE
} NodeType;

typedef struct Node{
	NodeType type;
	int is_expand;
	char item_name[120];
	char ip[120];
	char user_name[120];
	char password[120];
	struct Node *next;
	struct Node *prev;
} Node;



Node *new_node(NodeType type, char *item_name, char *ip, char *user_name, char *password);

Node *gen_node(char *config);

Node * read_config(char *path);

void free_node(Node *node);

Node *get_node_by_index(int index, Node *head);

void expand_node(Node *head, int status);

ITEM **get_item_list(Node *head);

int get_group_index(Node *node);

#endif
