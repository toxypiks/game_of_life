#ifndef PIXELBUFFER_H_
#define PIXELBUFFER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "raylib.h"

/**
 * @defgroup pixelbuffer PixelBuf
 * @brief public struct and functions for Pixelbuffer
 * @{
 */

typedef int Errno;

#define return_defer(value) do {result = (value); goto defer; } while (0)

/**
 * @brief saves width and height of RGBA data
 */

typedef struct {
    size_t width;
    size_t height;
    Color *pixels;
} PixelBuf;

/**
 * @brief creates Pixelbuffer
 * @param width
 * @param height
 * @return PixelBuf
 */
PixelBuf *create_pixelbuf(size_t width, size_t height);

/**
 * @brief frees Pixelbuffer data
 * @param PixelBuf
 */
void free_pixelbuf(PixelBuf *pb);

/**
 * @brief fills Pixelbuffer with RGBA data
 * @param PixelBuf
 * @param RGBA color
 */
void fill_pixels(PixelBuf *pb, Color color);

#endif // PIXELBUFFER_H_
