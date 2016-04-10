#ifndef NODE_H
#define NODE_H
typedef enum{
	GROUP = 0,
	NODE,
	ITEM
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

#endif
