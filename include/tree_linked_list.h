/**
 * Linked list of quadtree. Used as buffer for freeing minimized quadtree.
 */ 

#ifndef __TLINKEDLIST
#define __TLINKEDLIST

#include "quadtree.h"

typedef struct tll_node {
    struct tll_node *next;
    Quadtree value;
} *TreeLinkedList;

void tll_free_with_value(TreeLinkedList list);
void tll_free(TreeLinkedList head);
void tll_add(TreeLinkedList *head, Quadtree tree);
int tll_contains(TreeLinkedList head, Quadtree tree);
Quadtree tll_find_with_color(TreeLinkedList head, int value);
Quadtree tll_find_with_distance(TreeLinkedList head, Quadtree tree, double distance);

#endif