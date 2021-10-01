/*
Image visualisation area's structure.
*/

#include "../include/area.h"

/**
 * Return the sub-area from the specified area at the given direction.
 * 
 * \param area the area to divide.
 * \param direction the direction of the sub-area to return.
 * \return the sub-area of the specified area.
 */ 
Area get_sub_area(Area area, Direction direction)
{
    int x = area.x + (direction == NORTH_EAST || direction == SOUTH_EAST) * area.width / 2;
    int y = area.y + (direction == SOUTH_WEST || direction == SOUTH_EAST) * area.height / 2;
    int width = area.width / 2;
    int height = area.height / 2;

    return (Area){x, y, width, height};
}

/**
 * Return 1 if this area contains the specified coordinate.
 * \param area the area to be cheched.
 * \param x the 'x' of the coordinate.
 * \param y the 'y' of the coordinate.
 * \return 1 if the area contains the specified coordinate.
 */ 
int area_contains(Area area, int x, int y) {
    return !(
        x < area.x || x > area.width + area.x ||
        y < area.y || y > area.width + area.y
    );
}
