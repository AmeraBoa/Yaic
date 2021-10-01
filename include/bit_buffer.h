/**
 * A Bit buffer, storing bits one by one in an array of char.
 */ 

#ifndef __BIT_BUFFER
#define __BIT_BUFFER

#include "../include/quadtree.h"

typedef struct {
    size_t size;
    size_t bit_pos;
    char *buffer;
} BitBuffer;

void bbuf_init(BitBuffer *b_buffer, int size);
void bbuf_clear(BitBuffer b_buffer);

void bbuf_add(BitBuffer *b_buffer, int bit);
void bbuf_add_char(BitBuffer *b_buffer, char type);
void bbuf_add_color(BitBuffer *b_buffer, uint32_t color);

void bbuf_put(FILE *dest, BitBuffer *b_buffer);
void print_bit(int octet);

void bbuf_open(BitBuffer *b_buffer, FILE *src);
int bbuf_read(BitBuffer *b_buffer);
uint32_t bbuf_read_color(BitBuffer *b_buffer);

#endif