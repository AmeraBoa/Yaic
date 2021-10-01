/**
 * Encoding of quadtrees.
 */ 

#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "../include/bit_buffer.h"
#include "../include/encode.h"
#include "../include/draw.h"

#define LEAF 1
#define NODE 0

/* Quadtree. */
static void save_to_qt(Quadtree tree, const char* filename, color_format color_format);
static Quadtree load_qt(const char* filename, color_format color_format);

static void add_qt_to_bit_buffer(BitBuffer *b_buffer, Quadtree tree, color_format color_format);
static Quadtree create_quadtree_from_qt(BitBuffer *b_buffer, color_format color_format);

/* Minimized graph. */
static void add_gm_to_file(Quadtree tree, FILE *file, color_format color_format);
static Quadtree create_quadtree_from_gm(FILE* file, size_t nb_node, color_format color_format);

static void save_to_gm(Quadtree tree, const char* filename, color_format color_format);
static Quadtree load_gm(const char* filename, color_format color_format);

static size_t count_lines(const char* filename);

/**
 * Save a quadtree to the specified filename with qtn format.
 * \param tree the quadtree to be saved.
 * \param filename the filename of the saved quadtree.
 */
void enc_save_to_qtn(Quadtree tree, const char* filename) {
    save_to_qt(tree, filename, BIT);
}

/**
 * Save a quadtree to the specified filename with qtc format.
 * \param tree the quadtree to be saved.
 * \param filename the filename of the saved quadtree.
 */
void enc_save_to_qtc(Quadtree tree, const char* filename) {
    save_to_qt(tree, filename, COLOR);
}

void save_to_qt(Quadtree tree, const char* filename, color_format color_format) {
    BitBuffer bit_buffer;
    size_t leaves, internal_nodes;

    qt_get_infos(tree, &leaves, &internal_nodes);

    size_t size;
    if(color_format == BIT)
        size = leaves/4 + internal_nodes/8; /* BIT: 2 bits for a leaf, 1 for an internal node. */
    else        
        size = 4*leaves + leaves/8 + internal_nodes/8; /* COLOR:(4*8 + 1) bits for a leaf, 1 for an internal node. */

    bbuf_init(&bit_buffer, size);
    add_qt_to_bit_buffer(&bit_buffer, tree, color_format);

    FILE *dest = fopen(filename, "w");
    if(dest == NULL) {
        printf("Couldn't save quadtree to qt\n");
        exit(EXIT_FAILURE);
    }
    bbuf_put(dest, &bit_buffer);

    bbuf_clear(bit_buffer);
    fclose(dest);
}

/* Adding bits to a buffer based on a specified quadtree and format. */
void add_qt_to_bit_buffer(BitBuffer *b_buffer, Quadtree tree, color_format color_format) {
    if(tree == NULL) return;

    if(qt_is_leaf(tree)) {
        bbuf_add(b_buffer, LEAF);
        if(color_format == BIT)
            bbuf_add(b_buffer, convert_to_bit_color(tree->color));
        else
            bbuf_add_color(b_buffer, tree->color);
    }
    else {
        bbuf_add(b_buffer, NODE);
    }

    size_t i;
    for (i = 0; i < QT_MAX_NODE; i++)
    {
        add_qt_to_bit_buffer(b_buffer, tree->nodes[i], color_format);
    }
}

/**
 * Load a quadtree from a specified file with the qtn format.
 * \param filename the filename containing the quadtree.
 * \return the loaded quadtree.
 */
Quadtree enc_load_qtn(const char* filename) {
    return load_qt(filename, BIT);
}

/**
 * Load a quadtree from a specified file with the qtc format.
 * \param filename the filename containing the quadtree.
 * \return the loaded quadtree.
 */
Quadtree enc_load_qtc(const char* filename) {
    return load_qt(filename, COLOR);
}

Quadtree load_qt(const char* filename, color_format color_format) {
    Quadtree tree = NULL;
    FILE *src = fopen(filename, "r");
    if(src == NULL) {
        printf("Couldn't read qtc file\n");
        return tree;
    }

    BitBuffer bit_buffer;
    bbuf_open(&bit_buffer, src);

    tree = create_quadtree_from_qt(&bit_buffer, color_format);

    bbuf_clear(bit_buffer);
    fclose(src);
    return tree;
}

/* Creating a quadtree from a qt file. */
Quadtree create_quadtree_from_qt(BitBuffer *b_buffer, color_format color_format) {
    int bit = bbuf_read(b_buffer);
    Quadtree tree = NULL;

    tree = qt_create_node(MLV_COLOR_GREY);

    if(bit == NODE) {
        size_t i;
        for (i = 0; i < QT_MAX_NODE; i++)
        {
            tree->nodes[i] = create_quadtree_from_qt(b_buffer, color_format);
        }
    }
    else {
        if(color_format == BIT)
            tree->color = bbuf_read(b_buffer) ? MLV_COLOR_WHITE : MLV_COLOR_BLACK;
        else
            tree->color = bbuf_read_color(b_buffer);
    }

    return tree;
}

/**
 * Save a quadtree to the specified filename with gmn format.
 * \param tree the quadtree to be saved.
 * \param filename the filename of the saved quadtree.
 */
void enc_save_to_gmn(Quadtree tree, const char* filename) {
    save_to_gm(tree, filename, BIT);
}

/**
 * Save a quadtree to the specified filename with gmc format.
 * \param tree the quadtree to be saved.
 * \param filename the filename of the saved quadtree.
 */
void enc_save_to_gmc(Quadtree tree, const char* filename) {
    save_to_gm(tree, filename, COLOR);
}

void save_to_gm(Quadtree tree, const char* filename, color_format color_format) {
    FILE *dest = fopen(filename, "w");
    if(dest == NULL) {
        printf("Couldn't save quadtree to gmc\n");
        exit(EXIT_FAILURE);
    }
    qt_reset_visited_nodes(tree);
    qt_set_id(tree);
    qt_reset_visited_nodes(tree);
    add_gm_to_file(tree, dest, color_format);
    fclose(dest);
}

/* Adding character to a file based on the specified quadtree and color format. */
void add_gm_to_file(Quadtree tree, FILE *file, color_format color_format) {
    if(tree->visited) return;
    tree->visited = 1;

    if(!qt_is_leaf(tree)) {
        fprintf(file, "%ld %ld %ld %ld %ld\n", tree->id,
        tree->nodes[0]->id, tree->nodes[1]->id, tree->nodes[2]->id, tree->nodes[3]->id);

        size_t i;
        for (i = 0; i < QT_MAX_NODE; i++)
        {
            add_gm_to_file(tree->nodes[i], file, color_format);
        }
    } else {
        if(color_format == BIT) {
            fprintf(file, "%ld %d\n", tree->id, convert_to_bit_color(tree->color));
        } else{
            fprintf(file, "%ldf %d %d %d %d\n", tree->id, 
            red(tree->color), 
            green(tree->color), 
            blue(tree->color), 
            alpha(tree->color));
        }
    }
}

/**
 * Load a quadtree from a specified file with the gmn format.
 * \param filename the filename containing the quadtree.
 * \return the loaded quadtree.
 */
Quadtree enc_load_gmn(const char* filename) {
    return load_gm(filename, BIT);
}

/**
 * Load a quadtree from a specified file with the gmc format.
 * \param filename the filename containing the quadtree.
 * \return the loaded quadtree.
 */
Quadtree enc_load_gmc(const char* filename) {
    return load_gm(filename, COLOR);
}

Quadtree load_gm(const char* filename, color_format color_format) {
    Quadtree tree = NULL;
    FILE *src = fopen(filename, "r");
    if(src == NULL) {
        printf("Couldn't read gm file\n");
        return tree;
    }
    size_t nb_node = count_lines(filename);
    tree = create_quadtree_from_gm(src, nb_node, color_format);
    fclose(src);
    return tree;
}

size_t count_lines(const char* filename) {
    char c;
    size_t lines;
    FILE* file = fopen(filename, "r");
    lines = 0;
    while((c = fgetc(file)) != EOF) {
        if(c == '\n') lines++;
    }
    fclose(file);
    return lines;
}

void parse_value_gmc(Quadtree *nodes, char* line) {
    int value[5];
    size_t nb_value, j;
    
    char * token = strtok(line, " ");
    for(nb_value = 0; token != NULL; nb_value++) {
        value[nb_value] = atoi(token);
        token = strtok(NULL, " ");
    }

    int id = value[0];
    /* Tagging used nodes. */
    nodes[id]->visited = 1;
    
    if(strchr(line, 'f') != NULL) {
        nodes[id]->color = MLV_convert_rgba_to_color(value[1], value[2], value[3], value[4]);
    } else {
        for (j = 0; j < QT_MAX_NODE; j++)
        {
            int node_id = value[j + 1];
            nodes[id]->nodes[j] = nodes[node_id];
        }
    }
}

void parse_value_gmn(Quadtree *nodes, char* line) {
    int value[5];
    size_t nb_value;

    char * token = strtok(line, " ");
    for(nb_value = 0; token != NULL; nb_value++) {
        value[nb_value] = atoi(token);
        token = strtok(NULL, " ");
    }

    int id = value[0];
    /* Tagging used nodes. */
    nodes[id]->visited = 1;
    size_t j;

    if(nb_value == 5) {
        for (j = 0; j < QT_MAX_NODE; j++)
        {
            int node_id = value[j + 1];
            nodes[id]->nodes[j] = nodes[node_id];
        }
    } else {
        nodes[id]->color = value[1] ? MLV_COLOR_WHITE : MLV_COLOR_BLACK;
    }
}

/* Creating a quadtree from a minimized graph file file. */
Quadtree create_quadtree_from_gm(FILE* file, size_t nb_node, color_format color_format) {
    /* When minimizing some nodes are freed. Thus the identification number is 
    not linear. We need a bigger buffer for indexing quadtree. */
    size_t size = nb_node;
    Quadtree nodes[size];
    Quadtree root = NULL;

    /* Reading line variables. */
    char * line = NULL;
    size_t len = 0;
    
    size_t i;
    for (i = 0; i < size; i++)
    {
        nodes[i] = qt_create_node(0);
    }
    root = nodes[0];

    /* Reading the line, then split it and parse it into integer. */
    while(getline(&line, &len, file) != EOF) {
        if(color_format == BIT)
            parse_value_gmn(nodes, line);
        else
            parse_value_gmc(nodes, line);
    }
    free(line);

    /* Removing unused nodes. */
    for (i = 0; i < size; i++)
    {
        if(!nodes[i]->visited)
            free(nodes[i]);
    }

    return root; 
}

/**
 * Load a quadtree from a specified file. The format of the file determine 
 * how it should be loaded. Return NULL if the file extension is not valid.
 * \param filename the filename containing the quadtree.
 * \return the loaded quadtree.
 */
Quadtree enc_load(const char* filename) {
    Quadtree tree = NULL;

    char* ext = strchr(filename, '.') + 1;
    if(ext == NULL + 1) return tree;

    if(strcmp(ext, "qtn") == 0) {
        tree = enc_load_qtn(filename);
    }
    else if(strcmp(ext, "qtc") == 0) {
        tree = enc_load_qtc(filename);
    }
    else if(strcmp(ext, "gmn") == 0) {
        tree = enc_load_gmn(filename);
    }
    else if(strcmp(ext, "gmc") == 0) {
        tree = enc_load_gmc(filename);
    }

    return tree;
}

/**
 * Save a quadtree to the specified filename. Check the file
 * extension and returns 0 if it is invalid.
 * \param tree the quadtree to be saved.
 * \param filename the filename of the saved quadtree.
 * \return 0 if the file extension is invalid.
 */
int enc_save(Quadtree tree, const char* filename) {
    char* ext = strchr(filename, '.') + 1;
    if(ext == NULL + 1) {
        printf("invalid filename\n");
        return 0;
    }

    if(strcmp(ext, "qtn") == 0) {
        enc_save_to_qtn(tree, filename);
    }
    else if(strcmp(ext, "qtc") == 0) {
        enc_save_to_qtc(tree, filename);
    }
    else if(strcmp(ext, "gmn") == 0) {
        enc_save_to_gmn(tree, filename);
    }
    else if(strcmp(ext, "gmc") == 0) {
        enc_save_to_gmc(tree, filename);
    }

    return 1;
}