#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
//File handling - used later on to check if file exists
#include <io.h>
#define F_OK 0
#define access _access
#pragma warning(disable:4996)

// --------------GENERAL--------------
void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


// bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
// {
//     if(ax1 > bx2) return false;
//     if(ax2 < bx1) return false;
//     if(ay1 > by2) return false;
//     if(ay2 < by1) return false;

//     return true;
// }

//int max(int a, int b) {
//    if (a > b) return a;
//    return b;
//}

// ------------DISPLAY----------------
#define BUFFER_W 640
#define BUFFER_H 640

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

//---------------CHECKERS----------

int initBoard(MAN Board[8][8])
{
    int turn;
    //First init
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Board[i][j].color = NONE;
        }
    }

    //Checks if file with game save exists
    if (access("gameSave.txt", F_OK) == 0) {
        FILE* gameSave = fopen("gameSave.txt", "r");
        int row, col, color, isKing;

        fscanf(gameSave, "%d", &turn);

        while (!feof(gameSave)) {
            fscanf(gameSave, "%d %d %d %d", &row, &col, &color, &isKing);
            Board[row][col].color = color;
            Board[row][col].isKing = isKing;
            printf("%d %d %d %d\n", row, col, color, isKing);
        }
        fclose(gameSave);

        return turn;
    }
    else {

        for (int i = 0;i < 3;i++) {
            for (int j = 0;j < 8;j += 2) {
                if (i == 1) {
                    Board[i][j].color = WHITE;
                    Board[i][j].isKing = false;

                    Board[7 - i][j + 1].color = BLACK;
                    Board[7 - i][j + 1].isKing = false;
                }
                else {
                    Board[i][j + 1].color = WHITE;
                    Board[i][j + 1].isKing = false;

                    Board[7 - i][j].color = BLACK;
                    Board[7 - i][j].isKing = false;
                }
            }
        }

        turn = BLACK;
        return turn;
    }
}

void switchTurn(int* turn) {
    *turn = -1 * (*turn);
}


// Checks if a move is valid
int isValid(MAN Board[8][8], int Player, int col1, int row1, int col2, int row2) {
    // Check if not OTB
    if (col1 < 0 || col2 < 0 || row1 < 0 || row2 < 0) {
        return false;
    }
    if (col1 > 7 || col2 > 7 || row1 > 7 || row2 > 7) {
        return false;
    }

    // Check if diffrence between both axes is equal and not grater than 2
    int xdiff = abs(col2 - col1);
    int ydiff = abs(row2 - row1);

    if (xdiff != ydiff || xdiff > 2) return false;

    // Check if theres a MAN on starting pos and no MAN on end pos
    if (Board[row1][col1].color != Player || Board[row2][col2].color != NONE) return false;

    if (Board[row1][col1].isKing) {
        //Takes down
        if (row2 == row1 + 2) {
            //Takes left
            if (col2 == col1 - 2) {
                if (Board[row1 + 1][col1 - 1].color == Player || Board[row1 + 1][col1 - 1].color == NONE) return false;
            }
            //Takes right
            if (col2 == col1 + 2) {
                if (Board[row1 + 1][col1 + 1].color == Player || Board[row1 + 1][col1 + 1].color == NONE) return false;
            }
        }
        // Takes up
        else if (row2 == row1 - 2) {
            //Takes left
            if (col2 == col1 - 2) {
                if (Board[row1 - 1][col1 - 1].color == Player || Board[row1 - 1][col1 - 1].color == NONE) return false;
            }
            //Takes right
            if (col2 == col1 + 2) {
                if (Board[row1 - 1][col1 + 1].color == Player || Board[row1 - 1][col1 + 1].color == NONE) return false;
            }
        }
    }
    else { // Normal MAN
        // Check if correct direction (BLACK ^ | WHITE ↓)
        if (row2 != row1 + Player || row2 != row1 + (2 * Player)) return false;

        // Check if takes
        if (row2 == row1 + (2 * Player)) {
            //Takes left
            if (col2 == col1 - 2) {
                if (Board[row1 + Player][col1 - 1].color == Player || Board[row1 + Player][col1 - 1].color == NONE) return false;
            }
            //Takes right
            if (col2 == col1 + 2) {
                if (Board[row1 + Player][col1 + 1].color == Player || Board[row1 + Player][col1 + 1].color == NONE) return false;
            }
        }
    }
    return true;
}

// Move a pawn (no captures)
int move(MAN Board[8][8], int Player, int col1, int row1, int col2, int row2) {

    // Check if move is valid
    if (isValid(Board, Player, col1, row1, col2, row2)) return false;

    // Move by 1
    if (abs(col2 - col1) != 1) return false;

    // Place on new position 
    Board[col2][row2].color = Player;
    if ((row2 == 0 && Player == BLACK) || (row2 == 7 && Player == WHITE)) Board[col2][row2].isKing = true;
    else Board[row2][col2].isKing = Board[row1][col1].isKing;

    // Remove from old position
    Board[row1][col1].color = NONE;
    Board[row1][col1].isKing = false;
    return 1;
}

// Capture a pawn
int capture(MAN Board[8][8], int Player, int col1, int row1, int col2, int row2) {
    int capped_row = (row2 + row1) / 2;
    int capped_col = (col2 + col1) / 2;

    // Check if move is valid
    if (isValid(Board, Player, col1, row1, col2, row2)) return 0;

    // Check if you move by 2
    if (abs(col2 - col1) != 2) return 0;

    //Perform capture
    Board[capped_row][capped_col].color = NONE;
    Board[capped_row][capped_row].isKing = false;

    Board[col2][row2].color = Player;
    if ((row2 == 0 && Player == BLACK) || (row2 == 7 && Player == WHITE)) Board[col2][row2].isKing = true;
    else Board[row2][col2].isKing = Board[row1][col1].isKing;

    // Remove from old position
    Board[row1][col1].color = NONE;
    Board[row1][col1].isKing = false;

    return 1;
}

field* double_captures(MAN Board[8][8], int Player, int col, int row) {
    field* final_fields;
    final_fields = (field*)malloc(sizeof(field) * 4);

    int i = 0;

    // Downright
    // Changed from .col and .row to .y and .x ~ Tomasz (Changed it back cos I can read this better))
    if (isValid(Board, Player, col, row, col + 2, row + 2)) {
        final_fields[i].row = col + 2;
        final_fields[i].col = row + 2;
        i++;
    }
    if (isValid(Board, Player, col, row, col - 2, row + 2)) {
        final_fields[i].row = col - 2;
        final_fields[i].col = row + 2;
        i++;
    }
    if (isValid(Board, Player, col, row, col - 2, row - 2)) {
        final_fields[i].row = col - 2;
        final_fields[i].col = row - 2;
        i++;
    }
    if (isValid(Board, Player, col, row, col + 2, row - 2)) {
        final_fields[i].row = col + 2;
        final_fields[i].col = row - 2;
        i++;
    }
    field* return_fields;
    return_fields = (field*)malloc(sizeof(field) * i);

    for (int j = 0; j < i; j++) {
        return_fields[j] = final_fields[j];
    }

    return return_fields;
}

//-----------Drawing---------------

void draw_board(MAN board[8][8]) {
    for (int row = 0; row < 8;row++) {
        for (int col = (row + 1) % 2; col < 8;col += 2) {
            int x = (col) * 80;
            // printf("%d ",x);
            int y = row * 80;
            al_draw_filled_rectangle(x, y, x + 80, y + 80, al_map_rgb_f(0, 0, 0));
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
        // printf("\n");
    };
}

void saveToFile(MAN board[8][8], int turn) {
    //Saving the state of the game to gameSave.txt file
    FILE* gameSave = fopen("gameSave.txt", "w");

    //First line tells which turn it is
    fprintf(gameSave, "%d\n", turn);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (abs(board[i][j].color) == 1) {
                //Format of the save file is {row} {column} {color} {isKing}
                fprintf(gameSave, "%d %d %d %d\n", i, j, board[i][j].color, board[i][j].isKing);
            }
        }
    }

    fclose(gameSave);
}

//----------------MAIN-------------
int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 200.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();

    must_init(al_install_mouse(), "mouse");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    MAN Board[8][8];
    int turn = initBoard(Board);

    saveToFile(Board, turn);

    al_start_timer(timer);

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            break;


        case ALLEGRO_EVENT_MOUSE_AXES:
            // cursour.x = event.mouse.x;
            // cursour.y = event.mouse.y;
            redraw = true;
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:

            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(255, 255, 255));
            saveToFile(Board, turn);
            draw_board(Board);
            // draw_cursour(cursour.x,cursour.y);
            disp_post_draw();
            redraw = false;
        }
    }

}