#include <stdint.h>
#include <stdlib.h>
#include "pixelbuffer.h"
#include <string.h>

PixelBuf *create_pixelbuf(size_t width, size_t height)
{
    PixelBuf *pb = malloc(sizeof(PixelBuf));
    if (!pb) return NULL;

    pb->width = width;
    pb->height = height;
    pb->pixels = malloc(width*height*sizeof(Color));
    if (!pb->pixels) {
        free(pb);
        return NULL;
    }
    return pb;
}

void free_pixelbuf(PixelBuf *pb)
{
    if (!pb) return;
    free(pb->pixels);
    free(pb);
}

void fill_pixels(PixelBuf *pb, Color color)
{
    for (int y = 0; y < pb->height; ++y) {
        for (int x = 0; x < pb->width; ++x) {
            pb->pixels[y * pb->width + x] = color;
        }
    }
}
