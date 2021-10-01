/*
Colors stored as 32 bits integer.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/color.h"

/**
 * Convert the specified rgba color to an 32 bits integer color.
 * \param rgba the rgba to be converted.
 * \return the converted rgba color.
 */
Color convert_rgba_to_color(int rgba[4])
{
    return (rgba[RED] << 24) + (rgba[GREEN] << 16) + (rgba[BLUE] << 8) + rgba[ALPHA];
}

/**
 * Convert the specified color to a bit color. Either 0 or 1 depending on the average color.
 * \param color the color to be converted.
 * \return the converted color.
 */
int convert_to_bit_color(Color color) {
    return 127 < (red(color) + green(color) + blue(color) + alpha(color)) / 4;
}

/**
 * Return the specified channel value in a color.
 * \param value the color to get the channel from.
 * \param channel the channel of the color.
 * \return the specified channel in the color.
 */
Channel get_channel_value(Color value, ChannelType channel)
{
    return (value >> (24 - channel * 8)) & 0xff;
}

/**
 * Return the red channel in the color.
 */
Channel red(Color value) {
    return get_channel_value(value, RED);
}

/**
 * Return the green channel in the color.
 */
Channel green(Color value) {
    return get_channel_value(value, GREEN);
}

/**
 * Return the blue channel in the color.
 */
Channel blue(Color value) {
    return get_channel_value(value, BLUE);
}

/**
 * Return the alpha channel in the color.
 */
Channel alpha(Color value) {
    return get_channel_value(value, ALPHA);
}

/**
 * Get the distance between two colors.
 * \param p1 'p1' in the distance formula.
 * \param p2 'p2' in the distance formula.
 * \return the distance between two colors.
 */
double color_distance(Color p1, Color p2) {
    return sqrt(
            pow(red(p1) - red(p2), 2) +
            pow(blue(p1) - blue(p2), 2) +
            pow(green(p1) - green(p2), 2) +
            pow(alpha(p1) - alpha(p2), 2)
        );
}
