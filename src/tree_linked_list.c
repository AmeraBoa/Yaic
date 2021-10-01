/**
 * Linked list of quadtree. Used as buffer for freeing minimized quadtree.
 */ 
#include <stdlib.h>
#include "../include/tree_linked_list.h"

TreeLinkedList create_node(Quadtree tree) {
    TreeLinkedList node = malloc(sizeof(Node));
    if (node == NULL)
    {
        printf("Error malloc LinkedList\n");
        exit(EXIT_FAILURE);
    }

    node->value = tree;
    node->next = NULL;

    return node;
}

/* Free the specified linked list and the containing Quadtree values. */
void tll_free_with_value(TreeLinkedList head) {
    TreeLinkedList tmp;

    while (head != NULL)
    {
       tmp = head;
       head = head->next;
       if(tmp->value != NULL) free(tmp->value);
       free(tmp);
    }
}

/**
 * Free the specified linked list.
 * \param head the linked list to be freed.
*/
void tll_free(TreeLinkedList head) {
    TreeLinkedList tmp;

    while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

/**
 * Add a specified quadtree to a linked list.
 * \param head_ref the pointer of the linked list to be added.
 * \param tree the quadtree to add.
*/
void tll_add(TreeLinkedList *head_ref, Quadtree tree) {
    TreeLinkedList tc = create_node(tree);
    tc->next = *head_ref;
    *head_ref = tc;
}

/**
 * Return 1 if the specified quadtree is in the linked list.
 * \param head the linked list from which to check.
 * \param tree the quadtree to be checked.
 * \return 1 if the linked list contains the quadtree.
*/
int tll_contains(TreeLinkedList head, Quadtree tree) {
    while (head != NULL)
    {
        if(tree == head->value) return 1;
        head = head->next;
    }

    return 0;
}

/**
 * Return the quadtree in a linked list with the specified color
 * \param head the linked list to search from.
 * \param value the color to identify the quadtree.
 * \return the found quadtree, or NULL if does not exist in the linked list.
*/
Quadtree tll_find_with_color(TreeLinkedList head, int value) {
    while (head != NULL)
    {
        if(head->value->color == value) return head->value;
        head = head->next;
    }

    return NULL;
}

/**
 * Return the quadtree in a linked list with the specified distance to a quadtree.
 * \param head the linked list to search from.
 * \param tree the other quadtree to compare with.
 * \param distance the distance to identify the quadtree.
 * \return the found quadtree, or NULL if does not exist in the linked list.
*/
Quadtree tll_find_with_distance(TreeLinkedList head, Quadtree tree, double distance) {
    if(tree == NULL) return NULL;

    while (head != NULL)
    {
        if(qt_distance(head->value, tree) <= distance) {
            return head->value;
        }
            
        head = head->next;
    }

    return NULL;
}