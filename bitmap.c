#include "bitmap.h"
#include <stdlib.h>

/* 
 * Private interface
 */ 

typedef struct                       /**** BMP file header structure ****/
{
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} Header;

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} Info;

static void error(const char *message)
{
    printf("Error: %s\n", message);
    exit(1);
}

static void check(size_t isone)
{
    if (!isone) 
        error("Could not read header from bitmap file.");
}

/* Compute the number of remaining bytes in a file from the current position 
    of the file-index to the end-of-file. Original file-index position is preserved. */
static long flength(FILE *fp)
{
    long length = -1L;
    long pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, pos, SEEK_SET);

    return length - pos;
}

/* This implementation assumes little-endian byte order. */
static Header get_header(FILE *fp)
{
    Header header;

    check(fread(&header.bfType, sizeof(header.bfType), 1, fp));
    check(fread(&header.bfSize, sizeof(header.bfSize), 1, fp));
    check(fread(&header.bfReserved1, sizeof(header.bfReserved1), 1, fp));
    check(fread(&header.bfReserved2, sizeof(header.bfReserved2), 1, fp));
    check(fread(&header.bfOffBits, sizeof(header.bfOffBits), 1, fp));

    return header;
}

/* This implementation assumes little-endian byte order, and byte-alignment
    not more than 4-bytes. */
static Info get_info(FILE *fp)
{
    Info info;
    check(fread(&info, sizeof(info), 1, fp));
    return info;
}

/* 
 * Implementations for public interface
 */ 

/* Checks the header information of a bitmap file. Terminates the program if 
 *  the file is not a bitmap, or has invalid format.
 *  Returns the size (in bytes) of (i) the header, (ii) the pixel data. */
BmpData check_bitmap(FILE *fp)
{
    BmpData data;
    Header header;
    Info info;
    long len;

    len = flength(fp);
    header = get_header(fp);
    info = get_info(fp);
    rewind(fp);

    if (header.bfType != 0x4d42) /* magic number for 'MB', which is little-endian for 'B' then 'M' */
        error("Bitmap file does not start with BM.");
    if (header.bfSize != len)
        error("Bitmap header gives incorrect file size.");
    if (info.biBitCount != 24)
        error("This program only works with 24-bit bitmaps.");
    if (info.biCompression != 0 || info.biClrImportant != 0 || info.biClrUsed != 0)
        error("Incompatible bitmap, unexpected properties.");

    data.headersize = header.bfOffBits;
    data.numpixelbytes = header.bfSize - header.bfOffBits;

    return data;
}
