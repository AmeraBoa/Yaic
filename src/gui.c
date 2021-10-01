#include "../include/gui.h"
#include "../include/encode.h"
#include "../include/draw.h"
#include "../include/minimize.h"

#include <string.h> 

#define TOOL_BAR_HEIGHT 40

#define STATUS_BAR_HEIGHT 25
#define STATUS_BAR_COLOR 0x202229ff

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT IMG_SIZE + TOOL_BAR_HEIGHT + STATUS_BAR_HEIGHT

#define NB_BUTTON 6

#define BACKGROUND_COLOR MLV_COLOR_GRAY5

#define IMG_PADDING 20
#define HOVER_EFFECT_POS 2

#define TOOL_BAR_COLOR MLV_COLOR_GREY15

/* Allow on release button state. */
MLV_Button_state was_pressed = MLV_RELEASED;

/* Informations about the current image. */
char status_message[255];

const char * gui_icons[] = {
    "res/gui/open.png",
    "res/gui/save.png",
    "res/gui/tree.png",
    "res/gui/min.png",
    "res/gui/draw.png",
    "res/gui/quit.png"
};

const char * gui_hover_infos[] = {
    "open a new file",
    "save the current file",
    "initiate quadtree approximation",
    "reduce the current quadtree",
    "change drawing style",
    "close window"
};


static int gui_is_pressed(Button button);
static void gui_display_tool_bar(Button *toolbar);
static Button gui_create_button(int x, int y, int width, int height, MLV_Image *img, const char* message);
static void gui_free_button(Button button);
static void gui_display_button_on_default(Button button);
static char* gui_create_input_box();
static void gui_display_button_on_hover(Button button);

int gui_is_pressed(Button button) {
    int x, y;
    MLV_get_mouse_position(&x, &y);

    if(area_contains(button.box, x, y)) {
        gui_display_button_on_hover(button);
        return 
        MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_RELEASED && 
        was_pressed == MLV_PRESSED;
    }

    return 0;
}

void gui_display_button_on_hover(Button button) {
    MLV_draw_filled_rectangle(button.box.x, button.box.y, button.box.width, button.box.height, MLV_COLOR_GREY30);
    MLV_draw_adapted_text_box(button.box.x + 32, button.box.y + 35, button.hover_info, 3, 
    MLV_COLOR_GREY90, MLV_COLOR_GREY32, MLV_COLOR_GREY90, MLV_TEXT_LEFT);

    MLV_draw_image(button.img, button.box.x + IMG_PADDING/2, button.box.y + IMG_PADDING/2);
}

void gui_display_button_on_default(Button button) {
    MLV_draw_filled_rectangle(button.box.x, button.box.y, button.box.width, button.box.height, TOOL_BAR_COLOR);
    MLV_draw_image(button.img, button.box.x + IMG_PADDING/2, button.box.y + IMG_PADDING/2);
}

void gui_display_tool_bar(Button *toolbar) {
    MLV_draw_filled_rectangle(0, 0, WINDOW_WIDTH, TOOL_BAR_HEIGHT, TOOL_BAR_COLOR);

    size_t i;
    for (i = 0; i < NB_BUTTON; i++)
    {
        gui_display_button_on_default(toolbar[i]);
    }
    MLV_draw_filled_rectangle(0, 0, WINDOW_WIDTH, TOOL_BAR_HEIGHT, 0x0000004f);
}

void gui_display_status_bar(const char* message) {
    MLV_draw_text_box(
        0, WINDOW_HEIGHT - STATUS_BAR_HEIGHT, WINDOW_WIDTH, STATUS_BAR_HEIGHT,
        message, 10,
        STATUS_BAR_COLOR, MLV_COLOR_WHITE, STATUS_BAR_COLOR, 
        MLV_TEXT_LEFT, MLV_HORIZONTAL_LEFT, MLV_VERTICAL_CENTER
    );
}

Button gui_create_button(int x, int y, int width, int height, MLV_Image *img, const char* message) {
    Button button;
    button.box = (Area) {x, y, width, height};
    MLV_resize_image(img, button.box.width - IMG_PADDING, button.box.height - IMG_PADDING);
    button.img = img;
    button.hover_info = malloc(strlen(message));
    strcpy(button.hover_info, message);
    return button;
}

void gui_free_button(Button button) {
    MLV_free_image(button.img);
    if(button.hover_info != NULL)
        free(button.hover_info);
}

char* gui_create_input_box() {
    char* filename;
    MLV_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0x0000004f);

    MLV_Font *font = MLV_load_font("res/font/Open_Sans/OpenSans-Regular.ttf", 12);
    MLV_wait_input_box_with_font(
        WINDOW_WIDTH/2 - 300/2, 60, 300, TOOL_BAR_HEIGHT, 
        MLV_COLOR_GREY45, MLV_COLOR_WHITE, MLV_COLOR_GREY25, 
        " Filename : ", &filename, font
    );
    MLV_free_font(font);
    return filename;
}

void change_status_message(const char* message) {
    memset(status_message, '\0', 255);
    strcpy(status_message, message);
}

void open_button_action(Quadtree* tree, MLV_Image **img) {
    if(*tree != NULL) qt_free_minimized(*tree);
    *tree = NULL;
    *img = NULL;

    char* filename;
    filename = gui_create_input_box();
    *tree = enc_load(filename);

    if(*tree != NULL) {
        qt_reset_color(*tree);
        printf("tree loaded\n");
    } else {
        *img = MLV_load_image(filename);
        if(*img != NULL) MLV_resize_image(*img, IMG_SIZE, IMG_SIZE);
        printf("img loaded\n");
    }

    if(*img == NULL && *tree == NULL) {
        printf("the file does not exist or is not valid\n");
        change_status_message("no file selected");
    } else {
        change_status_message(filename);
    }
    free(filename);
}

void save_button_action(Quadtree* tree) {
    if(*tree == NULL) {
        printf("this image is not valid\n");
        return;
    }

    char* filename;
    filename = gui_create_input_box();
    if(enc_save(*tree, filename) == 0) {
        printf("invalid filename\n");
    }
    else {
        printf("file saved at %s\n", filename);
    }
    free(filename);
}

void tree_button_action(Quadtree *tree, MLV_Image **img) {
    if(*tree != NULL || *img == NULL) {
        printf("invalid state : image format does not allow conversion\n");
        return;
    }

    printf("beginning tree approximation...\n");
    *tree = qt_create_quadtree(*img);
    *img = NULL;
    printf("...done\n");
}

void min_button_action(Quadtree *tree) {
    if(*tree == NULL) {
        printf("invalid state : a quadtree image is required\n");
        return;
    }

    printf("minimizing...\n");
    minimize_loss(*tree, DISTANCE_RATE);
    printf("...done\n");
}

int img_selected(Quadtree tree, MLV_Image *img) {
    return img != NULL || tree != NULL;
}

/**
 * Start the main GUI frame.
 */
void gui_start() {
    MLV_create_window_with_default_font("QT_view", "", WINDOW_WIDTH, WINDOW_HEIGHT, 
    "res/font/Open_Sans/OpenSans-Regular.ttf", 11);
    Button toolbar[8];
    Quadtree current_tree = NULL;
    MLV_Image *current_img = NULL;
    draw_style style = STANDARD;

    change_status_message("no file selected");

    size_t i;
    for (i = 0; i < NB_BUTTON; i++)
    {
        toolbar[i] = gui_create_button(
            TOOL_BAR_HEIGHT * i, 0, TOOL_BAR_HEIGHT, TOOL_BAR_HEIGHT, 
            MLV_load_image(gui_icons[i]), gui_hover_infos[i]
        );
    }

    while(1) {
        MLV_actualise_window();
        MLV_clear_window(BACKGROUND_COLOR);
        
        if(current_img != NULL) {
            MLV_draw_image(current_img, WINDOW_WIDTH/2 - IMG_SIZE/2, TOOL_BAR_HEIGHT);
        } else if(current_tree != NULL) {
            draw_quadtree_image(WINDOW_WIDTH/2 - IMG_SIZE/2, TOOL_BAR_HEIGHT, current_tree, style);
        }        

        gui_display_tool_bar(toolbar);
        gui_display_status_bar(status_message);

        if(gui_is_pressed(toolbar[OPEN])) {
            open_button_action(&current_tree, &current_img);
        }
        if(gui_is_pressed(toolbar[SAVE])) {
            save_button_action(&current_tree);
        }
        if(gui_is_pressed(toolbar[TREE])) {
            tree_button_action(&current_tree, &current_img);
        }
        if(gui_is_pressed(toolbar[MIN])) {
            min_button_action(&current_tree);
        }
        if(gui_is_pressed(toolbar[STYLE])) {
            style = (style + 1) % 3;
            printf("current style : %s\n", style == 0 ? "STANDARD" : style == 1 ? "CIRCLE" : "BOX");
        }
        if(gui_is_pressed(toolbar[QUIT])) 
            break;
        was_pressed = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);
    }

    for (i = 0; i < NB_BUTTON; i++)
    {
        gui_free_button(toolbar[i]);
    }

    if(current_tree != NULL) 
        qt_free_minimized(current_tree);
    if(current_img != NULL)
        MLV_free_image(current_img);
    MLV_free_window();
}