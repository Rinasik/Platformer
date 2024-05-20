#pragma once

constexpr int FRAME_FREQUENCY = 50;

constexpr double EPSILON = 0.00001;

constexpr int WIDTH = 20;
constexpr int HEIGHT = 20;

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;

constexpr double DELTA_X = 2.0 / WIDTH;
constexpr double DELTA_Y = 2.0 / HEIGHT;

constexpr double MAX_X_VELOCITY = 0.012;
constexpr double MAX_Y_VELOCITY = 0.6;

constexpr double DELTA_X_VELOCITY = 0.0028;
constexpr double X_ACC = 0.0003;

constexpr double DELTA_Y_VELOCITY = 0.42;
constexpr double Y_ACC = 0.03;

constexpr int HERO_MAX_LIVES = 3;
constexpr int INVISIBLE_TICKS_COUNT = 74;

constexpr double ENEMY_X_VELOCITY = 0.0025;
constexpr double HIT_VELOCITY = 0.005;

constexpr double ARROW_VELOCITY = 0.01;

enum class MapEncoding
{
    Magma = -1,
    Empty = 0,
    Brick = 1,
    BreakingBrick = -2,
    Exit = 100,

    Hero = 2,
    Hit = -100,
    Arrow = -200,
    Bonus = -10,

    Warrior = 3,
    Jumper = 4,
    Archer = 5,
    Monster = 6,
    Boss = 10,

    Platform = 7,

    Chest = 8,
    Box = 9,
};

enum class Direction
{
    Up = 0,
    Left = 1,
    Right = 2
};

enum class BonusType
{
    MaxLives = 1,
    OneLife = 2,
    ThreeLives = 3,
    DoubleJump = 4,
    Key = 5,
    BigHit = 6,
    Bow = 7,
    Arrows = 8
};

enum class Weapon
{
    Hit = 1,
    Bow = 2,
};