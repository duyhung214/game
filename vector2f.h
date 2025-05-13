#ifndef vector2f_HPP
#define vector2f_HPP

#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <math.h>
#include <iomanip>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 608;
const int MAP_WIDTH = 1200;
const int MAP_HEIGHT = 1200;
const int CAMERA_WIDTH = 200;
const int CAMERA_HEIGHT = 152;
const std::string WINDOW_NAME = "pxokq's screen";
const int FPS = 100;
const float FRAME_DELAY = 1000.0f / FPS;

extern SDL_Rect camera;
extern std::vector<SDL_Rect> map_tiles;
const double pi = 3.14159;

struct vector2f
{
    vector2f(float p_x = 0.0f, float p_y = 0.0f) : x(p_x), y(p_y) {}
    float x, y;
};

inline mt19937 rng(static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()));
inline int fade_state, current_opacity, current_game_state;

inline double get_angle_degree(double x1, double y1, double x2, double y2)
{
    double dot = x1*x2 + y1*y2;
    double det = x1*y2 - y1*x2;
    double angle = atan2(det, dot) * 180 / pi;
    return angle;
}

inline double get_angle_radian(double x1, double y1, double x2, double y2)
{
    double dot = x1*x2 + y1*y2;
    double det = x1*y2 - y1*x2;
    double angle = atan2(det, dot);
    return angle;
}

#endif
