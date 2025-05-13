#include "window.h"
using namespace std;

Window main_window;

Window::Window():window(NULL), renderer(NULL)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        cout << "Init failed due to an error: " << SDL_GetError() << endl;

    window = SDL_CreateWindow(WINDOW_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
        cout << "Creating window failed: " << SDL_GetError() << endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        cout << "Creating renderer failed: " << SDL_GetError() << endl;
    SDL_RenderSetLogicalSize(renderer, CAMERA_WIDTH, CAMERA_HEIGHT);
}

void Window::before_render()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(renderer);
}

SDL_Texture* Window::load_texture(string path)
{
    SDL_Texture* NewTexture = NULL;
    SDL_Surface* LoadedSurface = IMG_Load(path.c_str());
    if(LoadedSurface == NULL)
        cout << "Loading texture failed: " << IMG_GetError() << endl;
    else
    {
        NewTexture = SDL_CreateTextureFromSurface(renderer, LoadedSurface);
        if(NewTexture == NULL)
            cout << "Creating texture failed: " << SDL_GetError() << endl;
        SDL_FreeSurface(LoadedSurface);
    }
    return NewTexture;
}

void Window::QuitSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::present()
{
    SDL_RenderPresent(renderer);
}

void Window::render_entity(Entity &entity)
{
    SDL_Rect src = entity.get_sprite();
    SDL_Rect dst;
    dst.x = entity.get_pos().x - camera.x;
    dst.y = entity.get_pos().y - camera.y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopyEx(renderer, entity.get_tex(), &src, &dst, entity.roll, NULL, entity.get_flip());
}

void Window::render_fade()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, current_opacity);
    SDL_RenderFillRect(renderer, NULL);
    int speed = 5;
    switch(fade_state)
    {
        case 1: // increase opacity
            current_opacity = min(current_opacity + speed, 255);
            if(current_opacity == 255) fade_state = 2;
            break;
        case 2: // decrease opacity
            current_opacity = max(current_opacity - speed, 0);
            if(current_opacity == 0) fade_state = 0;
            break;
    }
}
