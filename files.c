//-----------File---------------
void saveToFile(MAN board[BOARD_SIZE][BOARD_SIZE], int turn) {
    //Saving the state of the game to gameSave.txt file
    FILE* gameSave = fopen("gameSave.txt", "w");

    //First line tells which turn it is
    fprintf(gameSave, "%d\n", turn);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (abs(board[i][j].color) == 1) {
                //Format of the save file is {row} {column} {color} {isKing}
                fprintf(gameSave, "%d %d %d %d\n", i, j, board[i][j].color, board[i][j].isKing);
            }
        }
    }

    fclose(gameSave);
}

int ReadBoard(MAN Board[BOARD_SIZE][BOARD_SIZE], int* turn) {
    if (access("gameSave.txt", F_OK) == 0) {
        FILE* gameSave = fopen("gameSave.txt", "r");
        int row, col, color, isKing;

        fscanf(gameSave, "%d", turn);
        if (*turn != WHITE && *turn != BLACK) {
            // Invalid turn, do not save the game
            *turn = BLACK;
        }

        while (!feof(gameSave)) {
            fscanf(gameSave, "%d %d %d %d", &row, &col, &color, &isKing);
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || (color != WHITE && color != BLACK && color != NONE) || (isKing != 0 && isKing != 1) || (color == NONE && isKing == 1)|| (col+row) % 2 != 1) {
                // Invalid data in the save file ignore enrty
                continue;
            }
            Board[row][col].color = color;
            Board[row][col].isKing = isKing;
        }
        fclose(gameSave);

        return true;
    }
    return false;
}

//This function will remove the save file when the game is completed (when someone wins)
void deleteFile() {
    if (remove("gameSave.txt") == 0) {
        printf("File deleted\n");
    }
    else {
        printf("Error: Save File cannot be deleted\n");
    }
}