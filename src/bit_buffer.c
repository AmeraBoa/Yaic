#include "../include/bit_buffer.h"


/**
 * Initialize the specified bit buffer with a given size.
 * \param b_buffer the bit buffer to be initialized.
 * \param size the size of the bit buffer.
 */
void bbuf_init(BitBuffer *b_buffer, int size) {
    b_buffer->size = size;
    b_buffer->buffer = malloc(b_buffer->size);
    b_buffer->bit_pos = 0;

    size_t i;
    for (i = 0; i < b_buffer->size; i++)
    {
        b_buffer->buffer[i] = 0;
    }
}

/**
 * Add a bit to the specified bit buffer.
 * \param b_buffer the bit buffer to be added.
 * \param bit the bit to add.
 */
void bbuf_add(BitBuffer *b_buffer, int bit) {
    if(b_buffer->bit_pos / 8 >= b_buffer->size) {
        b_buffer->size *= 1.5;
        b_buffer->buffer = realloc(b_buffer->buffer, b_buffer->size);
    }

    int byte = b_buffer->bit_pos / 8;
    int bit_pos = b_buffer->bit_pos % 8;

    b_buffer->buffer[byte] |= bit << (7 - (bit_pos));
    b_buffer->bit_pos++;
}

/**
 * Clear the specified bit buffer.
 * \param b_buffer the bit buffer to be cleared.
 */
void bbuf_clear(BitBuffer b_buffer) {
    free(b_buffer.buffer);
}

void print_bit(int octet) {
    int i;
    for (i = 7; i >= 0; i--)
    {
        printf("%c", octet & (1 << i) ? '1' : '0');
    }
    printf("\n");
}

/**
 * Add a color to the specified bit buffer.
 * \param b_buffer the bit buffer to be added.
 * \param color the color to add.
 */
void bbuf_add_color(BitBuffer *b_buffer, uint32_t color) {
    bbuf_add_char(b_buffer, red(color));
    bbuf_add_char(b_buffer, green(color));
    bbuf_add_char(b_buffer, blue(color));
    bbuf_add_char(b_buffer, alpha(color));
}

/**
 * Add a character to the specified bit buffer.
 * \param b_buffer the bit buffer to be added.
 * \param char the character to add.
 */
void bbuf_add_char(BitBuffer *b_buffer, char c) {
    int i;
    for (i = 8 * sizeof(c) - 1; i >= 0; i--)
    {
        bbuf_add(b_buffer, c & 1 << i ? 1 : 0);
    }
}

/**
 *  Put the specified buffer to the destination file.
 * \param dest the destination file.
 * \param b_buffer the bit buffer to put.
 */
void bbuf_put(FILE *dest, BitBuffer *b_buffer) {
    while(b_buffer->bit_pos % 8 != 0) {
        bbuf_add(b_buffer, 0);
    }

    size_t i;
    for (i = 0; i < b_buffer->bit_pos / 8; i++)
    {
        fprintf(dest, "%c", b_buffer->buffer[i]);
    }
}

/**
 *  Read and add it to the specified bit buffer. 
 * \param b_buffer the bit to be added.
 * \param src the source file from which to read.
 */
void bbuf_open(BitBuffer *b_buffer, FILE *src) {
    char c;
    size_t size, i;

    fseek(src, 0L, SEEK_END);
    size = ftell(src);
    rewind(src);

    b_buffer->buffer = malloc(size);
    b_buffer->size = size;
    b_buffer->bit_pos = 0;
    for (i = 0; i < size; i++)
    {
        c = fgetc(src);
        b_buffer->buffer[i] = c;
    }
}

/**
 * Return the bit at the current bit position.
 * \param b_buffer the bit buffer from which a bit can be read.
 */
int bbuf_read(BitBuffer *b_buffer) {
    int byte = b_buffer->bit_pos / 8;
    int bit_pos = b_buffer->bit_pos % 8;
    int bit;
    
    bit = b_buffer->buffer[byte] & 1 << (7 - (bit_pos)) ? 1 : 0;
    b_buffer->bit_pos++;

    return bit;
}

/**
 * Return the color at the current bit position.
 * \param b_buffer the bit buffer from which a color can be read.
 */
uint32_t bbuf_read_color(BitBuffer *b_buffer) {
    uint32_t color;

    int i;
    for (i = sizeof(uint32_t) * 8 - 1, color = 0; i >= 0; i--)
    {
        color |= bbuf_read(b_buffer) << i;
    }
    return color;
}