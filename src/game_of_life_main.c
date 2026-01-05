#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "game_of_life.h"
#include "pixelbuffer.h"

int main()
{
    game_loop(10, 400, 400);
    return 0;
}
