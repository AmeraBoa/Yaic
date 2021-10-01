/*
Set of functions to write down a quadtree into a dot file.
*/
#include "../include/diagram.h"


static void write_header(FILE *f);
static void write_tree(FILE *f , Quadtree a);
static void write_end(FILE *f);
static void create_file(FILE *f, Quadtree a);
static void create_dot(Quadtree head);

void write_header(FILE *f) {
    fprintf(f, "digraph arbre {\n");
    fprintf(f, "node [shape=circle , height=.1 ]\n");
    fprintf(f, "edge [arrowhead=none,arrowtail=dot];\n");
}

void write_tree(FILE *f , Quadtree a) {
    if(a->visited) return;
    a->visited = 1;

    if(a != NULL) fprintf(f, "\"n%o\" [label=\"\"][style=filled,fillcolor=\"#%08x\"]\n", a, a->color);
    
    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        if(a->nodes[i] != NULL) {
            fprintf(f, "\"n%o\":value:c -> \"n%o\":value\n", a, a->nodes[i]);
            write_tree(f, a->nodes[i]);
        }
    }
}

void write_end(FILE *f) {
    fprintf(f, "}");
}

void create_file(FILE *f, Quadtree a) {
    write_header(f);
    write_tree(f, a);
    write_end(f);
}

void create_dot(Quadtree head) {
    qt_reset_visited_nodes(head);
    FILE *f = fopen("visualise.dot", "w");
    create_file(f, head);
    fclose(f);
}

/**
 * Show the diagram of the nodes from the specified quadtree.
 * \param head the quadtree form which to make the diagram.
 */
void show_diagram(Quadtree head) {
    create_dot(head);
    system("dot -Tpdf visualise.dot -o visualise.pdf");
    system("rm -f visualise.dot");
    system("mupdf visualise.pdf");
}