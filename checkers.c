#include "files.c"

//---------------CHECKERS----------

int initBoard(MAN Board[BOARD_SIZE][BOARD_SIZE])
{
    int turn;
    //First init
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Board[i][j].color = NONE;
        }
    }

    //Checks if file with game save exists
    if (ReadBoard(Board, &turn) == true) {
        // If file exists, load the game state
        return turn;
    } else {
        // If file does not exist, initialize the new game board

        for (int i = 0;i < 3;i++) {
            for (int j = 0;j < BOARD_SIZE;j += 2) {
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


// Checks if a move is valid
int isValid(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player, int col1, int row1, int col2, int row2) {
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

    // Check if player is trying to move someone else's pawn
    if (Board[row1][col1].color != Player) return false;

    // Check if theres no MAN on end pos
    if (Board[row2][col2].color != NONE) return false;
    
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
        if (row2 != row1 + Player && row2 != row1 + (2 * Player)) return false;
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
int move(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player, int col1, int row1, int col2, int row2) {
    // Check if move is valid
    if (isValid(Board, Player, col1, row1, col2, row2) == 0) return false;

    // Move by 1
    if (abs(col2 - col1) != 1) return false;

    // Place on new position 
    Board[row2][col2].color = Player;
    if ((row2 == 0 && Player == BLACK) || (row2 == 7 && Player == WHITE)) Board[row2][col2].isKing = true;
    else Board[row2][col2].isKing = Board[row1][col1].isKing;

    // Remove from old position
    Board[row1][col1].color = NONE;
    Board[row1][col1].isKing = false;
    return 1;
}

// Capture a pawn
int capture(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player, int col1, int row1, int col2, int row2) {
    int capped_row = (row2 + row1) / 2;
    int capped_col = (col2 + col1) / 2;

    // Check if move is valid
    if (isValid(Board, Player, col1, row1, col2, row2) == 0) {
        return 0;
    }
    // Check if you move by 2
    if (abs(col2 - col1) != 2) return 0;

    //Perform capture
    Board[capped_row][capped_col].color = NONE;
    Board[capped_row][capped_row].isKing = false;

    Board[row2][col2].color = Player;
    if ((row2 == 0 && Player == BLACK) || (row2 == 7 && Player == WHITE)) Board[row2][col2].isKing = true;
    else Board[row2][col2].isKing = Board[row1][col1].isKing;

    // Remove from old position
    Board[row1][col1].color = NONE;
    Board[row1][col1].isKing = false;

    return 1;
}

// Checks if player can capture from a given position
int captures(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player, int col, int row) {
    // Downright
    // Changed from .col and .row to .y and .x ~ Tomasz (Changed it back cos I can read this better))
    if (isValid(Board, Player, col, row, col + 2, row + 2) == 1) {
        return true;
    }
    if (isValid(Board, Player, col, row, col - 2, row + 2) == 1) {
        return true;
    }
    if (isValid(Board, Player, col, row, col - 2, row - 2) == 1) {
        return true;
    }
    if (isValid(Board, Player, col, row, col + 2, row - 2) == 1) {
        return true;
    }
    return false;
}

// Checks if player has any captures
int hasCapt(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = (row + 1) % 2; col < BOARD_SIZE;col += 2) {
            if (Board[row][col].color == Player) {
                if (captures(Board, Player, col, row) == 1) {
                    // Player has captures
                    return true;
                }
            }
        }
    }
    return false;
}

// Checks if player has any moves (including captures if captures resturns 2)
int hasMoves(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player) {
    // If player has captures, he cannot move so send out that he has captures
    if (hasCapt(Board, Player)) return 2;

    // Check if player has normal moves
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = (row + 1) % 2; col < BOARD_SIZE;col += 2) {
            if (Board[row][col].color == Player) {
                // Check if player can move by 1 (normal)
                if (Board[row][col].isKing && isValid(Board, Player, col, row, col + 1, row + Player) || isValid(Board, Player, col, row, col - 1, row + Player)) {
                    return true;
                }
                // Check if player can move by 1 (KING)
                if (isValid(Board, Player, col, row, col + 1, row + 1) || isValid(Board, Player, col, row, col - 1, row + 1) ||
                    isValid(Board, Player, col, row, col + 1, row - 1) || isValid(Board, Player, col, row, col - 1, row - 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

int switchTurn(MAN Board[BOARD_SIZE][BOARD_SIZE],int* turn) {
    *turn = -1 * (*turn);
    saveToFile(Board, *turn);
    if (hasMoves(Board, *turn) == 0) {
        return *turn;
    }
    return 0;
}

//-----------Input--------------

int PerformMove(MAN Board[BOARD_SIZE][BOARD_SIZE], int Player, int col1, int row1, int col2, int row2){
    if (abs(col2 - col1) == 1 && abs(row2 - row1) == 1 && hasCapt(Board, Player) == 0 && move(Board, Player, col1, row1, col2, row2) == true ) {
        // Normal move
        return true;
    }
    else if (abs(col2 - col1) == 2 && abs(row2 - row1) == 2 && capture(Board, Player, col1, row1, col2, row2) == true) {
        // Capture return 2
        return 2;
    }
    else {
        // Invalid move
        return false;
    }
}