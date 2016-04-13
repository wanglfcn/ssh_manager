#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Node.h"

Node *new_node(NodeType type, char *item_name, char *ip, char *user_name, char *password)
{
	Node * res = (Node *)malloc(sizeof(Node));
	if (res != NULL)
	{
		res->type = type;
		res->is_expand = 0;
		res->next = NULL;
		res->prev = NULL;
		strcpy(res->ip, ip);
		if (type == CHILD)
		{
			strcpy(res->item_name, item_name);
		}
		else
		{
			sprintf(res->item_name, "[#] %s", item_name);
		}
		strcpy(res->user_name, user_name);
		strcpy(res->password, password);
	}
	return res;
}

Node *gen_node(char *config)
{
	Node *res = NULL;
	char empty_str = 0;
	char *para[5] = {&empty_str, &empty_str, &empty_str, &empty_str, &empty_str};
	int index = 0;

	while (*config && index < 5)
	{
		if (*config == '\t' || *config == '\n')
		{
			*config = 0;
			index ++;
		}
		else if (para[index] == &empty_str && *config != 0)
		{
			para[index] = config;
		}
		config ++;
	}

	NodeType type;
	if (strcmp(para[0], "group") == 0)
	{
		type = GROUP;
	}
	else if (strcmp(para[0], "node") == 0)
	{
		type = NODE;
	}
	else
	{
		type = CHILD;
	}

	res = new_node(type, para[1], para[2], para[3], para[4]);

	return res;
}

Node * read_config(char *path)
{
	FILE *fp = fopen(path, "r");
	Node *res = NULL;
	Node *cur = NULL;
	char buf[1024];
	if (fp != NULL)
	{
		while (!feof(fp))
		{
			if (fgets(buf, 1024, fp) == NULL)
			{
				break;
			}

			Node *node = gen_node(buf);
			if (node == NULL) continue;
			if (res == NULL)
			{
				res = node;
				cur = node;
			}
			else
			{
				cur->next = node;
				node->prev = cur;
				cur = node;
			}
		}
	}

	Node *node = res;
	while (node)
	{
		if (node->type == CHILD)
		{
			char buf[80];
			strcpy(buf, node->item_name);
			if ((node->next == NULL) || (node->next->type != CHILD))
			{
				sprintf(node->item_name, " |__[#] %s", buf);
			}
			else
			{
				sprintf(node->item_name, " |--[#] %s", buf);
			}
		}
		node = node->next;
	}
	return res;
}


void free_node(Node *node)
{
	Node *next;
	while (node)
	{
		next = node->next;
		free(node);
		node = next;
	}
}

int get_group_index(Node *node)
{
	int index = 0;
	while (node && node->type == CHILD)
	{
		node = node->prev;
	}

	while (node)
	{
		if (node->type != CHILD || node->is_expand == 1)
		{
			index ++;
		}
		node = node->prev;
	}
	return index > 0? index -1: 0;
}

Node *get_node_by_index(int index, Node *head)
{
	Node *res = NULL;

	while (head)
	{
		if (head->type == GROUP || head->type == NODE || (head->type == CHILD && head->is_expand))
		{
			if (index == 0)
			{
				res = head;
				break;
			}
			index --;
		}
		head = head->next;
	}

	return res;
}

void expand_node(Node *head, int status)
{
	while(head && (head->type == CHILD))
	{
		head = head->prev;
	}

	if (head != NULL)
	{
		Node *child = head->next;
		head->is_expand = status;
		while (child)
		{
			if (child->type == CHILD)
			{
				child->is_expand = status;
			}
			else
			{
				break;
			}
			child = child->next;
		}
	}
}

ITEM **get_item_list(Node *head)
{
	ITEM **item_list = NULL;
	size_t node_count = 0;
	size_t i;
	Node *node = head;

	while(node)
	{
		if (node->type == GROUP || node->type == NODE || (node->type == CHILD && node->is_expand))
		{
			node_count ++;
		}
		node = node->next;
	}

	item_list = (ITEM **)calloc(node_count + 1, sizeof(ITEM **));
	node = head;
	i = 0;
	while(node)
	{
		if (node->type == GROUP || node->type == NODE || (node->type == CHILD && node->is_expand))
		{
			if (node->type == GROUP)
			{
				node->item_name[1] = (char)(node->is_expand? '-': '+');
				printw(node->item_name);
			}
			item_list[i] = new_item(node->item_name, node->ip);
			i ++;
		}
		node = node->next;
	}
	item_list[i] = NULL;

	return item_list;
}

