#ifndef GAME_OF_LIFE_H_
#define GAME_OF_LIFE_H_

#include <raylib.h>
#include "pixelbuffer.h"

unsigned int val(Color color);
void initialize_pixelbuf(PixelBuf* pixbuf, float threshold);
Color is_alive(PixelBuf* old_pixbuf, size_t x, size_t y);
void set_cell_states(PixelBuf* old_pixbuf, PixelBuf* new_pixbuf);

#endif // GAME_OF_LIFE_H_
