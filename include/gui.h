#ifndef __GUI
#define __GUI

#include "quadtree.h"

#include <MLV/MLV_all.h>

/**
 * Graphical interface of the program.
*/

extern const char * gui_icons[];

extern const char * gui_hover_infos[];

/**
 * A button containing a box collision, an image and a 
 * message displayed when hovering with the mouse.
*/
typedef struct {
    Area box;
    MLV_Image *img;
    char* hover_info;
} Button;

typedef enum {
    OPEN,
    SAVE,
    TREE,
    MIN,
    STYLE,
    QUIT
} tool_bar_button;

void gui_start();

#endif