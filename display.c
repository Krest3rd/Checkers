// --------------GENERAL--------------
void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

// ------------DISPLAY----------------
#define BUFFER_W BOARD_WIDTH + BOARD_X
#define BUFFER_H BOARD_HEIGHT + BOARD_Y

#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
    // al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    // al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}


//-----------Drawing---------------

void draw_board(MAN board[BOARD_SIZE][BOARD_SIZE],field selected, char message[]) {
    for (int row = 0; row < BOARD_SIZE;row++) {
        for (int col = (row + 1) % 2; col < BOARD_SIZE;col += 2) {
            int x = (col) * BOARD_SQUARE_SIZE;
            // printf("%d ",x);
            int y = row * BOARD_SQUARE_SIZE;
            y = y + BOARD_Y; // Add the offset for the board
            x = x + BOARD_X; // Add the offset for the board
            al_draw_filled_rectangle(x, y, x + BOARD_SQUARE_SIZE, y + BOARD_SQUARE_SIZE, al_map_rgb_f(0, 0, 0));
            if (board[row][col].color == WHITE) {
                al_draw_filled_circle(x + 40, y + 40, 35, al_map_rgb(200, 200, 200));
            }
            if (board[row][col].color == BLACK) {
                al_draw_filled_circle(x + 40, y + 40, 35, al_map_rgb(55, 55, 55));
            }
            //Drawing crowns for the kings
            if (board[row][col].color == WHITE && board[row][col].isKing == true) {
                al_draw_filled_triangle(x + 10, y + 50, x + 25, y + 20, x + 40, y + 50, al_map_rgb(55, 55, 55));
                al_draw_filled_triangle(x + 25, y + 50, x + 40, y + 20, x + 55, y + 50, al_map_rgb(55, 55, 55));
                al_draw_filled_triangle(x + 40, y + 50, x + 55, y + 20, x + 70, y + 50, al_map_rgb(55, 55, 55));
                al_draw_filled_triangle(x + 10, y + 50, x + 24, y + 50, x + 24, y + 20, al_map_rgb(200, 200, 200));
                al_draw_filled_triangle(x + 70, y + 50, x + 56, y + 50, x + 56, y + 20, al_map_rgb(200, 200, 200));
            }
            if (board[row][col].color == BLACK && board[row][col].isKing == true) {
                al_draw_filled_triangle(x + 10, y + 50, x + 25, y + 20, x + 40, y + 50, al_map_rgb(200, 200, 200));
                al_draw_filled_triangle(x + 25, y + 50, x + 40, y + 20, x + 55, y + 50, al_map_rgb(200, 200, 200));
                al_draw_filled_triangle(x + 40, y + 50, x + 55, y + 20, x + 70, y + 50, al_map_rgb(200, 200, 200));
                al_draw_filled_triangle(x + 10, y + 50, x + 24, y + 50, x + 24, y + 20, al_map_rgb(55, 55, 55));
                al_draw_filled_triangle(x + 70, y + 50, x + 56, y + 50, x + 56, y + 20, al_map_rgb(55, 55, 55));
            }

        }
        // printf("Selected field: %d %d\n", selected.col, selected.row);
        if (selected.col != -1 && selected.row != -1) {
            // printf("x1: %d, y1: %d, x2: %d, y2: %d\n", (selected.col-BOARD_X) * BOARD_SQUARE_SIZE, (selected.row-BOARD_Y) * BOARD_SQUARE_SIZE, (selected.col-BOARD_X) * BOARD_SQUARE_SIZE + BOARD_SQUARE_SIZE, (selected.row-BOARD_Y) * BOARD_SQUARE_SIZE + BOARD_SQUARE_SIZE);
            // Draw a rectangle around the selected field
            al_draw_rectangle((selected.col) * BOARD_SQUARE_SIZE + BOARD_X, (selected.row) * BOARD_SQUARE_SIZE+BOARD_Y, (selected.col) * BOARD_SQUARE_SIZE + BOARD_SQUARE_SIZE + BOARD_X, (selected.row) * BOARD_SQUARE_SIZE + BOARD_SQUARE_SIZE+BOARD_Y, al_map_rgb(255, 0, 0), 5);
        }
        // printf("\n");
    };
}