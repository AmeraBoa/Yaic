/*
Quadtree minimization using an hash table of Linked List of quadtrees.
*/

#include "../include/minimize.h"
#include "../include/tree_linked_list.h"

#include <stdlib.h>

/* The size of the hash table. */
#define SIZE_HTABLE 1000000

/* Define by how much the color should be grouped up when indexing the
hash table.
*/
#define GROUP_FACTOR 3000

static void minimize_with_hashtable(Quadtree tree, TreeLinkedList tree_hashtable[SIZE_HTABLE], double distance);

/**
 *  Minimize the specified quadtree with the distance value. The minimized quadtree must
 * be freed using qt_free_minimized.
 * \param tree the tree to be minimized.
 * \param distance the distance value to compare two quadtree.
 */
void minimize_loss(Quadtree tree, double distance) {
    TreeLinkedList tree_hashtable[SIZE_HTABLE];
    qt_reset_visited_nodes(tree);

    size_t i;
    for (i = 0; i < SIZE_HTABLE; i++)
    {
        tree_hashtable[i] = NULL;
    }
    
    minimize_with_hashtable(tree, tree_hashtable, distance);

    for (i = 0; i < SIZE_HTABLE; i++)
    {
        tll_free(tree_hashtable[i]);
    }
}

/* Return the index in the hashtable of the specified quadtree. */
int hashcode(Quadtree tree) {
    return (tree->color / GROUP_FACTOR) % SIZE_HTABLE;
}

/* Fill the hashtable with the specified quadtree nodes. */
void minimize_with_hashtable(Quadtree tree, TreeLinkedList tree_hashtable[SIZE_HTABLE], double distance) {
    if(tree == NULL || tree->visited) return;
    tree->visited = 1;

    Quadtree related_tree = NULL;
    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        if(qt_is_leaf(tree)) break;

        related_tree = 
        tll_find_with_distance(tree_hashtable[hashcode(tree->nodes[i])], tree->nodes[i], distance);

        if(related_tree != NULL) {
            qt_free(tree->nodes[i]);
            tree->nodes[i] = related_tree;
        }
        minimize_with_hashtable(tree->nodes[i], tree_hashtable, distance);
    }

    if(related_tree == NULL) 
        tll_add(tree_hashtable + hashcode(tree), tree);
}