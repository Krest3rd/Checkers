#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "display.c"   
#include "checkers.c" 
//File handling - used later on to check if file exists (works on Windows)
//  #include <io.h> // This is Winndows specific so I can't test for now
//  #define F_OK 0
//  #define access _access
//  #pragma warning(disable:4996)



//----------------MAIN-------------
int main()
{
    must_init(al_init(), "allegro");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_FONT* font = al_create_builtin_font();

    disp_init();

    must_init(al_install_mouse(), "mouse");

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_init_font_addon(), "font addon");

    // al_register_event_source(queue, al_get_keyboard_event_source()); Uncomment if you want to use keyboard events
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    MAN Board[BOARD_SIZE][BOARD_SIZE];
    int turn = initBoard(Board);
    field selected;
    field after_capture;
    int canCapture = hasCapt(Board, turn);
    char message[64] = {0}; // Allocate buffer for message
    int Loser = 0; // Variable to store the winner
    selected.col = -1;
    selected.row = -1;
    after_capture.col = -1;
    after_capture.row = -1;
    saveToFile(Board, turn);

    al_start_timer(timer);

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;


        case ALLEGRO_EVENT_MOUSE_AXES:
            // cursour.x = event.mouse.x;
            // cursour.y = event.mouse.y;
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (Loser != 0) {
                deleteFile();
                turn = initBoard(Board);
                Loser = 0;
                canCapture = hasCapt(Board, turn);
                selected.col = -1;
                selected.row = -1;
                break;
            }
            if (event.mouse.button == 1) // Left mouse button
            {
                // printf("Selected field: %d %d\n", (event.mouse.x - BOARD_X) / BOARD_SQUARE_SIZE, (event.mouse.y - BOARD_Y) / BOARD_SQUARE_SIZE);
                // printf("Selected field: %d %d\n", (event.mouse.x - BOARD_X), (event.mouse.y - BOARD_Y));
                if (selected.col == -1 || selected.row == -1) {
                    // Select a field
                    selected.col = (event.mouse.x - BOARD_X) / BOARD_SQUARE_SIZE;
                    selected.row = (event.mouse.y - BOARD_Y)/ BOARD_SQUARE_SIZE;
                } else {
                    int col = (event.mouse.x - BOARD_X) / BOARD_SQUARE_SIZE;
                    int row = (event.mouse.y - BOARD_Y) / BOARD_SQUARE_SIZE;
                    int result = PerformMove(Board, turn, selected.col, selected.row, col, row);
                    if (result != false) {
                    // If result is true, it was a normal move
                    // If result is 2, it was a capture
                        if (result == 2) {
                            // Check if player has any captures left
                            if (captures(Board, turn,col, row)) {
                                // Player has captures left, so he can continue capturing
                                selected.col = col;
                                selected.row = row;
                                after_capture.col = col;
                                after_capture.row = row;
                            } else {
                                // Player has no captures left, so switch turn
                                Loser = switchTurn(Board,&turn);
                                canCapture = hasCapt(Board, turn);
                                selected.col = -1;
                                selected.row = -1;
                            }
                        } else {
                            // Normal move, switch turn
                            Loser = switchTurn(Board,&turn);
                            canCapture = hasCapt(Board, turn);
                            selected.col = -1;
                            selected.row = -1;
                        }
                    } else {
                        if (after_capture.col != -1 || after_capture.row != -1) {
                            printf("You can continue capturing!\n");
                            selected.col = after_capture.col;
                            selected.row = after_capture.row;
                        } else{
                            selected.col = (event.mouse.x - BOARD_X) / BOARD_SQUARE_SIZE;
                            selected.row = (event.mouse.y - BOARD_Y) / BOARD_SQUARE_SIZE;
                        }
                    }
                }
            }
            else if (event.mouse.button == 2) // Right mouse button
            {
                // Deselect the field
                selected.col = -1;
                selected.row = -1;
            }
            // printf("Mouse clicked at: %d %d\n", event.mouse.x, event.mouse.y);   
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            exit(0);

        if (redraw && al_is_event_queue_empty(queue))
        {   

            int msg_len = 0;
            switch(turn){
                case WHITE:
                    msg_len = sprintf(message, "Turn: WHITE");
                    break;
                case BLACK:
                    msg_len = sprintf(message, "Turn: BLACK");
                    break;
            }

            if (canCapture) {
                msg_len += sprintf(message + msg_len, " | You can capture!");
            }
            if (Loser != 0) {
                if (Loser == WHITE) {
                    msg_len = sprintf(message, "BLACK wins!");
                } else if (Loser == BLACK) {
                    msg_len = sprintf(message, "WHITE wins!");
                }
            }

            disp_pre_draw();
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_text(font, al_map_rgb(0, 0, 0), 10, 20, 0, message);
            draw_board(Board,selected,message);
            disp_post_draw();
            redraw = false;
        }
    }

}