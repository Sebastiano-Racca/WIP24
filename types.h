#define ROWS 8
#define COLUMNS 8

typedef enum{
    NONE,
    PLAYER1,
    PLAYER2,
} Giocatore;


typedef enum{
    TOP_RIGHT,
    TOP_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT,
} Direction;



typedef struct{
    bool is_dama;
    Giocatore giocatore;
} Pedina;


typedef struct{
    int x;
    int y;
    Direction direction;
} Mossa;