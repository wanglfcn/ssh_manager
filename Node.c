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
		strcpy(res->item_name, item_name);
		strcpy(res->user_name, user_name);
		strcpy(res->password, password);
	}
	return res;
}

Node *gen_node(char *config)
{
	Node *res = NULL;
	char *para[5];
	int index = 0;
	memset(para, 0, 5*sizeof(char *));

	while (*config && index < 5)
	{
		if (*config == '\t' || *config == '\n')
		{
			*config = 0;
			index ++;
		}
		else if (para[index] == NULL && *config != 0)
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
		type = ITEM;
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
			fgets(buf, 1024, fp);
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
	return res;
}
