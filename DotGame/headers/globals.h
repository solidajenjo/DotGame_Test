#pragma once
#define SCREEN_WIDTH 650
#define HORIZONTAL_PADDING 5
#define SCREEN_HEIGHT 709
#define VERTICAL_PADDING 64 + HORIZONTAL_PADDING
#define HORIZONTAL_CELLS 10
#define VERTICAL_CELLS 10

#define RAND_FLOAT ((float) rand() / (float) RAND_MAX)
#define DOT_DURATION 2.0f
#define SPLASH_SCREEN_DURATION 5.0f

#define LERP(a, b, f) (float(a) * (1.0 - f)) + (float(b) * f)

