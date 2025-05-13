#include "player.h"
#include "audio.h"
using namespace std;

Player main_player;

Player::Player():
    Entity({73, 53})
{
    texture = main_window.load_texture("player.png");
    if (!texture) {
        printf("Failed to load player.png!\n");
    }
    order = 0;
    state = 0;
    wait = 0;
    last_update = 0;
    health_point = 20;
    speed = 60;
}

void Player::player_render()
{
    SDL_Rect player_sprite = get_sprite();
    SDL_Rect player_pos;
    player_pos.w = player_pos.h = 32;
    player_pos.x = pos.x - camera.x;
    player_pos.y = pos.y - camera.y;
    //SDL_SetRenderDrawColor(main_window.renderer, 255, 255, 0, 255);
    //SDL_RenderDrawRect(main_window.renderer, &player_pos);
    SDL_RenderCopyEx(main_window.renderer, get_tex(), &player_sprite, &player_pos, 0, NULL, get_flip());
}

bool Player::is_death()
{
    return (state == 3);
}

void Player::camera_update()
{
    camera.x = max(pos.x + 16 - CAMERA_WIDTH / 2, 0.0f);
    camera.x = min(camera.x, MAP_WIDTH - camera.w);
    camera.y = max(pos.y + 16 - CAMERA_HEIGHT / 2, 0.0f);
    camera.y = min(camera.y, MAP_HEIGHT- camera.h);
    SDL_SetRenderDrawColor(main_window.renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(main_window.renderer, &camera);
}

SDL_Rect Player::get_leg_rect()
{
    SDL_Rect l_rect;
    l_rect.x = pos.x + 9;
    l_rect.y = pos.y + 26;
    l_rect.w = 13;
    l_rect.h = 3;
    return l_rect;
}

float Player::get_y()
{
    return get_leg_rect().y + get_leg_rect().h;
}

int Player::get_health()
{
    return health_point;
}

void Player::update(float current_time, bool is_attack, float delta_time,
    vector <Enemy*> &enemies)
{
    if(state == 3 && wait == 0) return;
    hitbox.x = (int)pos.x + 8;
    hitbox.y = (int)pos.y + 10;
    hitbox.w = 15;
    hitbox.h = 20;
    const Uint8* key = SDL_GetKeyboardState(NULL);


    if(wait == 0)
    {
        bool is_hit = false;
        for(auto &e: enemies)
            if(e->is_attack())
            {
                SDL_Rect e_hitbox = e->get_attack_hitbox();
                if(SDL_HasIntersection(&hitbox, &e_hitbox))
                {
                    if(Projectile* enemy = dynamic_cast<Projectile*>(e))
                        enemy->set_death();
                    int damage_taken = 2 + random() % 2;
                    health_point = max(health_point - damage_taken, 0);
                    is_hit = true;

                    if(e_hitbox.x >= hitbox.x)
                        direction.x = -1;
                    else direction.x = 1;
                    if(e_hitbox.y >= hitbox.y)
                        direction.y = -1;
                    else direction.y = 1;
                    break;
                }
            }
        if(is_hit)
        {
            order = 0;
            wait = 4;
            if(health_point > 0)
            {
                state = 2;
                wait = 4;
            }
            else
            {
                state = 3;
                wait = 15;
            }
        }
        else
        {
            if(key[SDL_SCANCODE_W] ||
                key[SDL_SCANCODE_S] ||
                key[SDL_SCANCODE_A] ||
                key[SDL_SCANCODE_D])
            {
                state = 1;
                wait = 4;
                sound.play("player/walk");
            }
            else
            {
                state = 0;
                wait = 1;
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
            if(key[SDL_SCANCODE_W])
                move_y(-speed * delta_time, get_leg_rect());
            if(key[SDL_SCANCODE_S])
                move_y(speed * delta_time, get_leg_rect());
            if(key[SDL_SCANCODE_A])
            {
                move_x(-speed * delta_time, get_leg_rect());
                if(!is_attack)
                    set_flip(SDL_FLIP_HORIZONTAL);
            }
            if(key[SDL_SCANCODE_D])
            {
                move_x(speed * delta_time, get_leg_rect());
                if(!is_attack)
                    set_flip(SDL_FLIP_NONE);
            }
            break;
        case 2:
            set_sprite(vector2f(0, 12 + order));
            move_x(speed * delta_time * direction.x * 1.5f, get_leg_rect());
            move_y(speed * delta_time * direction.y * 0.7f, get_leg_rect());
            break;
        case 3:
            set_sprite(vector2f(0, 16 + order));
            break;
    }

    if(current_time - last_update > 0.1f)
    {
        last_update = current_time;
        order++;
        if(state == 3 && order == 4) order--;
        wait--;
        switch(state)
        {
            case 0: // idle
                order %= 4;
                break;
            case 1: // run
                order %= 8;
                break;
            case 2: // hit
                break;
            case 3: // death
                if(wait == 0)
                {
                    current_game_state = 1;
                    fade_state = 1;
                    Mix_PlayMusic(sound.title, -1);
                }
                break;
        }
    }
    camera_update();
}
