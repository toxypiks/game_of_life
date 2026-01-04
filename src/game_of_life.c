#include "game_of_life.h"

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
