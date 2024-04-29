#pragma once

const int FRAME_FREQUENCY = 50;

const double EPSILON = 0.00001;

const int WIDTH = 20;
const int HEIGHT = 20;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

const double DELTA_X = 2.0 / WIDTH;
const double DELTA_Y = 2.0 / HEIGHT;

const double MAX_X_VELOCITY = 0.25;
const double MAX_Y_VELOCITY = 0.6;

const double DELTA_X_VELOCITY = 0.0015;
const double X_ACC = 0.00025;

const double DELTA_Y_VELOCITY = 0.52;
const double Y_ACC = 0.04;

const int HERO_MAX_LIVES = 3;

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