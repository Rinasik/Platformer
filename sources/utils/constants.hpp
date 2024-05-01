#pragma once

constexpr int FRAME_FREQUENCY = 50;

constexpr double EPSILON = 0.00001;

constexpr int WIDTH = 20;
constexpr int HEIGHT = 20;

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;

constexpr double DELTA_X = 2.0 / WIDTH;
constexpr double DELTA_Y = 2.0 / HEIGHT;

constexpr double MAX_X_VELOCITY = 0.3;
constexpr double MAX_Y_VELOCITY = 0.6;

constexpr double DELTA_X_VELOCITY = 0.002;
constexpr double X_ACC = 0.00025;

constexpr double DELTA_Y_VELOCITY = 0.52;
constexpr double Y_ACC = 0.04;

constexpr int HERO_MAX_LIVES = 3;
constexpr int INVISIBLE_TICKS_COUNT = 74;

constexpr double ENEMY_X_VELOCITY = 0.0025;
constexpr double HIT_VELOCITY = 0.005;

enum class MapEncoding
{
    Magma = -1,
    Empty = 0,
    Brick = 1,
    Exit = 100,

    Hero = 2,
    Hit = -100,

    Warrior = 3,
    Jumper = 4,
    Archer = 5,
    Monster = 6,

    Platform = 7,
};

enum class Direction
{
    Up = 0,
    Left = 1,
    Right = 2
};