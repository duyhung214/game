#ifndef enemy_H
#define enemy_H
#include "window.h"
using namespace std;

class Enemy : public Entity
{
public:
    virtual ~Enemy() = default;
    bool is_death();
    bool is_attack();
    bool is_spawn();
    SDL_Rect get_attack_hitbox();
    virtual SDL_Rect get_leg_rect() {return SDL_Rect{0, 0, 0, 0};}
    float get_y();
    Enemy(vector2f spawn_point, SDL_Rect new_sprite);
protected:
    int wait;
    vector2f target;
    bool spawned, death, attack;
    SDL_Rect real_attack_hitbox;
};

extern vector <Enemy*> rooms[4];
extern vector <vector2f> pos_available;
extern vector <Enemy*> enemies;

void spawn_enemies();

void set_pos_available();

class Slime : public Enemy
{
public:
    Slime(vector2f spawn_point);
    SDL_Rect set_attack_hitbox();
    SDL_Rect get_leg_rect();
    void update(float current_time, float delta_time);
private:
    vector2f direction;
    vector2f max_move, current_move;
    SDL_Rect attack_hitbox[4];
};

class Troll : public Enemy
{
public:
    Troll(vector2f spawn_point);
    SDL_Rect set_attack_hitbox();
    SDL_Rect get_leg_rect();
    void update(float current_time, float delta_time);
private:
    vector2f direction;
    vector2f max_move, current_move;
};

class Orc : public Enemy
{
public:
    Orc(vector2f spawn_point);
    SDL_Rect set_attack_hitbox();
    SDL_Rect get_leg_rect();
    void update(float current_time, float delta_time);
private:
    vector2f direction;
    vector2f max_move, current_move;
    SDL_Rect attack_hitbox[4];
};

class Skeleton : public Enemy
{
public:
    Skeleton(vector2f spawn_point);
    SDL_Rect set_attack_hitbox();
    SDL_Rect get_leg_rect();
    void update(float current_time, float delta_time);
private:
    vector2f direction;
    vector2f max_move, current_move;
};

class Projectile : public Enemy
{
public:
    Projectile(vector2f spawn_point, int type, vector <Enemy*> &enemies);
    void update(float current_time, float delta_time);
    float get_y();
    void set_death();
private:
    vector2f direction;
    float speedx, speedy;
};

class Neucromancer : public Enemy
{
public:
    Neucromancer(vector2f spawn_point);
    SDL_Rect get_leg_rect();
    void update(float current_time, float delta_time, vector <Enemy*> &enemies);
private:
    vector2f direction;
    vector2f max_move, current_move;
    float last_summon, last_fire;
};

#endif
