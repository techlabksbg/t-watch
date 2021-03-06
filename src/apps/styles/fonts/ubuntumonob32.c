/*******************************************************************************
 * Size: 32 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef UBUNTUMONOB32
#define UBUNTUMONOB32 1
#endif

#if UBUNTUMONOB32

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002D "-" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x6f, 0xff, 0xff,
    0xff, 0xf6, 0x6f, 0xff, 0xff, 0xff, 0xf6, 0x6f,
    0xff, 0xff, 0xff, 0xf6, 0x6f, 0xff, 0xff, 0xff,
    0xf6,

    /* U+002E "." */
    0x6, 0xcd, 0x70, 0x6f, 0xff, 0xf7, 0xbf, 0xff,
    0xfc, 0xcf, 0xff, 0xfc, 0x7f, 0xff, 0xf8, 0x8,
    0xee, 0x90,

    /* U+0030 "0" */
    0x0, 0x0, 0x7d, 0xff, 0xd8, 0x10, 0x0, 0x0,
    0x2e, 0xff, 0xff, 0xff, 0xe3, 0x0, 0x1, 0xef,
    0xff, 0xff, 0xff, 0xfe, 0x10, 0x9, 0xff, 0xfe,
    0x99, 0xef, 0xff, 0x90, 0xf, 0xff, 0xe2, 0x0,
    0x2e, 0xff, 0xf0, 0x5f, 0xff, 0x70, 0x0, 0x7,
    0xff, 0xf5, 0x8f, 0xff, 0x10, 0x0, 0x1, 0xff,
    0xf8, 0xbf, 0xfe, 0x0, 0x44, 0x0, 0xef, 0xfa,
    0xcf, 0xfc, 0x6, 0xff, 0x60, 0xcf, 0xfc, 0xdf,
    0xfb, 0xb, 0xff, 0xb0, 0xbf, 0xfd, 0xdf, 0xfa,
    0xa, 0xff, 0xa0, 0xaf, 0xfd, 0xdf, 0xfb, 0x2,
    0xdd, 0x20, 0xbf, 0xfd, 0xcf, 0xfc, 0x0, 0x0,
    0x0, 0xcf, 0xfc, 0xaf, 0xfe, 0x0, 0x0, 0x0,
    0xef, 0xfa, 0x8f, 0xff, 0x10, 0x0, 0x1, 0xff,
    0xf8, 0x5f, 0xff, 0x70, 0x0, 0x7, 0xff, 0xf5,
    0xf, 0xff, 0xe2, 0x0, 0x2e, 0xff, 0xf0, 0x9,
    0xff, 0xff, 0x99, 0xff, 0xff, 0x90, 0x1, 0xef,
    0xff, 0xff, 0xff, 0xfe, 0x10, 0x0, 0x3e, 0xff,
    0xff, 0xff, 0xe3, 0x0, 0x0, 0x1, 0x8d, 0xff,
    0xd8, 0x10, 0x0,

    /* U+0031 "1" */
    0x0, 0x0, 0x0, 0x7f, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x8, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x2,
    0xcf, 0xff, 0xfc, 0x0, 0x0, 0x1, 0x9f, 0xff,
    0xff, 0xfc, 0x0, 0x0, 0x4f, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0xb, 0xff, 0xa4, 0xff, 0xfc, 0x0,
    0x0, 0x4, 0x92, 0x3, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x3, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x2,
    0x66, 0x68, 0xff, 0xfd, 0x66, 0x64, 0x5, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfb, 0x5, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfb, 0x5, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfb,

    /* U+0032 "2" */
    0x0, 0x5, 0xae, 0xfe, 0xb6, 0x0, 0x0, 0x4,
    0xdf, 0xff, 0xff, 0xff, 0xd2, 0x0, 0x5f, 0xff,
    0xff, 0xff, 0xff, 0xfd, 0x0, 0x2f, 0xff, 0xea,
    0x9d, 0xff, 0xff, 0x60, 0x6, 0xf8, 0x0, 0x0,
    0xaf, 0xff, 0xb0, 0x0, 0x40, 0x0, 0x0, 0x3f,
    0xff, 0xd0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xff,
    0xd0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0x90,
    0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0x30, 0x0,
    0x0, 0x0, 0x1e, 0xff, 0xfa, 0x0, 0x0, 0x0,
    0x1, 0xdf, 0xff, 0xd1, 0x0, 0x0, 0x0, 0x1d,
    0xff, 0xfe, 0x20, 0x0, 0x0, 0x1, 0xdf, 0xff,
    0xf3, 0x0, 0x0, 0x0, 0xc, 0xff, 0xff, 0x30,
    0x0, 0x0, 0x0, 0x8f, 0xff, 0xf3, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0x50, 0x0, 0x0, 0x0,
    0xa, 0xff, 0xfa, 0x0, 0x0, 0x0, 0x0, 0xe,
    0xff, 0xf9, 0x66, 0x66, 0x66, 0x64, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x2f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfc,

    /* U+0033 "3" */
    0x0, 0x28, 0xde, 0xfe, 0xb6, 0x0, 0x0, 0x1a,
    0xff, 0xff, 0xff, 0xff, 0xd2, 0x0, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xfd, 0x0, 0x8, 0xff, 0xb8,
    0x8c, 0xff, 0xff, 0x40, 0x1, 0x70, 0x0, 0x0,
    0xbf, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x6f,
    0xff, 0x90, 0x0, 0x0, 0x0, 0x0, 0x8f, 0xff,
    0x70, 0x0, 0x0, 0x0, 0x28, 0xff, 0xff, 0x10,
    0x0, 0x3, 0xff, 0xff, 0xff, 0xf5, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xff, 0x90, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x1, 0x66,
    0x8c, 0xff, 0xff, 0xa0, 0x0, 0x0, 0x0, 0x0,
    0x4f, 0xff, 0xf1, 0x0, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xf5, 0x0, 0x0, 0x0, 0x0, 0x9, 0xff,
    0xf6, 0x0, 0x0, 0x0, 0x0, 0xc, 0xff, 0xf5,
    0x6, 0x0, 0x0, 0x0, 0x5f, 0xff, 0xf2, 0x2f,
    0xfb, 0x98, 0x8b, 0xff, 0xff, 0xc0, 0x6f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x20, 0x9f, 0xff, 0xff,
    0xff, 0xff, 0xe3, 0x0, 0x4, 0x8c, 0xef, 0xfe,
    0xa6, 0x0, 0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0x0, 0x0, 0x1d, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xcf, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x9, 0xff, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x5f, 0xff, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x1, 0xef, 0xff, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0xb, 0xff, 0xee, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x6f, 0xff, 0x3e, 0xff, 0xf1, 0x0,
    0x0, 0x1, 0xff, 0xf8, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0xa, 0xff, 0xd0, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0x4f, 0xff, 0x40, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0xdf, 0xfb, 0x0, 0xe, 0xff, 0xf1, 0x0,
    0x6, 0xff, 0xf3, 0x0, 0xe, 0xff, 0xf1, 0x0,
    0xe, 0xff, 0xd5, 0x55, 0x5e, 0xff, 0xf6, 0x51,
    0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0x0, 0x0, 0x0, 0x0, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xe, 0xff, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xe, 0xff, 0xf1, 0x0,

    /* U+0035 "5" */
    0x0, 0x9f, 0xff, 0xff, 0xff, 0xff, 0x90, 0x0,
    0xaf, 0xff, 0xff, 0xff, 0xff, 0x90, 0x0, 0xbf,
    0xff, 0xff, 0xff, 0xff, 0x90, 0x0, 0xcf, 0xfd,
    0x66, 0x66, 0x66, 0x30, 0x0, 0xcf, 0xfa, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xef, 0xf9, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xfa, 0x43, 0x0, 0x0, 0x0,
    0x2, 0xff, 0xff, 0xff, 0xfa, 0x30, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xff, 0xf6, 0x0, 0x3, 0xab,
    0xce, 0xff, 0xff, 0xff, 0x40, 0x0, 0x0, 0x0,
    0x28, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x4f, 0xff, 0xf1, 0x0, 0x0, 0x0, 0x0, 0xd,
    0xff, 0xf3, 0x0, 0x0, 0x0, 0x0, 0xc, 0xff,
    0xf4, 0x0, 0x0, 0x0, 0x0, 0xe, 0xff, 0xf2,
    0x3, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x4f,
    0xda, 0x87, 0x9d, 0xff, 0xff, 0x80, 0x8f, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0xcf, 0xff, 0xff,
    0xff, 0xff, 0xb1, 0x0, 0x16, 0xad, 0xff, 0xed,
    0x93, 0x0, 0x0,

    /* U+0036 "6" */
    0x0, 0x0, 0x0, 0x2, 0x7b, 0xde, 0x60, 0x0,
    0x0, 0x3, 0xbf, 0xff, 0xff, 0x70, 0x0, 0x0,
    0x7f, 0xff, 0xff, 0xff, 0x80, 0x0, 0x8, 0xff,
    0xff, 0xfe, 0xba, 0x50, 0x0, 0x5f, 0xff, 0xfc,
    0x40, 0x0, 0x0, 0x0, 0xef, 0xff, 0x90, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0xd, 0xff, 0xf4, 0x45, 0x40, 0x0, 0x0,
    0x1f, 0xff, 0xff, 0xff, 0xff, 0xa1, 0x0, 0x5f,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x6f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xb0, 0x7f, 0xff, 0x92,
    0x1, 0x7f, 0xff, 0xf2, 0x8f, 0xff, 0x50, 0x0,
    0x9, 0xff, 0xf6, 0x7f, 0xff, 0x60, 0x0, 0x5,
    0xff, 0xf8, 0x6f, 0xff, 0x70, 0x0, 0x5, 0xff,
    0xf8, 0x3f, 0xff, 0xc0, 0x0, 0x8, 0xff, 0xf6,
    0xf, 0xff, 0xf4, 0x0, 0x2e, 0xff, 0xf2, 0x9,
    0xff, 0xff, 0xa9, 0xef, 0xff, 0xc0, 0x1, 0xef,
    0xff, 0xff, 0xff, 0xff, 0x30, 0x0, 0x3e, 0xff,
    0xff, 0xff, 0xf4, 0x0, 0x0, 0x1, 0x7c, 0xef,
    0xd9, 0x20, 0x0,

    /* U+0037 "7" */
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x26, 0x66, 0x66,
    0x66, 0x8f, 0xff, 0xf3, 0x0, 0x0, 0x0, 0x0,
    0xbf, 0xff, 0x80, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0xa, 0xff, 0xf5,
    0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x0, 0x8f, 0xff, 0x60, 0x0, 0x0,
    0x0, 0x0, 0xef, 0xff, 0x10, 0x0, 0x0, 0x0,
    0x4, 0xff, 0xfa, 0x0, 0x0, 0x0, 0x0, 0xa,
    0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xe0, 0x0, 0x0, 0x0, 0x0, 0x4f, 0xff, 0xa0,
    0x0, 0x0, 0x0, 0x0, 0x9f, 0xff, 0x50, 0x0,
    0x0, 0x0, 0x0, 0xdf, 0xff, 0x10, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0,
    0x5, 0xff, 0xfa, 0x0, 0x0, 0x0, 0x0, 0x8,
    0xff, 0xf7, 0x0, 0x0, 0x0, 0x0, 0xa, 0xff,
    0xf6, 0x0, 0x0, 0x0, 0x0, 0xb, 0xff, 0xf4,
    0x0, 0x0, 0x0,

    /* U+0038 "8" */
    0x0, 0x2, 0x9d, 0xff, 0xea, 0x30, 0x0, 0x0,
    0x6f, 0xff, 0xff, 0xff, 0xf9, 0x0, 0x4, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x70, 0xc, 0xff, 0xfc,
    0x77, 0xcf, 0xff, 0xf0, 0x1f, 0xff, 0xc0, 0x0,
    0xc, 0xff, 0xf3, 0x4f, 0xff, 0x70, 0x0, 0x7,
    0xff, 0xf3, 0x3f, 0xff, 0x90, 0x0, 0x9, 0xff,
    0xf1, 0xf, 0xff, 0xf3, 0x0, 0xe, 0xff, 0xc0,
    0x9, 0xff, 0xff, 0x92, 0xbf, 0xff, 0x40, 0x0,
    0xbf, 0xff, 0xff, 0xff, 0xf6, 0x0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xd1, 0x0, 0x3, 0xff, 0xfd,
    0xef, 0xff, 0xfe, 0x30, 0xe, 0xff, 0xd0, 0x5,
    0xdf, 0xff, 0xe0, 0x6f, 0xff, 0x40, 0x0, 0xa,
    0xff, 0xf7, 0xaf, 0xff, 0x0, 0x0, 0x1, 0xff,
    0xfb, 0xcf, 0xff, 0x0, 0x0, 0x0, 0xff, 0xfc,
    0xaf, 0xff, 0x60, 0x0, 0x6, 0xff, 0xfa, 0x6f,
    0xff, 0xfb, 0x77, 0xbf, 0xff, 0xf5, 0xd, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x2, 0xdf, 0xff,
    0xff, 0xff, 0xfb, 0x10, 0x0, 0x5, 0xbe, 0xff,
    0xda, 0x40, 0x0,

    /* U+0039 "9" */
    0x0, 0x1, 0x8d, 0xfe, 0xc7, 0x0, 0x0, 0x0,
    0x4e, 0xff, 0xff, 0xff, 0xe2, 0x0, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x10, 0xb, 0xff, 0xfd,
    0x89, 0xff, 0xff, 0x80, 0x2f, 0xff, 0xd1, 0x0,
    0x3f, 0xff, 0xe0, 0x6f, 0xff, 0x70, 0x0, 0xb,
    0xff, 0xf4, 0x8f, 0xff, 0x50, 0x0, 0x6, 0xff,
    0xf7, 0x8f, 0xff, 0x50, 0x0, 0x4, 0xff, 0xf8,
    0x6f, 0xff, 0x90, 0x0, 0x3, 0xff, 0xf9, 0x2f,
    0xff, 0xf7, 0x10, 0x28, 0xff, 0xf9, 0xb, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf7, 0x2, 0xef, 0xff,
    0xff, 0xff, 0xff, 0xf6, 0x0, 0x1a, 0xff, 0xff,
    0xff, 0xff, 0xf2, 0x0, 0x0, 0x14, 0x54, 0x4f,
    0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0xcf, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x9, 0xff, 0xff, 0x10,
    0x0, 0x0, 0x4, 0xcf, 0xff, 0xf7, 0x0, 0x2,
    0x8b, 0xef, 0xff, 0xff, 0xa0, 0x0, 0x4, 0xff,
    0xff, 0xff, 0xf9, 0x0, 0x0, 0x4, 0xff, 0xff,
    0xfc, 0x40, 0x0, 0x0, 0x3, 0xfd, 0xc8, 0x30,
    0x0, 0x0, 0x0,

    /* U+003A ":" */
    0x8, 0xee, 0x80, 0x7f, 0xff, 0xf8, 0xcf, 0xff,
    0xfc, 0xbf, 0xff, 0xfc, 0x6f, 0xff, 0xf7, 0x7,
    0xdd, 0x70, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xcd,
    0x70, 0x6f, 0xff, 0xf7, 0xbf, 0xff, 0xfc, 0xcf,
    0xff, 0xfc, 0x7f, 0xff, 0xf8, 0x8, 0xee, 0x90
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 10, .box_h = 5, .ofs_x = 3, .ofs_y = 7},
    {.bitmap_index = 25, .adv_w = 256, .box_w = 6, .box_h = 6, .ofs_x = 5, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 337, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 484, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 631, .adv_w = 256, .box_w = 16, .box_h = 21, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 946, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1093, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1240, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1387, .adv_w = 256, .box_w = 14, .box_h = 21, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1534, .adv_w = 256, .box_w = 6, .box_h = 16, .ofs_x = 5, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 0, 2, 3, 4, 5, 6,
    7, 8, 9, 10, 11, 12
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 45, .range_length = 14, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 14, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t ubuntumonob32 = {
#else
lv_font_t ubuntumonob32 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 21,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -4,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if UBUNTUMONOB32*/

