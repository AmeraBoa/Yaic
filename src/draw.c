#include "../include/draw.h"

static void draw_by_level(int x, int y, Quadtree root, int level, Area area, draw_style style);
static void draw_node_with_box(int x, int y, Node node, Area area);
static void _draw_quadtree_image(int x, int y, Quadtree tree, Area area, draw_style style);
static void draw_node(int x, int y, Node node, Area area, draw_style style);

/**
 * Display the quadtree at the specified coordinate step by step; descending by power of 2.
 * \param x the 'x' in the coordinate.
 * \param y the 'y' in the coordinate.
 * \param root the quadtree to be drawn.
 * \param style the drawing style of the quadtree.
 * \param delay the delay between each step.
 */
void draw_quadtree_process(int x, int y, Quadtree root, draw_style style, int delay)
{
    int h = qt_height(root) + 1;
    size_t i;
    for (i = 1; i <= h; i++)
    {
        draw_by_level(x, y, root, i, (Area) {0, 0, IMG_SIZE, IMG_SIZE}, style);
        MLV_actualise_window();
        MLV_wait_milliseconds(delay);
    }
    MLV_actualise_window();
    MLV_wait_milliseconds(250);
}

void draw_by_level(int x, int y, Quadtree root, int level, Area area, draw_style style)
{
    if (root == NULL)
        return;
    if (level == 1)
        draw_node(x, y, *root, area, style);
    else if (level > 1)
    {
        size_t i;
        for (i = 0; i < QT_MAX_NODE; i++)
        {
            draw_by_level(x, y, root->nodes[i], level - 1, get_sub_area(area, i), style);
        }
    }
}

void draw_node_with_circle(int x, int y, Node node, Area area) {
    MLV_draw_filled_rectangle(area.x + x, area.y + y, area.width, area.height, MLV_COLOR_BLACK);
    MLV_draw_filled_circle(area.x + area.width/2 + x, area.y + area.height/2 + y, area.width/2, node.color); 
}

void draw_node_with_box(int x, int y, Node node, Area area)
{
    MLV_draw_filled_rectangle(area.x + x, area.y + y, area.width, area.height, node.color);
    MLV_draw_rectangle(area.x + x, area.y + y, area.width, area.height, 0x0000001f);
}

void draw_node(int x, int y, Node node, Area area, draw_style style) {
    switch (style)
    {
    case STANDARD:
        MLV_draw_filled_rectangle(area.x + x, area.y + y, area.width, area.height, node.color);
        break;
    case CIRCLE:
        draw_node_with_circle(x, y, node, area);
        break;
    case BOX:
        draw_node_with_box(x, y, node, area);
        break;
    default:
        break;
    }
}

/**
 * Draw a quadtree image at the specified coordinate.
 * \param x the 'x' in the coordinate.
 * \param y the 'y' in the coordinate.
 * \param tree the quadtree to be displayed.
 * \param style the drawing style of the quadtree.
 */
void draw_quadtree_image(int x, int y, Quadtree tree, draw_style style)
{
    _draw_quadtree_image(x, y, tree, (Area) {0, 0, IMG_SIZE, IMG_SIZE}, style);
}

void _draw_quadtree_image(int x, int y, Quadtree tree, Area area, draw_style style) {
    if(tree == NULL) return;

    if(qt_is_leaf(tree)) {
        draw_node(x, y, *tree, area, style);
    }

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        _draw_quadtree_image(x, y, tree->nodes[i], get_sub_area(area, i), style);
    }
}