#ifndef player_H
#define player_H

#include "window.h"
#include "enemy.h"
using namespace std;

class Player : public Entity
{
public:  
    Player();
    void player_render();
    void update(float current_time, bool is_attack, float delta_time,
        vector <Enemy*> &enemies);
    void camera_update();
    bool is_death();
    SDL_Rect get_leg_rect();
    float get_y();
    int get_health();
private:
    int wait;
    vector2f direction;
    //real hitbox = +8, +10, 15, 20
};

void running();

extern Player main_player;


#endif
