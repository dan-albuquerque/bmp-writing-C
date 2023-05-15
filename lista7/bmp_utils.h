/**
 * File bmp_utils.h
 * @date May 9th, 2023
 * 
 * Implemented according to the following spec: 
 * http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
*/

#ifndef __BMP_UTILS__
#define __BMP_UTILS__

#include <inttypes.h>

/**
 * Defines the BITMAP file attributes
*/
typedef struct __attribute__((packed))  { 
    char bfType[2];           // The characters "BM"
    uint32_t bfSize;          // The size of the file in bytes
    uint16_t bfReserved1;     // Unused - must be zero
    uint16_t bfReserved2;     // Unused - must be zero
    uint32_t brOffbits;       // Offset to start of Pixel Data
} BMPFileHeader;

/**
 * Defines the BITMAP properties (BMP Header)
*/ 
typedef struct __attribute__((packed)) {
    uint32_t biSize;           // Header Size - Must be at least 40
    uint32_t biWidth;          // Image width in pixels
    uint32_t biHeight;         // Image height in pixels
    uint16_t biPlanes;       // Must be 1
    uint16_t biBitCount;     // Bits per pixel - 1, 4, 8, 16, 24, or 32
    uint32_t biCompression;    // Compression type (0 = uncompressed)
    uint32_t biSizeImage;      // Image Size - may be zero for uncompressed images
    uint32_t biXPelsPerMeter;  // Preferred resolution in pixels per meter
    uint32_t biYPelsPerMeter;  // Preferred resolution in pixels per meter
    uint32_t biClrUsed;        // Number Color Map entries that are actually used
    uint32_t biClrImportant;   // Number of significant colors
} BMPHeader;

/**
 * Defines one pixel of BITMAP data for 24 bit format BITMAPs
*/
typedef struct __attribute__((packed)) {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    //uint8_t reserved;
} BMPData;

/**
 * Reads a BMP file and returns the data.
*/
BMPData * bmp_read(char * filename, BMPFileHeader * file_header, BMPHeader * bmp_header); 

//BMP_WRITE:
int bmp_write(char * filename, BMPFileHeader * file_header, BMPHeader * bmp_header, BMPData * bmp_data);

#endif //__BMP_UTILS__