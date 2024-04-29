#pragma once

constexpr int FRAME_FREQUENCY = 50;

constexpr double EPSILON = 0.00001;

constexpr int WIDTH = 20;
constexpr int HEIGHT = 20;

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;

constexpr double DELTA_X = 2.0 / WIDTH;
constexpr double DELTA_Y = 2.0 / HEIGHT;

constexpr double MAX_X_VELOCITY = 0.25;
constexpr double MAX_Y_VELOCITY = 0.6;

constexpr double DELTA_X_VELOCITY = 0.0015;
constexpr double X_ACC = 0.00025;

constexpr double DELTA_Y_VELOCITY = 0.52;
constexpr double Y_ACC = 0.04;

constexpr int HERO_MAX_LIVES = 3;
constexpr int INVISIBLE_TICKS_COUNT = 150;

enum class MapEncoding
{
    Empty = 0,
    Brick = 1,
    Hero = 2,
    Enemy = 3,
    Platform = 4,
    Exit = 100,
    Magma = -1,
};