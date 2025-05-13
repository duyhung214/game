#ifndef map_H
#define map_H
#include "window.h"
using namespace std;

class Map
{
public:
    Map();
    void map_init();
    void map_render();
private:
    SDL_Texture* MapTexture;
    vector2f pos;
};

#endif
