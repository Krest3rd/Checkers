#define BOARD_SIZE 8
#define BOARD_SQUARE_SIZE 80
#define BOARD_WIDTH (BOARD_SIZE * BOARD_SQUARE_SIZE)
#define BOARD_HEIGHT (BOARD_SIZE * BOARD_SQUARE_SIZE)
#define BOARD_X 0
#define BOARD_Y 50

enum COLOR {
    WHITE = 1, //Idą w dół (row+1)
    NONE = 0,
    BLACK = -1 //Idą w górę (row - 1)
};

enum COLS {
    A = 0,
    B,
    C,
    D,
    E,
    F,
    G,
    H
};

typedef struct MAN {
    int isKing;
    int color;
} MAN;

typedef struct field {
    int col;
    int row;
} field;

// typedef struct Field {
//     int isFull;
//     MAN checker;
// } Field;