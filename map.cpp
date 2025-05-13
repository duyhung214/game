#include "map.h"
using namespace std;

SDL_Rect camera;
vector <SDL_Rect> map_tiles;

Map::Map()
{

}
void Map::map_init()
{
    MapTexture = main_window.load_texture("dungeon_2.png");
    camera.x = camera.y = 0;
    camera.w = CAMERA_WIDTH;
    camera.h = CAMERA_HEIGHT;

    // map rectangle
    map_tiles.clear();
    map_tiles.emplace_back(SDL_Rect{49, 41, 440, 7});
    map_tiles.emplace_back(SDL_Rect{307, 48, 14, 1});
    map_tiles.emplace_back(SDL_Rect{43, 48, 6, 68});
    map_tiles.emplace_back(SDL_Rect{49, 111, 175, 5});
    map_tiles.emplace_back(SDL_Rect{491, 48, 6, 68});
    map_tiles.emplace_back(SDL_Rect{262, 111, 229, 5});
    map_tiles.emplace_back(SDL_Rect{215, 116, 6, 76});
    map_tiles.emplace_back(SDL_Rect{270, 116, 6, 76});
    map_tiles.emplace_back(SDL_Rect{52, 186, 163, 6});
    map_tiles.emplace_back(SDL_Rect{276, 186, 49, 6});
    map_tiles.emplace_back(SDL_Rect{47, 192, 6, 128});
    map_tiles.emplace_back(SDL_Rect{53, 314, 15, 6});
    map_tiles.emplace_back(SDL_Rect{90, 314, 234, 6});
    map_tiles.emplace_back(SDL_Rect{324, 192, 6, 128});
    map_tiles.emplace_back(SDL_Rect{59, 320, 6, 109});
    map_tiles.emplace_back(SDL_Rect{106, 320, 6, 66});
    map_tiles.emplace_back(SDL_Rect{65, 424, 141, 5});
    map_tiles.emplace_back(SDL_Rect{112, 381, 132, 5});
    map_tiles.emplace_back(SDL_Rect{244, 386, 6, 46});
    map_tiles.emplace_back(SDL_Rect{200, 429, 6, 151});
    map_tiles.emplace_back(SDL_Rect{250, 426, 223, 6});
    map_tiles.emplace_back(SDL_Rect{309, 432, 18, 4});
    map_tiles.emplace_back(SDL_Rect{473, 432, 6, 148});
    map_tiles.emplace_back(SDL_Rect{206, 574, 267, 6});
    map_tiles.emplace_back(SDL_Rect{272, 235, 10, 4});
    map_tiles.emplace_back(SDL_Rect{142, 225, 58, 25});
    map_tiles.emplace_back(SDL_Rect{167, 214, 8, 5});
    map_tiles.emplace_back(SDL_Rect{137, 253, 8, 5});
    map_tiles.emplace_back(SDL_Rect{192, 255, 10, 5});
    map_tiles.emplace_back(SDL_Rect{272, 234, 10, 5});
    map_tiles.emplace_back(SDL_Rect{304, 282, 10, 5});
    map_tiles.emplace_back(SDL_Rect{80, 250, 10, 5});
    map_tiles.emplace_back(SDL_Rect{66, 206, 26, 10});
    map_tiles.emplace_back(SDL_Rect{229, 290, 26, 10});
    map_tiles.emplace_back(SDL_Rect{241, 266, 8, 6});
    map_tiles.emplace_back(SDL_Rect{145, 282, 8, 6});
    map_tiles.emplace_back(SDL_Rect{320, 511, 42, 24});
    map_tiles.emplace_back(SDL_Rect{321, 462, 42, 24});
    map_tiles.emplace_back(SDL_Rect{345, 492, 10, 5});
    map_tiles.emplace_back(SDL_Rect{391, 513, 10, 5});
    map_tiles.emplace_back(SDL_Rect{280, 536, 10, 5});
    map_tiles.emplace_back(SDL_Rect{223, 506, 10, 5});
    map_tiles.emplace_back(SDL_Rect{296, 506, 8, 5});
    map_tiles.emplace_back(SDL_Rect{407, 442, 8, 5});
    map_tiles.emplace_back(SDL_Rect{217, 530, 22, 10});
    map_tiles.emplace_back(SDL_Rect{436, 521, 10, 10});
    map_tiles.emplace_back(SDL_Rect{437, 473, 22, 10});
    map_tiles.emplace_back(SDL_Rect{276, 495, 16, 14});
    map_tiles.emplace_back(SDL_Rect{388, 495, 16, 14});
    
}

void Map::map_render()
{
    SDL_RenderCopy(main_window.renderer, MapTexture, &camera, NULL);
}
