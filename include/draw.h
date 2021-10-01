#ifndef __DRAW
#define __DRAW

/**
 * Module related to quadtrees drawing. 
 */

#include "quadtree.h"

/* Drawing style, used when displaying quadtree. */
typedef enum {
    STANDARD,
    CIRCLE,
    BOX
} draw_style;

void draw_quadtree_process(int x, int y, Quadtree root, draw_style style, int delay);
void draw_quadtree_image(int x, int y, Quadtree tree, draw_style style);

#endif