#ifndef AREA
#define AREA

/**
 * Represents an area of the image.
 */
typedef struct
{
    int x;
    int y;
    int width;
    int height;
} Area;

/**
 * Direction of the sub-area. Define position for the subdivided region of
 * an Area.
 */
typedef enum direction
{
    NORTH_WEST,
    NORTH_EAST,
    SOUTH_EAST,
    SOUTH_WEST
} Direction;

Area get_sub_area(Area area, Direction direction);
int area_contains(Area area, int x, int y);

#endif