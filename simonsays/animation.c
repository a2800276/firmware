#include "animation.h"

typedef void (*AnimFunc)(void);

void animfunc_fadedark(void);
void animfunc_fadedark_down(void);


static uint8_t* bitmap = NULL;
static uint8_t width = 0;
static uint8_t height = 0;

static uint8_t fadespeed = 2;

AnimFunc anim_func = NULL;

void anim_init(uint8_t in_width, uint8_t in_height, uint8_t* in_bitmap) {
	width = in_width;
	height = in_height;
	bitmap = in_bitmap;
	anim_func = NULL;
}

void anim_tick() {
	if (anim_func) {
		anim_func();
	}
}

void anim_fill_color(uint8_t r, uint8_t g, uint8_t b) {
	int i;
	for (i = 0; i < width * height; i++) {
		bitmap[3*i  ] = r;
		bitmap[3*i+1] = g;
		bitmap[3*i+2] = b;
	}
}

void anim_set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	if (x >= width) return;
	if (y >= height) return;
	bitmap[3*(width*y+x)  ] = r;
	bitmap[3*(width*y+x)+1] = g;
	bitmap[3*(width*y+x)+2] = b;
}

void anim_set_matrix(const uint8_t* buf) {
	memcpy(bitmap, buf, 3*width*height);
}

void animfunc_fadedark() {
	int i;
	for (i = 0; i < width * height; i++) {
		uint8_t r = bitmap[3*i  ];
		uint8_t g = bitmap[3*i+1];
		uint8_t b = bitmap[3*i+2];

		r = (r>fadespeed) ? r-fadespeed : 0;
		g = (g>fadespeed) ? g-fadespeed : 0;
		b = (b>fadespeed) ? b-fadespeed : 0;

		bitmap[3*i  ] = r;
		bitmap[3*i+1] = g;
		bitmap[3*i+2] = b;
	}
}

void animfunc_fadedark_down(void) {
	int x;
	int y;
	for (y=0; y<height; y++) {
		for (x=0; x<width; x++) {
			uint16_t idx = 3 * (y * width + x);
			uint8_t r = bitmap[idx  ];
			uint8_t g = bitmap[idx+1];
			uint8_t b = bitmap[idx+2];

			uint8_t r2 = (y>0) ? bitmap[idx-3*width  ] : 0;
			uint8_t g2 = (y>0) ? bitmap[idx-3*width+1] : 0;
			uint8_t b2 = (y>0) ? bitmap[idx-3*width+2] : 0;
			
			r = (7*r+r2) / 8;
			g = (7*g+g2) / 8;
			b = (7*b+b2) / 8;

			r = (r>fadespeed) ? r-fadespeed : 0;
			g = (g>fadespeed) ? g-fadespeed : 0;
			b = (b>fadespeed) ? b-fadespeed : 0;

			bitmap[idx  ] = r;
			bitmap[idx+1] = g;
			bitmap[idx+2] = b;
		}
	}
}


