#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "game_of_life.h"
#include "pixelbuffer.h"

int main()
{
    const int screen_width = 400;
    const int screen_height = 400;

    PixelBuf* pixelbuffer[2] = {0};
    pixelbuffer[0] = create_pixelbuf(screen_width, screen_height);
    pixelbuffer[1] = create_pixelbuf(screen_width, screen_height);

    fill_pixels(pixelbuffer[0], BLACK);
    fill_pixels(pixelbuffer[1], BLACK);

    InitWindow(screen_width, screen_height, "Game of Life");

    SetTargetFPS(10);

    size_t pix_buf_idx = 0;
    initialize_pixelbuf(pixelbuffer[0], 0.3f);
    while(!WindowShouldClose())
    {
        set_cell_states(pixelbuffer[pix_buf_idx], pixelbuffer[(pix_buf_idx == 0)]);

        Image img = {
            .data = pixelbuffer[pix_buf_idx]->pixels,
            .width = screen_width,
            .height = screen_height,
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
            .mipmaps = 1
        };

        // TODO: use update texture
        Texture2D texture = LoadTextureFromImage(img);

        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTexture(texture, 0, 0, WHITE);
        ClearBackground(BLACK);

        EndDrawing();
        UnloadTexture(texture);
        pix_buf_idx = (pix_buf_idx == 1) ? 0 : 1;
    }

    CloseWindow();

    return 0;
}
