#include "../include/quadtree.h"
#include "../include/diagram.h"
#include "../include/minimize.h"
#include "../include/encode.h"
#include "../include/draw.h"
#include "../include/gui.h"

#include <sys/select.h>
#include <time.h>

void test_minimize(char* filename) {
    MLV_create_window("", "", IMG_SIZE, IMG_SIZE);
    MLV_Image *img = MLV_load_image(filename);

    if(img == NULL) {
        printf("file is invalid or does not exist\n");
        exit(EXIT_FAILURE);
    }

    MLV_resize_image(img, IMG_SIZE, IMG_SIZE);
    Quadtree qt = qt_create_quadtree(img);

    draw_quadtree_image(0, 0, qt, STANDARD);
    MLV_actualise_window();

    qt_reset_visited_nodes(qt);
    printf("before : %d nodes\n", qt_count_node(qt));
    qt_reset_visited_nodes(qt);

    printf("minimizing...\n");
    minimize_loss(qt, DISTANCE_RATE);
    printf("done...\n"); 

    qt_reset_visited_nodes(qt);
    printf("after : %d nodes\n", qt_count_node(qt));

    draw_quadtree_image(0, 0, qt, STANDARD);
    MLV_actualise_window();
    MLV_wait_milliseconds(2000);

    qt_free_minimized(qt);
    MLV_free_window();
    MLV_free_image(img);
}

void measure_minimize(char* filename) {
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    test_minimize(filename);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("=====================\n");
    printf("elapsed time : %lf\n", cpu_time_used);
}

void test_save() {
    MLV_create_window("", "", IMG_SIZE, IMG_SIZE);
    MLV_Image* img = MLV_load_image("res/img/beach.jpg");
    Quadtree qt = qt_create_quadtree(img);

    enc_save(qt, "img/beach.qtc");

    qt_free_minimized(qt);
    MLV_free_image(img);
    MLV_free_window();
}

void test_load() {
    MLV_create_window("", "", IMG_SIZE, IMG_SIZE);
    Quadtree qt = enc_load_qtc("img/beach.qtc");

    draw_quadtree_image(0, 0, qt, STANDARD);
    MLV_actualise_window();
    MLV_wait_milliseconds(250);

    qt_free_minimized(qt);
    MLV_free_window();
}

void open_file(char* filename) {
    MLV_create_window("", "", IMG_SIZE, IMG_SIZE);
    Quadtree qt = enc_load(filename);

    draw_quadtree_image(0, 0, qt, STANDARD);
    MLV_actualise_window();
    while(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) != MLV_PRESSED);

    qt_free_minimized(qt);
    MLV_free_window();
}

int main(int argc, char *argv[])
{

    if(argc == 1) {
        gui_start();
        return 0;
    }

    size_t i;
    for (i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "--time-min") == 0) {
            if(i + 1 >= argc) {
                printf("invalid argument: a file must be specified\n");

            }
            else {
                i++;
                measure_minimize(argv[i]);
            }
        }
        if(strcmp(argv[i], "--test-load") == 0) {
            test_load();
        }
        if(strcmp(argv[i], "--test-save") == 0) {
            test_save();
        }
        if(strcmp(argv[i], "-o") == 0) {
            if(i + 1 >= argc) {
                printf("invalid argument: a file must be specified\n");

            }
            else {
                i++;
                open_file(argv[i]);
            }
        }
    }

    return 0;
}
