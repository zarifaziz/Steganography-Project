/* This test file is provided to demonstrate how to use check_bitmap(). */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"

int main(void)
{
    FILE *fbmp = fopen("frog.bmp", "rb");
    FILE *fout = fopen("frogi.bmp", "wb");
    BmpData bdat;

    if (fbmp == NULL || fout == NULL) {
        printf("Error: Could not open files.\n");
        return 1;
    }

    bdat = check_bitmap(fbmp);

    while (bdat.headersize--) { /* Copy header verbatim */
        int c = fgetc(fbmp);
        fputc(c, fout);
    }

    while (bdat.numpixelbytes--) { /* Transform pixel data */
        int c = fgetc(fbmp);
        assert(c != EOF);
        fputc(~c, fout); 
    }

    assert(fgetc(fbmp) == EOF);

    return 0;
}
