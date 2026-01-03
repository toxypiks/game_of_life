#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "pixelbuffer.h"

int v(Color color){
    if(ColorToInt(color) == ColorToInt(WHITE)) {
        return 1;
    }
    else 0;
}

void initialize_pixelbuf(PixelBuf* pixbuf, float threshold)
{
    for (size_t i = 0; i < (pixbuf->width * pixbuf->height); i++){
        float rand_number = rand() / (float) RAND_MAX;
        pixbuf->pixels[i] = (rand_number > threshold) ? BLACK : WHITE;
    }
}

// check if alive
// calc_cell_state
Color is_alive(PixelBuf* old_pixbuf, size_t x, size_t y)
{
    //TODO: big if statement for neighbors
    // if -> return WHITE
    size_t w = old_pixbuf->width;
    int sum =   v(old_pixbuf->pixels[(y - 1)*w + (x - 1)])
              + v(old_pixbuf->pixels[(y - 1)*w + x])
              + v(old_pixbuf->pixels[(y - 1)*w + (x -+1)])
              + v(old_pixbuf->pixels[ y*w      + (x - 1)])
              + v(old_pixbuf->pixels[ y*w      + (x + 1)])
              + v(old_pixbuf->pixels[(y + 1)*w + (x - 1)])
              + v(old_pixbuf->pixels[(y + 1)*w + x])
              + v(old_pixbuf->pixels[(y + 1)*w + (x - 1)]);
    if((sum == 2) || (sum == 3) )
      return WHITE;
    return BLACK;
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
    initialize_pixelbuf(pixelbuffer[0], 0.2f);
    while(!WindowShouldClose())
    {
        // generate new state
        // for
        //   for
        //       is_alive(i,j)
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
