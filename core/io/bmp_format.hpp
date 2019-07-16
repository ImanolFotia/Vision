#pragma once
/**
    The file format consists of the following structures:

    Structure  | Bytes           | Description
    Header     | 0x00 - 0x0D     | contains information about the type, size,
                                   and layout of a device-independent bitmap file
    InfoHeader | 0x0E - 0x35     | specifies the dimensions, compression type, and color format for the bitmap
    ColorTable | 0x36 - variable | contains as many elements as there are colors in the bitmap, but is not present
                                   for bitmaps with 24 color bits because each pixel is represented by 24-bit
                                   red-green-blue (RGB) values in the actual bitmap data area
    Pixel Data | variable        | an array of bytes that defines the bitmap bits.
                                   These are the actual image data, represented by consecutive rows, or
                                   "scan lines," of the bitmap. Each scan line consists of consecutive bytes
                                   representing the pixels in the scan line, in left-to-right order.
                                   The system maps pixels beginning with the bottom scan line of the rectangular
                                   region and ending with the top scan line.
*/

#ifdef __linux__
#include <bits/stdint-uintn.h>
#endif

const char* SIGNATURE = "BM";

const uint32_t BMP_HEADER_SIZE = 14;
const uint32_t BMP_INFOHEADER_SIZE = 40;
const uint32_t BMP_HEADER_OFFSET = 0x00;
const uint32_t BMP_INFOHEADER_OFFSET = 0x0E;

enum BMP_COLORS {
    MONOCHROME = 1,
    P4 = 4,
    P8 = 8,
    RGB16 = 16,
    RGB24 = 24
};

enum BMP_COMPRESSION {
    BI_RGB  = 0,  //no compression
    BI_RLE8 = 1,  //8bit RLE encoding
    BI_RLE4 = 2   //4bit RLE encoding
};

struct bmp_header_t {
    uint16_t signature;
    uint32_t filesize;
    uint32_t reserved;
    uint32_t dataoffset;
};

struct info_header_t {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t xpixelsperm;
    uint32_t ypixelsperm;
    uint32_t colors;
    uint32_t importantcolors;
};

struct color_table_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};