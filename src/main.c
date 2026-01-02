#include <stdio.h>
#include "raylib.h"
#include "pixelbuffer.h"

int main()
{
    const int screen_width = 800;
    const int screen_height = 450;

    PixelBuf* pixelbuffer[2] = {0};
    pixelbuffer[0] = create_pixelbuf(screen_width, screen_height);
    pixelbuffer[1] = create_pixelbuf(screen_width, screen_height);

    fill_pixels(pixelbuffer[0], GREEN);
    fill_pixels(pixelbuffer[1], BLUE);

    InitWindow(screen_width, screen_height, "Game of Life");

    SetTargetFPS(1);

    size_t pix_buf_idx = 0;
    while(!WindowShouldClose())
    {
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
