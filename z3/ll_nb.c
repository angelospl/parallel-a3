#include <stdio.h>
#include <stdlib.h> /* rand() */
#include <limits.h>

#include "../common/alloc.h"
#include "ll.h"

typedef struct ll_node {
	int key;
	struct ll_node *next;
	int flag;
	/* other fields here? */
} ll_node_t;

typedef struct window {
	ll_node_t *pred,*curr;
} window;


struct linked_list {
	ll_node_t *head;
	/* other fields here? */
};

/**
 * Create a new linked list node.
 **/
static ll_node_t *ll_node_new(int key)
{
	ll_node_t *ret;

	XMALLOC(ret, 1);
	ret->key = key;
	ret->next = NULL;
	ret->flag = 0;
	/* Other initializations here? */

	return ret;
}

/**
 * Free a linked list node.
 **/
static void ll_node_free(ll_node_t *ll_node)
{
	XFREE(ll_node);
}

/**
 * Create a new empty linked list.
 **/
ll_t *ll_new()
{
	ll_t *ret;

	XMALLOC(ret, 1);
	ret->head = ll_node_new(-1);
	ret->head->next = ll_node_new(INT_MAX);
	ret->head->next->next = NULL;

	return ret;
}

/**
 * Free a linked list and all its contained nodes.
 **/
void ll_free(ll_t *ll)
{
	ll_node_t *next, *curr = ll->head;
	while (curr) {
		next = curr->next;
		ll_node_free(curr);
		curr = next;
	}
	XFREE(ll);
}

ll_node_t* get(ll_node_t * node,int* marked)
{
	if (node!=NULL) {
	__sync_lock_test_and_set(marked,node->flag);
	}
	return node;
}

int compare_and_set(ll_node_t** curr,ll_node_t* exp_key,ll_node_t* upd_key,int exp_mark,int upd_mark)
{
	int comp;
	comp=__sync_bool_compare_and_swap(curr,exp_key,upd_key);
	if (comp) {
		if (__sync_bool_compare_and_swap(&((*curr)->flag),exp_mark,upd_mark)){
			return 1;
		}
		else {
			__sync_bool_compare_and_swap(&((*curr)->key),upd_key,exp_key);
			return 0;
		}
	}
	return 0;
}

int attempt_mark(ll_node_t* curr,int exp_key,int new_mark)
{
	if (__sync_bool_compare_and_swap(&curr->key,exp_key,exp_key)){
		return __sync_lock_test_and_set(&curr->flag,new_mark);
	}
	return 0;
}

window* find (ll_node_t* head,int key)
{
	ll_node_t *pred,*curr,*succ;
	int *marked;	//marked = false
	marked=(int*)malloc(sizeof(int));
	*marked=0;
	int snip;
	window *ret;
	retry: while (1) {
		pred = head;
		curr = pred->next;
		while (1) {
			succ = get(curr->next,marked);
			while (marked[0]) {
				snip = compare_and_set(&(pred->next),curr,succ,0,0);
				if (!snip) goto retry;
				curr = succ;
				succ = get(curr->next,marked);
			}
			if (curr->key >= key) {
				ret  = malloc(sizeof(window));
				ret->pred = pred;
				ret->curr = curr;
				return ret;
			}
			pred = curr;
			curr = succ;
		}
	}
}

int ll_contains(ll_t *ll, int key)
{
	return 0;
}

int ll_add(ll_t *ll, int key)
{
	int splice;
	ll_node_t *pred,*curr,*new_node;
	while (1) {
		window* win = find(ll->head,key);
		pred = win->pred;
		curr = win->curr;
		if (curr->key == key) {
			return 0;
		}
		else {
			new_node=ll_node_new(key);
			new_node->next=curr;
			if (compare_and_set(&pred->next,curr,new_node,0,0)) return 1;
		}
	}
}

int ll_remove(ll_t *ll, int key)
{
	int snip;
	while (1) {
		window *win = find(ll->head,key);
		ll_node_t *pred,*curr,*succ;
		pred = win->pred;
		curr = win->curr;
		if (curr->key != key) {
			free(win);
			return 0;
		}
		else {
			succ = curr->next;
			snip = compare_and_set(&(curr->next),succ,succ,0,1);
			if (!snip) continue;
			compare_and_set(&(pred->next),curr,succ,0,0);
			free(win);
			return 1;
		}
	}
}

/**
 * Print a linked list.
 **/
void ll_print(ll_t *ll)
{
	ll_node_t *curr = ll->head;
	printf("LIST [");
	while (curr) {
		if (curr->key == INT_MAX)
			printf(" -> MAX");
		else
			printf(" -> %d", curr->key);
		curr = curr->next;
	}
	printf(" ]\n");
}
