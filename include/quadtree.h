#ifndef __QUADTREE
#define __QUADTREE

#include <stdint.h>
#include "area.h"
#include "color.h"
#include "bitmap.h"

#define QT_MAX_NODE 4

/**
 * A structure to represent quadtree.
 */
typedef struct node
{
    struct node *nodes[QT_MAX_NODE];
    Color color;

    /* Identification number for DAG encoding. */
    size_t id;

    /* Allow for DAG iteration. */
    char visited:1;
    
} * Quadtree, Node;

Quadtree qt_create_node(Color value);
Quadtree qt_create_quadtree(MLV_Image *img);
void qt_free(Quadtree quadtree);
void qt_free_minimized(Quadtree tree);
int qt_height(Quadtree quadtree);
double qt_distance(Quadtree a, Quadtree b);
int qt_is_leaf(Quadtree tree);
int qt_count_node(Quadtree tree);
int qt_count_leaf(Quadtree tree);
void qt_reset_visited_nodes(Quadtree tree);
void qt_get_infos(Quadtree tree, size_t *leaves, size_t *internal_nodes);
void qt_set_id(Quadtree tree);
void qt_reset_color(Quadtree tree);

#endif