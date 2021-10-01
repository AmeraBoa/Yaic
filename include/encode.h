#ifndef __ENCODE
#define __ENCODE

#include "../include/quadtree.h"

typedef enum {
    BIT,
    COLOR
} color_format;

void enc_save_to_qtn(Quadtree tree, const char* filename);
void enc_save_to_qtc(Quadtree tree, const char* filename);
Quadtree enc_load_qtn(const char* filename);
Quadtree enc_load_qtc(const char* filename);

void enc_save_to_gmn(Quadtree, const char* filename);
void enc_save_to_gmc(Quadtree, const char* filename);
Quadtree enc_load_gmn(const char* filename);
Quadtree enc_load_gmc(const char* filename);

int enc_save(Quadtree tree, const char* filename);
Quadtree enc_load(const char* filename);

#endif