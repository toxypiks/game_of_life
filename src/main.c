#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "pixelbuffer.h"

unsigned int val(Color color){
    if(ColorToInt(color) == ColorToInt(WHITE)) {
        return 1;
    }
    else return 0;
}

void initialize_pixelbuf(PixelBuf* pixbuf, float threshold)
{
    for (int y = 1; y < pixbuf->height - 1; ++y) {
        for (int x = 1; x < pixbuf->width - 1; ++x) {
            // time as seed
            float rand_number = rand() / (float) RAND_MAX;
            pixbuf->pixels[y * pixbuf->width + x] = (rand_number > threshold) ? BLACK : WHITE;
        }
    }
}

Color is_alive(PixelBuf* old_pixbuf, size_t x, size_t y)
{
    size_t w = old_pixbuf->width;
    unsigned int sum =   val(old_pixbuf->pixels[(y - 1)*w + (x - 1)])
              + val(old_pixbuf->pixels[(y - 1)*w + x])
              + val(old_pixbuf->pixels[(y - 1)*w + (x + 1)])
              + val(old_pixbuf->pixels[ y*w      + (x - 1)])
              + val(old_pixbuf->pixels[ y*w      + (x + 1)])
              + val(old_pixbuf->pixels[(y + 1)*w + (x - 1)])
              + val(old_pixbuf->pixels[(y + 1)*w + x])
              + val(old_pixbuf->pixels[(y + 1)*w + (x + 1)]);

    if(sum == 2){
        return old_pixbuf->pixels[y*w + x]; // old value
    } else if (sum == 3){
        return WHITE;
    } else {
        return BLACK;
    }
}

void set_cell_states(PixelBuf* old_pixbuf, PixelBuf* new_pixbuf)
{
    for (size_t y = 1; y < old_pixbuf->height - 1; ++y) {
        for (size_t x = 1; x < old_pixbuf->width - 1; ++x) {
            new_pixbuf->pixels[y*new_pixbuf->width + x] = is_alive(old_pixbuf, x, y);
        }
    }
}

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
