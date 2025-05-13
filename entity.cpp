#include "entity.h"
#include "player.h"

using namespace std;

Entity::Entity(vector2f new_pos, SDL_Texture* new_texture, int new_x, int new_y, int new_w, int new_h):
    pos(new_pos), texture(new_texture)
{
    roll = 0;
    sprite.x = new_x;
    sprite.y = new_y;
    sprite.w = new_w;
    sprite.h = new_h;
    flip = SDL_FLIP_NONE;
}

Entity::Entity(vector2f new_pos, SDL_Texture* new_texture, SDL_Rect new_sprite):
    pos(new_pos), texture(new_texture), sprite(new_sprite)
{
    roll = 0;
    flip = SDL_FLIP_NONE;
}

SDL_Texture* Entity::get_tex()
{
    return texture;
}

SDL_RendererFlip Entity::get_flip()
{
    return flip;
}

SDL_Rect Entity::get_rect()
{
    SDL_Rect real_pos;
    real_pos.x = pos.x + 9;
    real_pos.y = pos.y + 26;
    real_pos.w = 13;
    real_pos.h = 3;
    return real_pos;
}

SDL_Rect Entity::get_sprite()
{
    return sprite;
}

vector2f Entity::get_pos()
{
    return pos;
}

SDL_Rect Entity::get_hitbox()
{
    return hitbox;
}

float Entity::get_y()
{
    return pos.y + sprite.h;
}

void Entity::set_flip(SDL_RendererFlip new_flip)
{
    flip = new_flip;
}

void Entity::set_sprite(vector2f new_pos)
{
    sprite.x = new_pos.y * sprite.w;
    sprite.y = new_pos.x * sprite.h;
}

bool Entity::collision_detection(SDL_Rect x, vector<SDL_Rect>& y)
{
    for(int i = 0; i < (int)y.size(); ++i)
        if(SDL_HasIntersection(&x, &y[i]) == SDL_TRUE)
            return true;
    return false;
}

void Entity::move_x(float speed, SDL_Rect l_rect)
{
    pos.x += speed;
    if(round(speed) == 0.0f)
        l_rect.x += (speed > 0.0f ? 1 : -1);
    else l_rect.x += round(speed);
    if(collision_detection(l_rect, map_tiles))
    {
        pos.x -= speed;
    }
}

void Entity::move_y(float speed, SDL_Rect l_rect)
{
    pos.y += speed;
    if(round(speed) == 0.0f)
        l_rect.y += (speed > 0.0f ? 1 : -1);
    else l_rect.y += round(speed);
    if(collision_detection(l_rect, map_tiles))
    {
        pos.y -= speed;
        //cout << "collision detected on y-axis " << l_rect.y << endl;
    }
    SDL_SetRenderDrawColor(main_window.renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(main_window.renderer, &l_rect);
}
