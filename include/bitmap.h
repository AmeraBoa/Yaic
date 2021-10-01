#ifndef GRAPH
#define GRAPH

#include <stdint.h>
#include <MLV/MLV_all.h>
#include "area.h"
#include "color.h"

#define IMG_SIZE 512

void convert_img_to_bitmap(MLV_Image *img, Color bitmap[IMG_SIZE][IMG_SIZE]);
Color bitmap_average_color(Color bitmap[IMG_SIZE][IMG_SIZE], Area area);
long error_value(Color bitmap[IMG_SIZE][IMG_SIZE], Area area);

#endif