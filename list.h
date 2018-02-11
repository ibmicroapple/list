/* File: list.h, C single header source file
 * Description: Thread-safe Doubly-linked list
 * Language: ANSI C
 * License: MIT
 * Code conventions:
 * - pointers start with 'p_'
 * - Types start with uppercase character
 * - Functions and variables are lower case
 * - no camel case
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#define LIST_NULL (List){0}
#define LIST_OK 0
#define LIST_ERR -1

typedef struct _List_node
{
	struct _List_node	*prev, /* Pointer to the previous node */
				*next; /* Pointer to the next node */
	void			*obj; /* Pointer to the object */
} List_node;

typedef struct
{
	List_node	*first, /* Pointer to the first node */
			*last, /* Pointer to the last node */
			*current; /* Pointer to the current node */
	size_t		size; /* Number of nodes within list */
} List;

List *list_new(void);
void list_free(List *p_list);
int list_add_last(List *p_list, void *p_obj);
int list_add_first(List *p_list, void *p_obj);
int list_del_last(List *p_list);
int list_del_first(List *p_list);
int list_insert(List *p_list, void *p_obj);
int list_insert_at(List *p_list, size_t pos, void *p_obj);
int list_remove(List *p_list);
int list_remove_at(List *p_list, size_t pos);
int list_remove_node(List *p_list, List_node *p_node);
int list_remove_obj(List *p_list, void *p_obj);
int list_replace(List *p_list, void *p_obj);
int list_replace_at(List *p_list, size_t pos, void *p_obj);
int list_reverse(List *p_list);
void list_clear(List *p_list);
int list_next(List *p_list);
int list_prev(List *p_list);
int list_goto(List *p_list, size_t pos);

#define list_isempty(p_list) (!((List*)p_list)->size)
#define list_iterate(p_list, p_node) for(p_node = ((List_node*)((List*)(p_list))->first); p_node; p_node = p_node->next)
#define list_riterate(p_list, p_node) for(p_node = ((List_node*)((List*)(p_list))->last); p_node; p_node = p_node->prev)

static List_node *list_create_node(void *p_obj);
static void list_delete_node(List_node *p_node);
List_node *list_get_node(List *p_list, size_t pos);

static List_node *list_create_node(void *p_obj)
{
	List_node *new_node;

	new_node = malloc(sizeof(List_node));
	if(!new_node) return(0);

	new_node->prev = new_node->next = 0;
	new_node->obj = p_obj;

	return(new_node);
}

static void list_delete_node(List_node *p_node)
{
	p_node->prev = p_node->next = 0;
	p_node->obj = 0;
	free(p_node);
	p_node = 0;
}

List_node *list_get_node(List *p_list, size_t pos)
{
	List_node *p_node;
	size_t it;

	if(list_isempty(p_list)) return(0);

	if(pos <= (p_list->size / 2))
	{
		for(p_node = p_list->first, it = pos; it; p_node = p_node->next, it--){}
	}
	else
	{
		for(p_node = p_list->last, it = p_list->size - pos; it; p_node = p_node->prev, it--){}
	}

	return(p_node);
}

List *list_new(void)
{
	List *new_list;

	new_list = malloc(sizeof(List));
	if(!new_list) return(0);

	*new_list = LIST_NULL;

	return(new_list);
}

void list_free(List *p_list)
{
	list_clear(p_list);
	free(p_list);
}

int list_add_last(List *p_list, void *p_obj)
{
	List_node *new_node;

	new_node = list_create_node(p_obj);
	if(!new_node) return(LIST_ERR);

	if(list_isempty(p_list))
	{
		p_list->first = new_node;
		p_list->last = new_node;
		p_list->current = new_node;
	}
	else
	{
		p_list->last->next = new_node;
		new_node->prev = p_list->last;
		p_list->last = new_node;
	}

	p_list->size++;

	return(LIST_OK);
}

int list_add_first(List *p_list, void *p_obj)
{
	List_node *new_node;

	new_node = list_create_node(p_obj);
	if(!new_node) return(LIST_ERR);

	if(list_isempty(p_list))
	{
		p_list->first = new_node;
		p_list->last = new_node;
		p_list->current = new_node;
	}
	else
	{
		p_list->first->prev = new_node;
		new_node->next = p_list->first;
		p_list->first = new_node;
	}

	p_list->size++;

	return(LIST_OK);
}

int list_del_last(List *p_list)
{
	List_node *new_last;

	if(list_isempty(p_list)) return(LIST_ERR);

	if(p_list->size == 1)
	{
		list_delete_node(p_list->last);
		p_list->last = p_list->first = 0;
		p_list->current = 0;
	}
	else
	{
		if(p_list->current == p_list->last)
		{
			list_prev(p_list);
		}

		new_last = p_list->last->prev;
		list_delete_node(p_list->last);
		p_list->last = new_last;
		p_list->last->next = 0;
	}

	p_list->size--;

	return(LIST_OK);
}

int list_del_first(List *p_list)
{
	List_node *new_first;

	if(list_isempty(p_list)) return(LIST_ERR);

	if(p_list->size == 1)
	{
		list_delete_node(p_list->first);
		p_list->first = p_list->last = 0;
		p_list->current = 0;
	}
	else
	{
		if(p_list->current == p_list->first)
		{
			list_next(p_list);
		}

		new_first = p_list->first->next;
		list_delete_node(p_list->first);
		p_list->first = new_first;
		p_list->first->prev = 0;
	}

	p_list->size--;

	return(LIST_OK);
}

int list_insert(List *p_list, void *p_obj)
{
	List_node *new_node;

	if(!p_list->size) return(list_add_last(p_list, p_obj));

	if(p_list->current == p_list->first)
	{
		return(list_add_first(p_list, p_obj));
	}
	else if(p_list->current == p_list->last)
	{
		return(list_add_last(p_list, p_obj));
	}

	new_node = list_create_node(p_obj);
	if(!new_node) return(LIST_ERR);

	p_list->current->prev->next = new_node;
	new_node->prev = p_list->current->prev;
	p_list->current->prev = new_node;
	new_node->next = p_list->current;

	p_list->size++;

	return(LIST_OK);
}

int list_insert_at(List *p_list, size_t pos, void *p_obj)
{
	List_node *new_node, *temp_node;

	if(!pos || pos > (p_list->size + 1)) return(LIST_ERR);

	if(pos == 1)
	{
		return(list_add_first(p_list, p_obj));
	}
	else if(pos == (p_list->size + 1))
	{
		return(list_add_last(p_list, p_obj));
	}

	new_node = list_create_node(p_obj);
	if(!new_node) return(LIST_ERR);

	temp_node = list_get_node(p_list, pos);

	temp_node->prev->next = new_node;
	new_node->prev = temp_node->prev;
	temp_node->prev = new_node;
	new_node->next = temp_node;

	p_list->size++;

	return(LIST_OK);
}

int list_remove(List *p_list)
{
	List_node *new_current;

	if(p_list->current == p_list->first)
	{
		return(list_del_first(p_list));
	}
	else if(p_list->current == p_list->last)
	{
		return(list_del_last(p_list));
	}

	new_current = p_list->current->next;
	p_list->current->next->prev = p_list->current->prev;
	p_list->current->prev->next = p_list->current->next;

	list_delete_node(p_list->current);

	p_list->current = new_current;

	p_list->size--;

	return(LIST_OK);
}

int list_remove_at(List *p_list, size_t pos)
{
	List_node *temp_node;

	if(!pos || pos > p_list->size) return(LIST_ERR);

	if(pos == 1)
	{
		return(list_del_first(p_list));
	}
	else if(pos == p_list->size)
	{
		return(list_del_last(p_list));
	}

	temp_node = list_get_node(p_list, pos);

	if(temp_node == p_list->current)
	{
		list_next(p_list);
	}

	temp_node->next->prev = temp_node->prev;
	temp_node->prev->next = temp_node->next;

	list_delete_node(temp_node);

	p_list->size--;

	return(LIST_OK);
}

int list_remove_node(List *p_list, List_node *p_node)
{
	List_node *nodeptr;

	if(p_node == p_list->first)
	{
		return(list_del_first(p_list));
	}
	else if(p_node == p_list->last)
	{
		return(list_del_last(p_list));
	}
	else if(p_node == p_list->current)
	{
		return(list_remove(p_list));
	}

	list_iterate(p_list, nodeptr)
	{
		if(nodeptr == p_node)
		{
			nodeptr->next->prev = nodeptr->prev;
			nodeptr->prev->next = nodeptr->next;

			list_delete_node(nodeptr);

			p_list->size--;

			break;
		}
	}

	return(LIST_OK);
}

int list_remove_obj(List *p_list, void *p_obj)
{
	List_node *temp_node;

	list_iterate(p_list, temp_node)
	{
		if(temp_node->obj == p_obj)
		{
			if(temp_node == p_list->first)
			{
				return(list_del_first(p_list));
			}
			else if(temp_node == p_list->last)
			{
				return(list_del_last(p_list));
			}

			if(temp_node == p_list->current)
			{
				list_next(p_list);
			}

			temp_node->next->prev = temp_node->prev;
			temp_node->prev->next = temp_node->next;

			list_delete_node(temp_node);

			p_list->size--;

			return(LIST_OK);
		}
	}

	return(LIST_ERR);
}

int list_replace(List *p_list, void *p_obj)
{
	p_list->current->obj = p_obj;

	return(LIST_OK);
}

int list_replace_at(List *p_list, size_t pos, void *p_obj)
{
	List_node *temp_node;

	if(!pos || pos > p_list->size) return(LIST_ERR);

	if(pos == 1)
	{
		p_list->first->obj = p_obj;
		return(LIST_OK);
	}
	else if(pos == p_list->size)
	{
		p_list->last->obj = p_obj;
		return(LIST_OK);
	}

	temp_node = list_get_node(p_list, pos);

	temp_node->obj = p_obj;

	return(LIST_OK);
}

int list_reverse(List *p_list)
{
	List new_list = LIST_NULL;

	while(p_list->size)
	{
		list_add_last(&new_list, p_list->last->obj);

		if(p_list->last == p_list->current)
		{
			new_list.current = new_list.last;
		}

		list_del_last(p_list);
	}

	*p_list = new_list;

	return(LIST_OK);
}

void list_clear(List *p_list)
{
	while(p_list->size) list_del_last(p_list);
}

int list_next(List *p_list)
{
	if(p_list->current->next)
	{
		p_list->current = p_list->current->next;
		return(LIST_OK);
	}

	return(LIST_ERR);
}

int list_prev(List *p_list)
{
	if(p_list->current->prev)
	{
		p_list->current = p_list->current->prev;
		return(LIST_OK);
	}

	return(LIST_ERR);
}

int list_goto(List *p_list, size_t pos)
{
	if(pos > p_list->size) return(LIST_ERR);;

	p_list->current = list_get_node(p_list, pos);

	return(LIST_OK);
}

#endif /* LIST_H */