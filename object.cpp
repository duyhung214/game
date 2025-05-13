#include "object.h"
using namespace std;

vector <Entity*> objects;

void spawn_objects()
{
    objects.emplace_back(new Wall(vector2f{49, 96}, SDL_Rect{6, 6, 175, 20}));
    objects.emplace_back(new Wall(vector2f{262, 96}, SDL_Rect{3, 39, 235, 22}));
    objects.emplace_back(new Wall(vector2f{53, 288}, SDL_Rect{0, 74, 271, 32}));
    objects.emplace_back(new Wall(vector2f{65, 410}, SDL_Rect{8, 119, 141, 19}));
    objects.emplace_back(new Wall(vector2f{206, 560}, SDL_Rect{3, 150, 267, 22}));

    objects.emplace_back(new Decorations(vector2f{142, 222}, SDL_Rect{3, 2, 57, 28}));
    objects.emplace_back(new Decorations(vector2f{321, 458}, SDL_Rect{3, 34, 42, 28}));
    objects.emplace_back(new Decorations(vector2f{320, 507}, SDL_Rect{3, 34, 42, 28}));
    objects.emplace_back(new Decorations(vector2f{167, 212}, SDL_Rect{52, 37, 8, 7}));
    objects.emplace_back(new Decorations(vector2f{137, 251}, SDL_Rect{52, 37, 8, 7}));
    objects.emplace_back(new Decorations(vector2f{296, 504}, SDL_Rect{52, 37, 8, 7}));
    objects.emplace_back(new Decorations(vector2f{407, 440}, SDL_Rect{52, 37, 8, 7}));
    objects.emplace_back(new Decorations(vector2f{192, 253}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{272, 232}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{304, 280}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{80, 248}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{223, 504}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{280, 534}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{391, 511}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{345, 490}, SDL_Rect{51, 52, 10, 7}));
    objects.emplace_back(new Decorations(vector2f{241, 262}, SDL_Rect{4, 66, 8, 10}));
    objects.emplace_back(new Decorations(vector2f{145, 278}, SDL_Rect{4, 66, 8, 10}));
    objects.emplace_back(new Decorations(vector2f{437, 464}, SDL_Rect{37, 70, 22, 19}));
    objects.emplace_back(new Decorations(vector2f{217, 521}, SDL_Rect{37, 70, 22, 19}));
    objects.emplace_back(new Decorations(vector2f{229, 281}, SDL_Rect{3, 85, 26, 19}));
    objects.emplace_back(new Decorations(vector2f{66, 197}, SDL_Rect{3, 85, 26, 19}));
    objects.emplace_back(new Decorations(vector2f{436, 517}, SDL_Rect{19, 65, 10, 14}));
    objects.emplace_back(new Decorations(vector2f{276, 485}, SDL_Rect{8, 110, 16, 24}));
    objects.emplace_back(new Decorations(vector2f{388, 485}, SDL_Rect{40, 101, 16, 24}));
}

Wall::Wall(vector2f spawn_point, SDL_Rect new_sprite):
    Entity(spawn_point)
{
    sprite = new_sprite;
    texture = main_window.load_texture("wall_2.png");
}

Decorations::Decorations(vector2f spawn_point, SDL_Rect new_sprite):
    Entity(spawn_point)
{
    sprite = new_sprite;
    texture = main_window.load_texture("decorations.png");
}
