#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include "types.h"
#include "board.h"


int rotate_player(int start, int end, int numero, int quanto_ruota){
    return start + (numero + quanto_ruota - start) % end;
}

void clear() { printf("\033[H\033[J"); }

void set_color(int color) { printf("\033[0;%dm", color); }


void colored_bg(int color, int bg) {
    printf("\x1b[%d;%dm", color, bg);
}


Giocatore check_win(int count_pedine[]){
    if(count_pedine[0] == 0)
        return PLAYER2;
    
    if(count_pedine[1] == 0)
        return PLAYER1;
    
    return NONE;
}


Mossa get_mossa(Pedina table[ROWS][COLUMNS], Giocatore player){
    Mossa mossa;
    Pedina current;
    
    do{
        set_color(player == PLAYER1 ? GIALLO : BLU);
        printf("(%d) Inserisci le coordinate nel formato (Riga Colonna): ", player);
        set_color(BIANCO-10);
        scanf("%d %d", &mossa.x, &mossa.y);

        current = table[mossa.x][mossa.y];

        if(current.giocatore != player){
            printf("La pedina non è tua\n");
        } else{

            printf(
                "Scegli la direzione:\n"
                "0: alto destra\n"
                "1: alto sinistra\n"
                "2: basso destra\n"
                "3: basso sinistra\n"
                "Inserisci la tua scelta: "
            );
            scanf("%d", &mossa.direction);
        }

    } while(current.giocatore != player);
    
    return mossa;
}


void fill_table(Pedina table[ROWS][COLUMNS]){
    Giocatore player = PLAYER1;
    bool inserted_player = true;

    for(int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            if(i == 3){
                i += 2;
                player = PLAYER2;
            }

            if(!inserted_player)
                table[i][j].giocatore = player;

            inserted_player = !inserted_player;
        }
        inserted_player = !inserted_player;
    }
}



void show_table(Pedina table[ROWS][COLUMNS]){
    table[0][0].is_dama = false;
    bool black = false;
    printf(" "); 
    for(int i = 0; i < COLUMNS; i++)
        printf(" %d", i);
    printf("\n");
    
    for(int i = 0; i < ROWS; i++){
        set_color(BIANCO-10);
        printf("%d ", i);
        for(int j = 0; j < COLUMNS; j++){
            black = !black;
            
            colored_bg(
                (table[i][j].giocatore == PLAYER1) ? GIALLO : BLU,
                black ? BIANCO: NERO
            );

            printf("%ls ",
                (table[i][j].is_dama) ? DAMA :
                ((table[i][j].giocatore != 0) ? PEDINA : SPAZIO)
            );
        }
        black = !black;
        set_color(BIANCO-10);
        printf("\n");
    }
}


bool move_pedina(Pedina table[ROWS][COLUMNS], const int pedina_x, const int pedina_y, const Direction direction, int count_pedine[]){
    Pedina current = table[pedina_x][pedina_y];

    if(!current.is_dama){
        if(direction >= BOTTOM_RIGHT && current.giocatore == PLAYER2)
            return false;

        if(direction < BOTTOM_RIGHT && current.giocatore == PLAYER1)
            return false;
    }

    int new_x, new_y;

    switch(direction){
        case TOP_RIGHT:
            new_x = -1;
            new_y = 1;
            break;
        case TOP_LEFT:
            new_x = -1;
            new_y = -1;
            break;
        case BOTTOM_RIGHT:
            new_x = 1;
            new_y = 1;
            break;
        case BOTTOM_LEFT:
            new_x = 1;
            new_y = -1;
            break;
        default:
            return false;
    }


    int new_pos_x = pedina_x + new_x;
    int new_pos_y = pedina_y + new_y;

    Pedina new_position = table[new_pos_x][new_pos_y];


    if(new_pos_x >= ROWS || new_pos_y >= COLUMNS || new_pos_x < 0 || new_pos_y < 0)
        return false;

    //il player è uguale
    if(new_position.giocatore == current.giocatore)
        return false;
    
    //lo spazio è vuoto
    if(new_position.giocatore == NONE){
        table[new_pos_x][new_pos_y].giocatore = current.giocatore;
        table[new_pos_x][new_pos_y].is_dama = current.is_dama;
        table[pedina_x][pedina_y].giocatore = NONE;
        table[pedina_x][pedina_y].is_dama = false;


        //controllo se deve diventare dama
        if(new_pos_x == 0 || new_pos_x == ROWS-1)
            table[new_pos_x][new_pos_y].is_dama = true;

        return true;
    }

    //controllo che la pediana is a dama
    if(new_position.is_dama && !current.is_dama)
        return false;

    // Player is different
    new_pos_x = pedina_x + new_x * 2;
    new_pos_y = pedina_y + new_y * 2;

    if(table[new_pos_x][new_pos_y].giocatore != NONE)
        return false;

    
    table[new_pos_x][new_pos_y].giocatore = current.giocatore;
    table[new_pos_x][new_pos_y].is_dama = current.is_dama;
    table[pedina_x][pedina_y].giocatore = NONE;
    table[pedina_x][pedina_y].is_dama = false;


    // eat
    count_pedine[table[pedina_x + new_x][pedina_y + new_y].giocatore]--;
    table[pedina_x + new_x][pedina_y + new_y].giocatore = NONE;
    table[pedina_x + new_x][pedina_y + new_y].is_dama = false;

    //controllo se deve diventare dama
    if(new_pos_x == 0 || new_pos_x == ROWS-1)
        table[new_pos_x][new_pos_y].is_dama = true;

    return true;

}


void play(){
    Pedina table[ROWS][COLUMNS] = { 0 };
    Giocatore player = PLAYER1;

    int count_pedine[2] = {
        12, 12
    };

    fill_table(table);
    show_table(table);


    do{

        Mossa mossa = get_mossa(table, player);

        if(move_pedina(table, mossa.x, mossa.y, mossa.direction, count_pedine)){
            
            
            player = rotate_player(PLAYER1, PLAYER2, player, 1);

        } else{
            printf("\n\nMossa non valida.\n\n");
        }


            
        show_table(table);

    } while(check_win(count_pedine) == 0);



}

/**
 * TODO:
 * mangiata doppia
 ** cursore
 ** board fancy
 * DAJE ROMA DAJEEE
 * dama fix
*/


int main(){
    setlocale(LC_ALL, ""); // Per usare i caratteri unicode
    play();

    return 0;
}