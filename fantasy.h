#ifndef fantasy_H
#define fantasy_H
#include "window.h"
#include "enemy.h"
#include "map.h"
using namespace std;

class Sword : public Entity
{
public:
    Sword();
    void sword_render();
    bool is_attack();
    SDL_Rect set_attack_hitbox();
    void attack(float now, vector2f player_pos, SDL_RendererFlip player_flip);
private:
    double roll;
    float last_attack;
};

class Heart : public Entity
{
public:
    Heart(vector2f new_pos);
    void update(int new_order);
};

extern vector <Heart*> health_bar;
extern Sword sword;
void set_health_bar();

class Door : public Enemy
{
public:
    Door(vector2f spawn_point, int type);
    float get_y();
    void update(float current_time, bool wave);
};

#endif
