#include "enemy.h"
#include "player.h"
#include "fantasy.h"
using namespace std;

vector <Enemy*> enemies;
vector <Enemy*> rooms[4];
vector <vector2f> pos_available;

Enemy::Enemy(vector2f spawn_point, SDL_Rect new_sprite):
    Entity(spawn_point) // player's hitbox = {+8, +10, 15, 20}
{
    spawned = death = false;
    pos = spawn_point;
    sprite = new_sprite;
    last_update = 0;
    target.x = 13 + rng() % 4;
    target.y = 18 + rng() % 4;
}

bool Enemy::is_death()
{
    return death;
}

bool Enemy::is_attack()
{
    return attack;
}

bool Enemy::is_spawn()
{
    return spawned;
}

SDL_Rect Enemy::get_attack_hitbox()
{
    return real_attack_hitbox;
}

float Enemy::get_y()
{
    return get_leg_rect().y + get_leg_rect().h;
}

void spawn_enemies()
{
    enemies.clear();
    for(int i = 0; i < 3; ++i) rooms[i].clear();

    rooms[0].emplace_back(new Slime(vector2f(128, 31)));
    rooms[0].emplace_back(new Slime(vector2f(143, 60)));
    rooms[0].emplace_back(new Slime(vector2f(109, 49)));
    rooms[0].emplace_back(new Slime(vector2f(305, 37)));
    rooms[0].emplace_back(new Slime(vector2f(285, 52)));
    rooms[0].emplace_back(new Troll(vector2f(376, 36)));
    rooms[0].emplace_back(new Troll(vector2f(353, 63)));
    rooms[0].emplace_back(new Troll(vector2f(393, 54)));
    rooms[0].emplace_back(new Troll(vector2f(419, 65)));
    rooms[0].emplace_back(new Door(vector2f(219, 90), 0));
    rooms[0].emplace_back(new Door(vector2f(55, 293), 1));
    rooms[0].emplace_back(new Door(vector2f(201, 402), 0));

    rooms[1].emplace_back(new Orc(vector2f(237, 254)));
    rooms[1].emplace_back(new Orc(vector2f(39, 173)));
    rooms[1].emplace_back(new Troll(vector2f(94, 223)));
    rooms[1].emplace_back(new Troll(vector2f(115, 258)));
    rooms[1].emplace_back(new Troll(vector2f(194, 266)));
    rooms[1].emplace_back(new Slime(vector2f(152, 233)));
    rooms[1].emplace_back(new Slime(vector2f(170, 256)));
    rooms[1].emplace_back(new Slime(vector2f(290, 229)));
    rooms[1].emplace_back(new Slime(vector2f(132, 179)));
    rooms[1].emplace_back(new Door(vector2f(219, 90), 0));
    rooms[1].emplace_back(new Door(vector2f(55, 293), 1));
    rooms[1].emplace_back(new Door(vector2f(201, 402), 0));

    rooms[2].emplace_back(new Neucromancer(vector2f(297, 463)));
    rooms[2].emplace_back(new Door(vector2f(219, 90), 0));
    rooms[2].emplace_back(new Door(vector2f(55, 293), 1));
    rooms[2].emplace_back(new Door(vector2f(201, 402), 0));
}

void set_pos_available()
{
    bool check_spawn;
    SDL_Rect p_spawn = SDL_Rect{0, 0, 32, 32};
    for(int ix = 210; ix <= 210 + 261 - 32; ix++)
        for(int iy = 438; iy <= 438 + 132 - 32; iy++)
        {
            p_spawn.x = ix; p_spawn.y = iy;
            check_spawn = 1;
            for(int j = 0; j < (int)map_tiles.size(); ++j)
                if(SDL_HasIntersection(&p_spawn, &map_tiles[j]) == SDL_TRUE)
                {
                    check_spawn = 0;
                    break;
                }
            if(check_spawn) pos_available.emplace_back(vector2f(ix, iy));
        }
}

Slime::Slime(vector2f spawn_point):
    Enemy(spawn_point, {0, 0, 32, 32})
{
    health_point = 5;
    speed = 18 + rng() % 3;
    state = 0;
    wait = 2;
    order = 0;
    texture = main_window.load_texture("slimeball.png");
    attack_hitbox[0] = SDL_Rect{10, 18, 12, 10};
    attack_hitbox[1] = SDL_Rect{9, 20, 14, 8};
    attack_hitbox[2] = SDL_Rect{8, 21, 16, 7};
    attack_hitbox[3] = SDL_Rect{10, 20, 12, 8};
}

SDL_Rect Slime::set_attack_hitbox()
{
    SDL_Rect current_hitbox = SDL_Rect{11, 17, 10, 11};
    if(state == 3)
    {
        switch(order)
        {
            case 3:
                current_hitbox = attack_hitbox[0];
                break;
            case 4:
                current_hitbox = attack_hitbox[1];
                break;
            case 5:
                current_hitbox = attack_hitbox[2];
                break;
            case 6:
                current_hitbox = attack_hitbox[3];
                break;
        }
    }
    current_hitbox.x += pos.x;
    current_hitbox.y += pos.y;
    return current_hitbox;
}

SDL_Rect Slime::get_leg_rect()
{
    return SDL_Rect{11 + (int)pos.x, 26 + (int)pos.y, 10, 2};
}

void Slime::update(float current_time, float delta_time)
{
    //state=0:summon
    //state=1:idle
    //state=2:move
    //state=3:attack
    //state=4:hit
    //state=5:die
    if(!spawned)
    {
        spawned = true;
        state = order = 0;
        wait = 4;
    }
    if(wait == 0 && state == 5)
    {
        death = true;
        return;
    }
    hitbox = real_attack_hitbox = set_attack_hitbox();
    vector2f core(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2);
    SDL_Rect player_hitbox = main_player.get_hitbox();
    SDL_Rect weapon_hitbox = sword.set_attack_hitbox();
    attack = false;

    if(sword.is_attack() && SDL_HasIntersection(&weapon_hitbox, &hitbox) == SDL_TRUE
        && ((state > 0 && state < 4) || (state == 4 && wait == 0)))
    {
        state = 4;
        order = 1;
        wait = 4;
        health_point--;
        if(main_player.get_flip() == SDL_FLIP_NONE)
            direction.x = 1;
        else direction.x = -1;
        direction.y = 1.0f / (1 + rng() % 5);
    }

    if(health_point == 0)
    {
        health_point = -1;
        state = 5;
        order = 0;
        wait = 8;
    }

    if(state == 2)
    {
        if(max_move.x <= 0.0f && max_move.y <= 0.0f) wait = 0;
        if(SDL_HasIntersection(&hitbox, &player_hitbox) == SDL_TRUE) wait = 0;
    }


    if(wait == 0)
    {
        if(SDL_HasIntersection(&hitbox, &player_hitbox)) // in attack range
            {
                state = 3;
                order = 0;
                wait = 8;
            }
        else
        {
            switch(state)
            {
                case 0:
                    state = 1;
                    order = 0;
                    wait = rng() % 5;
                    break;
                case 1:
                {
                    state = 2;
                    order = 0;
                    wait = 4 + rng() % 4;

                    vector2f new_target = main_player.get_pos();

                    new_target.x += target.x;
                    new_target.y += target.y;

                    direction.x = (core.x <= new_target.x ? 1 : -1);
                    direction.y = (core.y <= new_target.y ? 1 : -1);

                    max_move.x = abs(core.x - new_target.x);
                    max_move.y = abs(core.y - new_target.y);
                    break;
                }
                case 2:
                    state = 1;
                    order = 0;
                    wait = 4 + rng() % 4;
                    break;
                case 3:
                    state = 1;
                    order = 0;
                    wait = 4 + rng() % 4;
                    break;
                case 4:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 5:
                    break;
            }
        }
        
    }


    switch(state)
    {
        case 0:
            set_sprite(vector2f(0, order));
            break;
        case 1:
            set_sprite(vector2f(0, 4 + order));
            break;
        case 2:
            set_sprite(vector2f(0, 8 + order));

            if(max_move.x > 0.0f) current_move.x = speed * delta_time;
            else current_move.x = 0;
            if(max_move.y > 0.0f) current_move.y = speed * delta_time;
            else current_move.y = 0;

            max_move.x -= current_move.x;
            max_move.y -= current_move.y;

            move_x(current_move.x * direction.x, get_leg_rect());
            move_y(current_move.y * direction.y, get_leg_rect());

            if(direction.x == 1) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);

            break;
        case 3:
            set_sprite(vector2f(0, 12 + order));
            attack = (order > 2 && order < 7);
            break;
        case 4:
            set_sprite(vector2f(0, 20 + order));
            move_x(speed * 2.0f * delta_time * direction.x, get_leg_rect());
            move_y(speed * 1.5f * delta_time * direction.y, get_leg_rect());
            break;
        case 5:
            set_sprite(vector2f(0, 24 + order));
            break;
            
    }

    if(current_time - last_update > 0.15f)
    {
        last_update = current_time;
        wait--;
        if(order == 3 && state == 5)
            order--;
        order++;
        switch(state)
        {
            case 0: // spawn
                order %= 4;
                break;
            case 1: // idle
                order %= 4;
                break;
            case 2: // move
                order %= 4;
                break;
            case 3: // attack
                order %= 8;
                break;
            case 4: // hit
                order %= 4;
                break;
            case 5: // death
                order %= 4;
                break;
        }
    }
}

Troll::Troll(vector2f spawn_point):
    Enemy(spawn_point, {0, 0, 32, 32})
{
    health_point = 5;
    speed = 28 + rng() % 3;
    state = 0;
    wait = 0;
    order = 0;
    texture = main_window.load_texture("troll.png");
}

SDL_Rect Troll::set_attack_hitbox()
{
    SDL_Rect current_hitbox = hitbox;
    if(attack)
    {
        if(get_flip() == SDL_FLIP_NONE)
            current_hitbox = SDL_Rect{17 + (int)pos.x, 11 + (int)pos.y, 13, 14};
        else
            current_hitbox = SDL_Rect{2 + (int)pos.x, 11 + (int)pos.y, 13, 14};
    }
    return current_hitbox;
}

SDL_Rect Troll::get_leg_rect()
{
    return SDL_Rect{11 + (int)pos.x, 26 + (int)pos.y, 10, 2};
}

void Troll::update(float current_time, float delta_time)
{
    if(!spawned)
    {
        spawned = true;
        state = 1;
        order = 0;
        wait = 4;
    }
    if(wait == 0 && state == 5)
    {
        death = true;
        return;
    }
    hitbox = SDL_Rect{11 + (int)pos.x, 13 + (int)pos.y, 10, 14};
    real_attack_hitbox = set_attack_hitbox();
    vector2f core(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2);
    SDL_Rect player_hitbox = main_player.get_hitbox();
    SDL_Rect weapon_hitbox = sword.set_attack_hitbox();
    attack = false;

    if(sword.is_attack() && SDL_HasIntersection(&weapon_hitbox, &hitbox) == SDL_TRUE
        && ((state > 0 && state < 4) || (state == 4 && wait == 0)))
    {
        state = 4;
        order = 1;
        wait = 4;
        health_point--;
        if(main_player.get_flip() == SDL_FLIP_NONE)
            direction.x = 1;
        else direction.x = -1;
        direction.y = 1.0f / (1 + rng() % 5);
    }

    if(health_point == 0)
    {
        health_point--;
        state = 5;
        order = 0;
        wait = 8;
    }

    if(state == 2)
    {
        if(max_move.x <= 0.0f && max_move.y <= 0.0f) wait = 0;
        if(SDL_HasIntersection(&hitbox, &player_hitbox) == SDL_TRUE) wait = 0;
    }


    if(wait == 0)
    {
        if(SDL_HasIntersection(&hitbox, &player_hitbox)) // in attack range
            {
                state = 3;
                order = 0;
                wait = 4;
            }
        else
        {
            switch(state)
            {
                case 0:
                    break;
                case 1:
                {
                    state = 2;
                    order = 0;
                    wait = 8 + rng() % 4;

                    vector2f new_target = main_player.get_pos();

                    new_target.x += target.x;
                    new_target.y += target.y;

                    direction.x = (core.x <= new_target.x ? 1 : -1);
                    direction.y = (core.y <= new_target.y ? 1 : -1);

                    max_move.x = abs(core.x - new_target.x);
                    max_move.y = abs(core.y - new_target.y);
                    break;
                }
                case 2:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 3:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 4:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 5:
                    break;
            }
        }
        
    }


    switch(state)
    {
        case 0:
            break;
        case 1:
            set_sprite(vector2f(0, order));
            break;
        case 2:
            set_sprite(vector2f(0, 4 + order));

            if(max_move.x > 0.0f) current_move.x = speed * delta_time;
            else current_move.x = 0;
            if(max_move.y > 0.0f) current_move.y = speed * delta_time;
            else current_move.y = 0;

            max_move.x -= current_move.x;
            max_move.y -= current_move.y;

            move_x(current_move.x * direction.x, get_leg_rect());
            move_y(current_move.y * direction.y, get_leg_rect());

            if(direction.x == 1) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);

            break;
        case 3:
            set_sprite(vector2f(0, 12 + order));
            if(player_hitbox.x > hitbox.x) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);
            attack = true;
            break;
        case 4:
            set_sprite(vector2f(0, 16 + order));
            move_x(speed * 2.0f * delta_time * direction.x, get_leg_rect());
            move_y(speed * 1.5f * delta_time * direction.y, get_leg_rect());
            break;
        case 5:
            set_sprite(vector2f(0, 20 + order));
            break;
            
    }

    if(current_time - last_update > 0.15f)
    {
        last_update = current_time;
        wait--;
        if(order == 3 && state == 5)
            order--;
        order++;
        switch(state)
        {
            case 0: // spawn
                break;
            case 1: // idle
                order %= 4;
                break;
            case 2: // move
                order %= 8;
                break;
            case 3: // attack
                order %= 4;
                break;
            case 4: // hit
                order %= 4;
                break;
            case 5: // death
                order %= 4;
                break;
        }
    }
}

Orc::Orc(vector2f spawn_point):
    Enemy(spawn_point, {0, 0, 96, 64})
{
    health_point = 12;
    speed = 15 + rng() % 3;
    state = 0;
    wait = 0;
    order = 0;
    texture = main_window.load_texture("orc.png");
}

SDL_Rect Orc::set_attack_hitbox()
{
    SDL_Rect current_hitbox = hitbox;
    if(state == 3)
    {
        switch(order)
        {
            case 1:
                if(get_flip() == SDL_FLIP_NONE)
                    current_hitbox = SDL_Rect{23 + (int)pos.x, 13 + (int)pos.y, 40, 18};
                else
                    current_hitbox = SDL_Rect{33 + (int)pos.x, 13 + (int)pos.y, 40, 18};
                break;
            case 2:
                if(get_flip() == SDL_FLIP_NONE)
                    current_hitbox = SDL_Rect{47 + (int)pos.x, 13 + (int)pos.y, 34, 40};
                else
                    current_hitbox = SDL_Rect{15 + (int)pos.x, 13 + (int)pos.y, 34, 40};
                break;
            case 3:
                if(get_flip() == SDL_FLIP_NONE)
                    current_hitbox = SDL_Rect{53 + (int)pos.x, 24 + (int)pos.y, 12, 29};
                else
                    current_hitbox = SDL_Rect{15 + (int)pos.x, 24 + (int)pos.y, 12, 29};
                break;
            case 4:
                if(get_flip() == SDL_FLIP_NONE)
                    current_hitbox = SDL_Rect{53 + (int)pos.x, 30 + (int)pos.y, 12, 23};
                else
                    current_hitbox = SDL_Rect{15 + (int)pos.x, 30 + (int)pos.y, 12, 23};
                break;
        }
    }
    return current_hitbox;
}

SDL_Rect Orc::get_leg_rect()
{
    return SDL_Rect{33 + (int)pos.x, 50 + (int)pos.y, 22, 5};
}

void Orc::update(float current_time, float delta_time)
{
    if(!spawned)
    {
        spawned = true;
        state = 1;
        order = 0;
        wait = 4;
    }
    if(wait == 0 && state == 5)
    {
        death = true;
        return;
    }
    hitbox = SDL_Rect{33 + (int)pos.x, 21 + (int)pos.y, 24, 33};
    real_attack_hitbox = set_attack_hitbox();
    vector2f core(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2);
    SDL_Rect player_hitbox = main_player.get_hitbox();
    SDL_Rect weapon_hitbox = sword.set_attack_hitbox();
    attack = false;

    if(sword.is_attack() && SDL_HasIntersection(&weapon_hitbox, &hitbox) == SDL_TRUE
        && (state < 4 || (state == 4 && wait == 0)))
    {
        state = 4;
        order = 1;
        wait = 4;
        health_point--;
        if(main_player.get_flip() == SDL_FLIP_NONE)
            direction.x = 1;
        else direction.x = -1;
        direction.x /= 4;
        direction.y = 1.0f / (1 + rng() % 5);
    }

    if(health_point == 0)
    {
        health_point--;
        state = 5;
        order = 0;
        wait = 10;
    }

    if(state == 2)
    {
        if(max_move.x <= 0.0f && max_move.y <= 0.0f) wait = 0;
        if(SDL_HasIntersection(&hitbox, &player_hitbox) == SDL_TRUE) wait = 0;
    }


    if(wait == 0)
    {
        if(SDL_HasIntersection(&hitbox, &player_hitbox)) // in attack range
            {
                state = 3;
                order = 0;
                wait = 6;
            }
        else
        {
            switch(state)
            {
                case 0:
                    break;
                case 1:
                {
                    state = 2;
                    order = 0;
                    wait = 8 + rng() % 4;

                    vector2f new_target = main_player.get_pos();

                    new_target.x += target.x;
                    new_target.y += target.y;

                    direction.x = (core.x <= new_target.x ? 1 : -1);
                    direction.y = (core.y <= new_target.y ? 1 : -1);

                    max_move.x = abs(core.x - new_target.x);
                    max_move.y = abs(core.y - new_target.y);
                    break;
                }
                case 2:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 3:
                    state = 1;
                    order = 0;
                    wait = 4 + rng() % 4;
                    break;
                case 4:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 5:
                    break;
            }
        }
        
    }


    switch(state)
    {
        case 0:
            break;
        case 1:
            set_sprite(vector2f(0, order));
            break;
        case 2:
            set_sprite(vector2f(1, order));

            if(max_move.x > 0.0f) current_move.x = speed * delta_time;
            else current_move.x = 0;
            if(max_move.y > 0.0f) current_move.y = speed * delta_time;
            else current_move.y = 0;

            max_move.x -= current_move.x;
            max_move.y -= current_move.y;

            move_x(current_move.x * direction.x, get_leg_rect());
            move_y(current_move.y * direction.y, get_leg_rect());

            if(direction.x == 1) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);

            break;
        case 3:
            set_sprite(vector2f(2, order));
            if(player_hitbox.x > hitbox.x) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);
            attack = (order > 0 && order < 5);
            break;
        case 4:
            set_sprite(vector2f(3, order));
            move_x(speed * 2.0f * delta_time * direction.x, get_leg_rect());
            move_y(speed * 1.5f * delta_time * direction.y, get_leg_rect());
            break;
        case 5:
            set_sprite(vector2f(4, order));
            break;
            
    }

    if(current_time - last_update > 0.15f)
    {
        last_update = current_time;
        wait--;
        if(order == 3 && state == 5)
            order--;
        order++;
        switch(state)
        {
            case 0: // spawn
                break;
            case 1: // idle
                order %= 4;
                break;
            case 2: // move
                order %= 4;
                break;
            case 3: // attack
                order %= 6;
                break;
            case 4: // hit
                order %= 4;
                break;
            case 5: // death
                order %= 4;
                break;
        }
    }
}

Skeleton::Skeleton(vector2f spawn_point):
    Enemy(spawn_point, {0, 0, 32, 32})
{
    health_point = 4;
    speed = 30 + rng() % 3;
    attack = false;
    texture = main_window.load_texture("skeleton.png");
    spawned = true;
    state = order = 0;
    wait = 15;
}

SDL_Rect Skeleton::set_attack_hitbox()
{
    return SDL_Rect{11 + (int)pos.x, 9 + (int)pos.y, 10, 15};
}

SDL_Rect Skeleton::get_leg_rect()
{
    return SDL_Rect{13 + (int)pos.x, 22 + (int)pos.y, 6, 2};
}

void Skeleton::update(float current_time, float delta_time)
{
    if(wait == 0 && state == 4)
    {
        death = true;
        return;
    }
    hitbox = real_attack_hitbox = set_attack_hitbox();
    vector2f core(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2);
    SDL_Rect player_hitbox = main_player.get_hitbox();
    SDL_Rect weapon_hitbox = sword.set_attack_hitbox();


    if(sword.is_attack() && SDL_HasIntersection(&weapon_hitbox, &hitbox) == SDL_TRUE
        && ((state > 0 && state < 3) || (state == 3 && wait == 0)))
    {
        state = 3;
        order = 1;
        wait = 4;
        health_point--;
        if(main_player.get_flip() == SDL_FLIP_NONE)
            direction.x = 1;
        else direction.x = -1;
        direction.y = 1.0f / (1 + rng() % 5);
    }

    if(health_point == 0)
    {
        health_point--;
        state = 4;
        order = 0;
        wait = 8;
    }

    if(state == 2)
    {
        if(max_move.x <= 0.0f && max_move.y <= 0.0f) wait = 0;
        if(SDL_HasIntersection(&hitbox, &player_hitbox) == SDL_TRUE) wait = 0;
    }


    if(wait == 0)
    {
        if(SDL_HasIntersection(&hitbox, &player_hitbox)) // in attack range
            {
                state = 1;
                order = 0;
                wait = 4 + rng() % 5;
            }
        else
        {
            switch(state)
            {
                case 0:
                    state = 1;
                    order = 0;
                    wait = rng() % 5;
                    break;
                case 1:
                {
                    state = 2;
                    order = 0;
                    wait = 8 + rng() % 4;

                    vector2f new_target = main_player.get_pos();

                    new_target.x += target.x;
                    new_target.y += target.y;

                    direction.x = (core.x <= new_target.x ? 1 : -1);
                    direction.y = (core.y <= new_target.y ? 1 : -1);

                    max_move.x = abs(core.x - new_target.x);
                    max_move.y = abs(core.y - new_target.y);
                    break;
                }
                case 2:
                    state = 1;
                    order = 0;
                    wait = 4 + rng() % 4;
                    attack = false;
                    break;
                case 3:
                    state = 1;
                    order = 0;
                    wait = 1 + rng() % 4;
                    break;
                case 4:
                    break;
            }
        }
        
    }

    

    switch(state)
    {
        case 0:
            set_sprite(vector2f(state, order / 3));
            break;
        case 1:
            set_sprite(vector2f(state, order));
            break;
        case 2:
            set_sprite(vector2f(state, order));
            if(max_move.x > 0.0f) current_move.x = speed * delta_time;
            else current_move.x = 0;
            if(max_move.y > 0.0f) current_move.y = speed * delta_time;
            else current_move.y = 0;

            max_move.x -= current_move.x;
            max_move.y -= current_move.y;

            move_x(current_move.x * direction.x, get_leg_rect());
            move_y(current_move.y * direction.y, get_leg_rect());

            if(direction.x == 1) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);
            attack = true;

            break;
        case 3:
            set_sprite(vector2f(state, order));
            move_x(speed * 2.0f * delta_time * direction.x, get_leg_rect());
            move_y(speed * 1.5f * delta_time * direction.y, get_leg_rect());
            break;
        case 4:
            set_sprite(vector2f(state, order));
            break;
            
    }

    if(current_time - last_update > 0.1f)
    {
        last_update = current_time;
        wait--;
        if(order == 3 && state == 4)
            order--;
        order++;
        switch(state)
        {
            case 0: // spawn
                break;
            case 1: // idle
                order %= 4;
                break;
            case 2: // move
                order %= 4;
                break;
            case 3: // hit
                order %= 4;
                break;
            case 4: // death
                order %= 4;
                break;
        }
    }
}


Projectile::Projectile(vector2f spawn_point, int type, vector <Enemy*> &enemies):
    Enemy(spawn_point, {0, 0, 32, 32})
{
    texture = main_window.load_texture("projectile.png");
    state = type;
    attack = false;
    spawned = true;
    order = 0;
    speed = 50;
    switch(type)
    {
        case 0: //cast summon
            wait = 9;
            break;
        case 1: // cast fire
            wait = 9;
            break;
        case 2: // throw fire
            wait = 999;
            break;
        case 3: // summon -> pos = +0, +7 skeleton
            enemies.emplace_back(new
                Skeleton(vector2f(spawn_point.x, spawn_point.y - 7)));
            wait = 15;
            break;
    }
}

float Projectile::get_y()
{
    if(state == 3) return pos.y;
    return 9999;
}

void Projectile::set_death()
{
    death = true;
}

void Projectile::update(float current_time, float delta_time)
{
    hitbox = real_attack_hitbox =
        SDL_Rect{(int)pos.x + 13, (int)pos.y + 13, 6, 6};
    vector2f core(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2);
    SDL_Rect player_hitbox = main_player.get_hitbox();
    SDL_Rect weapon_hitbox = sword.set_attack_hitbox();

    if(wait == 0)
    {
        if(state == 1)
        {
            state = 2;
            wait = 50;
            order = 0;
            target.x = player_hitbox.x + player_hitbox.w / 2;
            target.y = player_hitbox.y + player_hitbox.h / 2;
            roll = get_angle_degree(-1, 0, target.x - core.x, target.y - core.y);
            if(target.x >= core.x) direction.x = 1;
            else direction.x = -1;
            if(target.y >= core.y) direction.y = 1;
            else direction.y = -1;
            speedx = speed * cos(abs(get_angle_radian(direction.x, 0,  target.x - core.x, target.y - core.y)));
            speedy = speed * cos(abs(get_angle_radian(0, direction.y,  target.x - core.x, target.y - core.y)));
        }
        else
        {
            death = true;
            return;
        }
    }

    if(state == 2)
    {
        if(sword.is_attack() && SDL_HasIntersection(&weapon_hitbox, &hitbox) == SDL_TRUE)
        {
            death = true;
            return;
        }
    }

    switch(state)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            move_x(speedx * delta_time * direction.x, SDL_Rect{0, 0, 0, 0});
            move_y(speedy * delta_time * direction.y, SDL_Rect{0, 0, 0, 0});
            attack = true;
            break;
        case 3:
            break;
    }

    set_sprite(vector2f(state, order));
    if(current_time - last_update > 0.1f)
    {
        last_update = current_time;
        wait--;
        order++;
        if(state == 2) order %= 8;
    }
}

Neucromancer::Neucromancer(vector2f spawn_point):
    Enemy(spawn_point, {0, 0, 48, 48})
{
    health_point = 20;
    speed = 15;
    state = 0;
    wait = 0;
    order = 0;
    last_update = last_summon = last_fire = 0;
    texture = main_window.load_texture("neucro.png");
}

SDL_Rect Neucromancer::get_leg_rect()
{
    return SDL_Rect{16 + (int)pos.x, 35 + (int)pos.y, 12, 2};
}

void Neucromancer::update(float current_time, float delta_time,
    vector <Enemy*> &enemies)
{
    if(wait == 0 && state == 4)
    {
        death = true;
        return;
    }
    hitbox = SDL_Rect{18 + (int)pos.x, 21 + (int)pos.y, 10, 14};
    real_attack_hitbox = SDL_Rect{0, 0, 0, 0};
    vector2f core(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2);
    SDL_Rect player_hitbox = main_player.get_hitbox();
    SDL_Rect weapon_hitbox = sword.set_attack_hitbox();
    attack = false;

    if(!spawned)
    {
        if( hitbox.x >= camera.x &&
        hitbox.y >= camera.y &&
        hitbox.x + hitbox.w <= camera.x + camera.w &&
        hitbox.y + hitbox.h <= camera.y + camera.h)
        {
            spawned = true;
            state = 1;
            order = 0;
            wait = 4;
        }
        else return;
    }

    if(sword.is_attack() && SDL_HasIntersection(&weapon_hitbox, &hitbox) == SDL_TRUE
        && ((state != 3 && state != 4) || (state == 3 && wait == 0)))
    {
        if(state == 5) last_summon = current_time;
        if(state == 6) last_fire = current_time;
        state = 3;
        order = 1;
        wait = 4;
        health_point--;
        if(main_player.get_flip() == SDL_FLIP_NONE)
            direction.x = 1;
        else direction.x = -1;
        direction.y = 1.0f / (1 + rng() % 5);
    }

    if(health_point == 0)
    {
        health_point--;
        state = 4;
        order = 0;
        wait = 8;
    }


    if(wait == 0)
    {
        if(current_time - last_summon > 16)
        {
            wait = 10;
            state = 5;
            order = 0;
        }
        else
        if(current_time - last_fire > 7)
        {
            wait = 1 + 9 * 6;
            state = 6;
            order = 0;
        }
        else
        {
            switch(state)
            {
                case 0:
                    break;
                case 1:
                {
                    state = 2;
                    order = 0;
                    wait = 8 + rng() % 4;

                    direction.x = (rng() % 2 ? 1 : -1);
                    direction.y = (rng() % 2 ? 1 : -1);
                    break;
                }
                case 2:
                    state = 1;
                    order = 0;
                    wait = 3 + rng() % 4;
                    break;
                case 3:
                    state = 1;
                    order = 0;
                    wait = 3 + rng() % 4;
                    break;
                case 4:
                    break;
                case 5:
                    state = 1;
                    order = 0;
                    wait = 3 + rng() % 4;
                    break;
                case 6:
                    state = 1;
                    order = 0;
                    wait = 3 + rng() % 4;
                    break;
            }
        }
        
    }

    if(player_hitbox.x <= hitbox.x) set_flip(SDL_FLIP_HORIZONTAL);
    else set_flip(SDL_FLIP_NONE);
    switch(state)
    {
        case 0:
            break;
        case 1:
            set_sprite(vector2f(0, order));
            break;
        case 2:
            set_sprite(vector2f(1, order));

            current_move.x = speed * delta_time;
            current_move.y = speed * delta_time;

            move_x(current_move.x * direction.x, get_leg_rect());
            move_y(current_move.y * direction.y, get_leg_rect());

            if(direction.x > 0.0f) set_flip(SDL_FLIP_NONE);
            else set_flip(SDL_FLIP_HORIZONTAL);

            break;
        case 3:
            set_sprite(vector2f(2, order));
            move_x(speed * 2.0f * delta_time * direction.x, get_leg_rect());
            move_y(speed * 1.5f * delta_time * direction.y, get_leg_rect());
            break;
        case 4:
            set_sprite(vector2f(3, order));
            break;
        case 5:
            set_sprite(vector2f(4, order));
            break;
        case 6:
            set_sprite(vector2f(4, order));
            break;
    }

    if(current_time - last_update > 0.15f)
    {
        last_update = current_time;
        wait--;
        if(order == 3 && state == 4)
            order--;
        order++;
        switch(state)
        {
            case 0: // spawn
                break;
            case 1: // idle
                order %= 4;
                break;
            case 2: // move
                order %= 4;
                break;
            case 3: // hit
                order %= 4;
                break;
            case 4: // death
                order %= 4;
                break;
            case 5: // summon
                order %= 4;
                if(wait == 9)
                {
                    enemies.emplace_back(new Projectile(vector2f(pos.x + 7, pos.y - 8), 0, enemies));
                    for(int i = 0; i < 3; ++i)
                        enemies.emplace_back(new Projectile(pos_available[rng() % (int)pos_available.size()], 3, enemies));
                }
                if(wait == 0) last_summon = current_time;
                break;
            case 6: // fire
                order %= 4;
                if(wait % 9 == 0 && wait)
                {
                    enemies.emplace_back(new Projectile(vector2f(pos.x + 7, pos.y - 8), 1, enemies));
                }
                if(wait == 0) last_fire = current_time;
                break;
        }
    }
}
