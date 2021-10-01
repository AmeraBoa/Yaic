#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../include/quadtree.h"
#include "../include/tree_linked_list.h"

#define ERROR_RATE 0

static int max_in_array(int *values, size_t size);
static Quadtree _construct_quadtree(Color bitmap[IMG_SIZE][IMG_SIZE], Area area);
static void collect_distinct_nodes(Quadtree tree, TreeLinkedList *tree_buffer);

/***
 * Create a new quadtree node with the specified value as color.
 * \param value the value of the created quadtree.
 * \return the allocated quadtree.
 */
Quadtree qt_create_node(Color value)
{
    Quadtree quadtree = malloc(sizeof(Node));
    if (quadtree == NULL)
    {
        printf("Error malloc Quadtree\n");
        exit(EXIT_FAILURE);
    }

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        quadtree->nodes[i] = NULL;
    }
    quadtree->color = value;
    quadtree->visited = 0;

    return quadtree;
}

/**
 * Create a quadtree from a specified image.
 * \param img the image from which to construct the quadtree.
 * \return the quadtree generated from the image. 
 */
Quadtree qt_create_quadtree(MLV_Image *img) {
    Color bitmap[IMG_SIZE][IMG_SIZE];
    convert_img_to_bitmap(img, bitmap);
    return _construct_quadtree(bitmap, (Area) {0, 0, IMG_SIZE, IMG_SIZE});
}

Quadtree _construct_quadtree(Color bitmap[IMG_SIZE][IMG_SIZE], Area area)
{
    Quadtree quadtree = qt_create_node(bitmap_average_color(bitmap, area));

    if (error_value(bitmap, area) <= ERROR_RATE)
        return quadtree;

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        quadtree->nodes[i] = _construct_quadtree(bitmap, get_sub_area(area, i));
    }    

    return quadtree;
}

/**
 * Free an allocated quadtree.
 * \param quadtree the quadtree to be freed. 
 */
void qt_free(Quadtree quadtree)
{
    if (quadtree != NULL)
    {
        size_t i;
        for (i = 0; i < QT_MAX_NODE; i++)
        {
            qt_free(quadtree->nodes[i]);
        }
        free(quadtree);
    }
}

/**
 * Count the number of node in a quadtree.
 * \param tree the tree to be counted.
 * \return the number of node in the quadtree.
 */
int qt_count_node(Quadtree tree) {
    if(tree == NULL || tree->visited) return 0;
    tree->visited = 1;

    size_t i, sum;
    for (i = 0, sum = 0; i < QT_MAX_NODE; i++)
    {
        sum += qt_count_node(tree->nodes[i]);
    }
    return sum + 1;
}

/**
 * Return the height in a specified quadtree.
 * \param quadtree the quadtree to be evaluated.
 * \return the height of the quadtree.
 */
int qt_height(Quadtree quadtree)
{
    if (quadtree == NULL)
        return -1;
    else
    {
        int heights[QT_MAX_NODE];
        size_t i;
        for (i = 0; i < QT_MAX_NODE; i++)
        {
            heights[i] = qt_height(quadtree->nodes[i]);
        }

        return max_in_array(heights, QT_MAX_NODE) + 1;
    }
}

int max_in_array(int *values, size_t size)
{
    size_t i, max;
    for (i = 0, max = values[0]; i < size; i++)
    {
        if (max < values[i])
            max = values[i];
    }
    return max;
}

/**
 * Evaluate the distance between two quadtrees. If the two quadtrees are
 * leaves, we check the color distance between them. Otherwise, descend recursively until we find a leaf.
 * 
 * \param a the quadtree to compare.
 * \param b the quadtree to compare.
 * \return the distance between the two quadtrees.
 */
double qt_distance(Quadtree a, Quadtree b) {
    if(a == NULL || b == NULL) return 0;

    int i;
    double sum;

    sum = 0;
    if(qt_is_leaf(a) && qt_is_leaf(b)){
        return color_distance(a->color, b->color);
    }
    for(i = 0; i < QT_MAX_NODE; i++){
        if (qt_is_leaf(a) && !qt_is_leaf(b)){
            sum += qt_distance(a, b->nodes[i]);
        }
        else if (!qt_is_leaf(a) && qt_is_leaf(b)){
            sum += qt_distance(b, a->nodes[i]);
        }
        else{
            sum += qt_distance(a->nodes[i], b->nodes[i]);
        }
    }
    return sum / 4;
}

/**
 * Return 1 if the specified quadtree is a leaf.
 * \param tree the quadtree to be tested.
 * \return 1 if the tree is a leaf.
 */
int qt_is_leaf(Quadtree tree) {
    return tree->nodes[0] == NULL;
}

/**
 * Reset the visited field in the quadtree and all his children, setting it to 0.
 * \param tree the tree to be modified.
 */
void qt_reset_visited_nodes(Quadtree tree) {
    if(tree == NULL) return;

    tree->visited = 0;

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        qt_reset_visited_nodes(tree->nodes[i]);
    }
}

/**
 * Count the number of leaf in a specified quadtree.
 * \param tree the tree to be counted.
 * \return the number of leaves in the quadtree.
 */
int qt_count_leaf(Quadtree tree) {
    if(tree == NULL || tree->visited) return 0;

    if(qt_is_leaf(tree) && !tree->visited) {
        tree->visited = 1;
        return 1;
    }

    size_t i, sum;
    for (i = 0, sum = 0; i < QT_MAX_NODE; i++)
    {
        sum += qt_count_leaf(tree->nodes[i]);
    }
    return sum;
}

/**
 * Return informations about the specified quadtree.
 * \param tree the tree to get informations from.
 * \param leaves the pointer which will receive the number of leaves.
 * \param internal_nodes the pointer wich will receive the number of internal nodes.
 */ 
void qt_get_infos(Quadtree tree, size_t *leaves, size_t *internal_nodes) {
    qt_reset_visited_nodes(tree);
    *leaves = qt_count_leaf(tree);
    qt_reset_visited_nodes(tree);
    *internal_nodes = qt_count_node(tree) - *leaves;
}

/**
 * Free a minimized quadtree. Prevent double free as two branches can have the same leaf.
 * \param tree the minimized tree to be freed.
 */
void qt_free_minimized(Quadtree tree) {
    qt_reset_visited_nodes(tree);
    
    TreeLinkedList tree_buffer = NULL;
    collect_distinct_nodes(tree, &tree_buffer);
    tll_free_with_value(tree_buffer);
}

void collect_distinct_nodes(Quadtree tree, TreeLinkedList *tree_buffer)
{
    if (tree == NULL) return;

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        collect_distinct_nodes(tree->nodes[i], tree_buffer);
    }

    if(!tree->visited) {
        tll_add(tree_buffer, tree);
        tree->visited = 1;
    }
}

/**
 * Set an identification number on each nodes of a specified quadtree.
 * \param tree the tree to be modified.
 */
void qt_set_id(Quadtree tree) {
    static int id = 0;
    if(tree == NULL || tree->visited) return;
    tree->visited = 1;

    tree->id = id;
    id++;

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        qt_set_id(tree->nodes[i]);
    }
    
}

/* Reset internal node average color. */
void qt_reset_color(Quadtree tree) {
    if(qt_is_leaf(tree)) return;

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        qt_reset_color(tree->nodes[i]);
    }

    long sum[4] = {0, 0, 0, 0};
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        Color color = tree->nodes[i]->color;
        sum[RED] += red(color);
        sum[GREEN] += green(color);
        sum[BLUE] += blue(color);
        sum[ALPHA] += alpha(color);
    }

    for (i = 0; i < 4; i++)
    {
        sum[i] /= QT_MAX_NODE;
    }
    tree->color = MLV_convert_rgba_to_color(sum[RED], sum[GREEN], sum[BLUE], sum[ALPHA]);
}