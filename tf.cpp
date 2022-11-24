#include "game.h"

using namespace std;

// Caracteres!
char glyphs[] = { 32, 219, 2, 176, 1, 3, 178, 179, 6};
// string glyphs[] = { " ", "█", "☻", "░", "☺","♥", "▓", "│", "♠"};

/* codigos en matriz
0x0700 (FINALMAP)
0x0409 (ENEMY)
0x0155 (WALL)
0x0506 (LIFES)
0X0600(FINAL)
0x080b (ALLIES)
*/




//Constantes de tipo de elemento
#define EMPTY    0
#define WALL     1
#define HERO     2
#define TRAIL    3
#define ENEMY    4
#define LIFES    5
#define FINAL    6
#define FINALMAP 7
#define ALLIES   8

#define FHERO    10
#define BHERO    0
#define FTRAIL   7
#define BTRAIL   0

// Variables globales
char key;
bool gameover;
bool nextLevel;
int start;
int lifes;
int allies;
int delay;
int x_; int y_;
int x; int y;

//parametros del juego
void parameters() {
    gameover = false;
    nextLevel = false;
    lifes = 3;
    delay = 50;
    allies = 0;
    x_ = 20;  y_ = 1;
    x = 20; y = 1;

     
}
void loadStuff(string fname, Map*& map, ConsoleInfo*& ci) {
    ci = new ConsoleInfo;
    getConsoleInfo(ci);
    map = loadMap(fname);
    start = 0;
    int marginv = 0;
    int marginh = ci->maxColumns - map->cols;
    if (marginh < 0) {
        map = nullptr; // console too small for labyrinth, try making it bigger
    }
    else {
        int top = marginv / 2;
        int left = marginh / 2;
        int bottom = marginv - top;
        int right = marginh - left;
        getConsoleInfo(ci, top, right, bottom, left);
    }
}
//cargar mapa
void drawMap(Map* map, ConsoleInfo* ci) {
    clear();
    for (int i = start, k = 0; i < map->rows && k < ci->bottom; ++i, ++k) {
        gotoxy(ci->left, ci->top + k);
        for (int j = 0; j < map->cols; ++j) {
            if (map->cells[i][j].glyph == EMPTY) {
                cout << " ";
            }
            else {
                color(map->cells[i][j].fcolor, map->cells[i][j].bcolor);
                cout << glyphs[map->cells[i][j].glyph];
                clearColor();
            }
        }
    }
}

void drawCell(int x, int y, Map* map, ConsoleInfo* ci) {
    gotoxy(ci->left + x, ci->top + y - start);
    color(map->cells[y][x].fcolor, map->cells[y][x].bcolor);
    cout << glyphs[map->cells[y][x].glyph];
    gotoxy(0, 0);
}

//funcion para mostar las reglas
void drawRules()
{
    cout << "          __________________________________________________________________________________________________" << endl;
    cout << "         *   REGLAMENTO:                                                                                    *" << endl;
    cout << "         |   *) Cada jugador empieza con 3 vidas.                                                           |" << endl;
    cout << "         |   *) El juego termina si el jugador pierde todas las vidas.                                      |" << endl;
    cout << "         |   *) El jugador perdera una vida cada vez que colisione con un enemigo.                          |" << endl;
    cout << "         |   *) El jugador ganara una vida si consume un corazon.                                           |" << endl;
    cout << "         |   *) Existen aliados, al conseguir 3 obtienes una vida extra                                     |" << endl;
    cout << "         |   *) Los aliados son eliminados si pierden las 3 vidas                                           |" << endl;
    cout << "         |                                                                                                  |" << endl;
    cout << "         |                             Presione cualquier tecla para continuar                              |" << endl;
    cout << "         *__________________________________________________________________________________________________*" << endl;

    _getch();

}

void credits() {
    cout << "          __________________________________________________________________________________________________" << endl;
    cout << "         *   CREDITOS:                                                                                    *" << endl;
    cout << "         |   *) Jose Gutierrez                                                                              |" << endl;
    cout << "         |   *) Sebastian Montañez                                                                          |" << endl;
    cout << "         |   *) Carlos Vazquez                                                                              |" << endl;
    cout << "         |                             Presione cualquier tecla para continuar                              |" << endl;
    cout << "         *__________________________________________________________________________________________________*" << endl;

    _getch();

}

// termina el juego
void finishGame(Map* map, ConsoleInfo*& ci) {
    clear();
    color(BRIGHT_RED);
    gotoxy(ci->maxColumns / 2 - 4, ci->maxRows / 2);
    cout << "GAME OVER";
    gotoxy(0, 0);
    cin.get();
    gameover = true;
}
void winMap(Map* map, ConsoleInfo*& ci) {
    clear();
    color(DARK_GREEN);
    gotoxy(ci->maxColumns / 2 - 4, ci->maxRows / 2);
    cout << "LEVEL 1 COMPLETED\n";
    gotoxy(ci->maxColumns / 2 - 4, ci->maxRows / 2);
    cout << "Press any key to continue to the next level\n";
    gotoxy(0, 0);
    _getch();
    clear();
    nextLevel = true;
}

void winGame(Map* map, ConsoleInfo*& ci) {
    clear();
    color(DARK_GREEN);
    gotoxy(ci->maxColumns / 2 - 4, ci->maxRows / 2);
    cout << "YOU WIN";
    gotoxy(0, 0);
    cin.get();
    gameover = true;
}
//movimiento 
void movement() {
    key = toupper(_getch());
    x_ = x;
    y_ = y;
    switch (key) {
    case 'W': --y; break;
    case 'A': --x; break;
    case 'S': ++y; break;
    case 'D': ++x; break;
    case 'X': gameover = true; break;
    }   

}

void continueGame(Map* map, ConsoleInfo*& ci) {
    map->cells[y_][x_] = { TRAIL, FTRAIL, BTRAIL };
    drawCell(x_, y_, map, ci);
    map->cells[y][x] = { HERO, FHERO, BHERO };
    drawCell(x, y, map, ci);
    
}
void printScore(Map* map, ConsoleInfo*& ci) {
    //coordenadas para imprmir las vidas
    int lx = ci->left - 25;
    int ly = ci->top + 5;
    gotoxy(lx, ly);
    cout << "Tienes " << lifes << " Vidas" << endl;
    //coords para imprimir el numero de aliados conseguidos
    gotoxy(lx, ly + 2);
    cout << "Tienes " << allies << " Aliados" << endl;
}

void validationGame(Map* map, ConsoleInfo*& ci) {
    // si se chocan
    if (map->cells[y][x].glyph == ENEMY) {
        --lifes;
        printScore(map,ci);
    }
    if (map->cells[y][x].glyph == WALL) {
        finishGame(map, ci);
    }
    if (lifes == 0) {
        finishGame(map, ci);
    }
    if (map->cells[y][x].glyph == LIFES) {
        ++lifes;
        printScore(map, ci);
    }
    if (map->cells[y][x].glyph == ALLIES) {
        ++allies;
        printScore(map, ci);
    }
    if (allies == 3|| allies == 6) {
        ++lifes;
        printScore(map, ci);
    }
    if (map->cells[y][x].glyph == FINAL) {
        winMap(map, ci);
    }
    if (map->cells[y][x].glyph == FINALMAP) {
        winGame(map, ci);
    }

    //si no se chocan, el juego continua
    else {
        continueGame(map, ci);
    }
    
}

void principalMenu(Map* logo,Map* map, ConsoleInfo*& ci) {
    drawMap(logo, ci);
    _getch();
    clear();
    credits();
    drawRules();
   
    // parametros del juego
    parameters();

    hideCursor();
    noecho();
    //dibujar mapa
    drawMap(map, ci);
}

int loadAllStuff(string fname1, Map*& logo, string fname2, Map*& map1, string fname3, Map*& map2, ConsoleInfo*& ci) {
    //cargar logo
    loadStuff(fname1, logo, ci);
    //cargar mapa 1
    loadStuff(fname2, map1, ci);
    //cargar mapa 2
    loadStuff(fname3, map2, ci);
    // si la consola es muy pequeña no abre 
    if (map1 == nullptr || logo == nullptr || map2 == nullptr) {
        cout << "Terminal too small for this map\n";
        return -1;
    }
}

void preLvl2(Map* map2, ConsoleInfo*& ci) {
    clear();
    x_ = 20;  y_ = 1;
    x = 20; y = 1;
    hideCursor();
    noecho();
    _getch();

    drawMap(map2, ci);
}


int main() {
    ConsoleInfo* ci;
    Map* map1;
    Map* logo;
    Map* map2;

    loadAllStuff("log.awesome", logo, "map2.awesome", map1, "map3.awesome", map2, ci);
        

    principalMenu(logo, map1, ci);
    //nivel 1
    while (!gameover) {
        sleep4(delay);
        printScore(map1, ci);
        //si detecta la tecla presionada
        if (_kbhit()) {
            //hace el movimiento
            movement();
            // si se chocan o no
            if (x_ != x || y_ != y) {
                validationGame(map1, ci);
            }
            if (nextLevel == true) {
                break;
            }
        }
    }
    //nivel 2
    destroyMap(map1);
    preLvl2(map2, ci);
   
    while (!gameover) {
        sleep4(delay);
        printScore(map2, ci);
        //si detecta la tecla presionada
        if (_kbhit()) {
            //hace el movimiento
            movement();
            // si se chocan o no
            if (x_ != x || y_ != y) {
                validationGame(map2, ci);
            }
        }
    }

    resetAll();
    delete ci;
    destroyMap(map2);
}