#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

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

    // zoom stuff:
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int zoomMode = 1;

    while(!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE)) zoomMode = 0;
        else if (IsKeyPressed(KEY_TWO)) zoomMode = 1;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        if (zoomMode == 0)
        {
            // Zoom based on mouse wheel
            float wheel = GetMouseWheelMove();
            if (wheel != 0)
            {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.offset = GetMousePosition();
                camera.target = mouseWorldPos;
                float scale = 0.2f*wheel;
                camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
            }
        } else { // righclick zoom
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.offset = GetMousePosition();
                camera.target = mouseWorldPos;
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                float deltaX = GetMouseDelta().x;
                float scale = 0.005f*deltaX;
                camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
            }
        }

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
        BeginMode2D(camera);

        ClearBackground(DARKBLUE);
        DrawTexture(texture, 0, 0, WHITE);
        ClearBackground(BLACK);

        EndMode2D();
        EndDrawing();

        UnloadTexture(texture);
        pix_buf_idx = (pix_buf_idx == 1) ? 0 : 1;
    }

    CloseWindow();

    return 0;
}
