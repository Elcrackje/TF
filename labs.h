#define _LABS_H_

#include "upc.h"
#include <fstream>

struct Cell {
    int ch;
    int backcolor;
    int forecolor;

};
  
struct Map {
    int nrows;
    int ncols;
    int nglyphs;
    Cell** map;
    std::string* glyphs;
};

Map* loadMap(std::string filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        return nullptr;
    }

    Map* map = new Map;
    f >> map->nrows >> map->ncols >> map -> nglyphs;
    map->glyphs = new std::string[map->nglyphs];

    for (int i = 0; i < map->nglyphs; ++i) {
        f >> map->glyphs[i];
    }

    map->map = new Cell * [map->nrows];
    for (int i = 0; i < map->nrows; ++i) {
        map->map[i] = new Cell[map->ncols];
        for (int j = 0; j < map->ncols; ++j) {
            int val;
            f >> std::hex >> val;
            map->map[i][j].ch = val >> 8;
            map->map[i][j].backcolor = (val >> 4) & 0xf;
            map->map[i][j].forecolor = val & 0xf;
        }
    }
    return map;
}

void drawMap(Map* map, ConsoleInfo* ci) {
    for (int i = 0; i < map->nrows; ++i) {
        for (int j = 0; j < map ->ncols; ++j) {
            if (map->map[i][j].ch == 0) {
                std::cout << " ";
            }
            else {
                color(map->map[i][j].forecolor, map->map[i][j].backcolor);
                std::cout << map->glyphs[map->map[i][j].ch];
                clearColor();
            }
        }
        std::cout << std::endl;
    }
}

void byeMap(Map* map) {
    for (int i = 0; i < map->nrows; ++i) {
        delete[] map->map[i];
    }
    delete map;
}
