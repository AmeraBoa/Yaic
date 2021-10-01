/*
Images are represented by a two dimension array of color.
*/

#include "../include/bitmap.h"

/**
 * Convert the specified img to a bitmap.
 * \param img the image to be converted.
 * \param bitmap the bitmap to receive the conversion.
 */
void convert_img_to_bitmap(MLV_Image *img, Color bitmap[512][512])
{
    if(img == NULL) {
        printf("Image does not exist or is not recognized\n");
        exit(EXIT_FAILURE);
    }

    size_t i, j;
    int red, green, blue, alpha;
    for (i = 0; i < 512; i++)
    {
        for (j = 0; j < 512; j++)
        {
            MLV_get_pixel_on_image(img, i, j, &red, &green, &blue, &alpha);
            bitmap[i][j] = MLV_convert_rgba_to_color(red, green, blue, alpha);
        }
    }
}

/**
 * Evaluate the average color from a specified bitmap in a given area.
 * \param bitmap the bitmap from which to determine the average color.
 * \param area the area in the bitmap to determine the average color.
 * \return the average color of the bitmap in a specified area.
 */
Color bitmap_average_color(Color bitmap[512][512], Area area)
{
    size_t i, j, k;
    int rgba_sum[4] = {0, 0, 0, 0};

    for (i = area.x; i < area.width + area.x; i++)
    {
        for (j = area.y; j < area.height + area.y; j++)
        {
            rgba_sum[RED] += red(bitmap[i][j]);
            rgba_sum[GREEN] += green(bitmap[i][j]);
            rgba_sum[BLUE] += blue(bitmap[i][j]);
            rgba_sum[ALPHA] += alpha(bitmap[i][j]);
        }
    }

    for (k = 0; k < 4; k++)
    {
        rgba_sum[k] /= area.width * area.height;
    }

    Color average_color = convert_rgba_to_color(rgba_sum);

    return average_color;
}

/**
 * Return the error value from a bitmap in a specified area. The sum of 
 * the color distance between the average color of the image and the pixel of the image.
 * \param bitmap the bitmap to evaluate the error from.
 * \param area the area in the bitmap to test.
 * \return the error valurn of the bitmap in a specified area.
 */
long error_value(Color bitmap[512][512], Area area) {
    long res = 0;
    Color p;

    p = bitmap_average_color(bitmap, area);

    int i, j;
    for (i = 0; i < area.width; i++)
    {
        for (j = 0; j < area.height; j++)
        {
            res += color_distance(bitmap[i + area.x][j + area.y], p);
        }
    }
    
    return res;
}
