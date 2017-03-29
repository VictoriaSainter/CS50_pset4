/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize factor infile outfile\n");
        return 1;
    }

    // remember filenames
    int factor = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    
    if( factor < 1 || factor >100 ) {
        printf("please use a factor between 1 and 100");
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_new;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_new = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_new;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_new = bi;
    

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //TODO change header files based on 'n'. Changes include: file size, images size, width, height
    //bisizeimage: total size of image incl pixels & padding (bytes)
    //biwidth: width of image (pixels)
    //biheight: height of image (pixles)
    
    //new size:
    bi_new.biWidth = bi.biWidth * factor;
    bi_new.biHeight = bi.biHeight * factor;
    
    //new padding:
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    int newPadding = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    
    //new image size:
    bi_new.biSizeImage = (bi_new.biWidth * sizeof(RGBTRIPLE) + newPadding)* abs(bi_new.biHeight);
    bf_new.bfSize = bf.bfSize - bi.biSizeImage + bi_new.biSizeImage;
    
    

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    //int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int z =0; z < factor; z++) {
        
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int n = 0 ; n < factor ; n++) { 
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            for (int m = 0; m < newPadding; m++) {
                fputc(0x00, outptr);
            }
           
            
            if (z < factor - 1) {
                fseek(inptr, -bi.biWidth*sizeof(RGBTRIPLE), SEEK_CUR);
            }
        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }
    
        //TODO resize vertically

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
