#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "everykey/everykey.h"

void anim_init(uint8_t width, uint8_t height, uint8_t* bitmap);

void anim_tick();

void anim_fill_color(uint8_t r, uint8_t g, uint8_t b);

void anim_set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);

void anim_set_matrix(const uint8_t* buf);


#endif