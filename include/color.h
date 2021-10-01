#ifndef __COLOR
#define __COLOR

#include <stdint.h>
#include "area.h"

typedef uint32_t Color;
typedef uint8_t Channel;

typedef enum
{
    RED,
    GREEN,
    BLUE,
    ALPHA
} ChannelType;

Channel red(Color Color);
Channel green(Color Color);
Channel blue(Color Color);
Channel alpha(Color Color);
int convert_to_bit_color(Color color);
Color convert_rgba_to_color(int rgba[4]);
double color_distance(Color p1, Color p2);

#endif