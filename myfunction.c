#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void writeBMPOpt(Image *image, const char* originalImgFileName, const char* fileName) {

    // open the file to be written
    FILE * bmpfile;
    bmpfile = fopen(fileName, "wb");
    if (NULL == bmpfile) {
        printf("Error opening output file\n");
        // close all open files and free any allocated memory
        exit (1);
    }

    // open BMP file of original image
    FILE * srcFile;
    if ((srcFile = fopen(originalImgFileName, "rb")) == NULL) {
        printf("File Not Found : %s\n", originalImgFileName);
        exit (1);
    }

    // read header of original image
    char originalHeader[54];
    fread(&originalHeader, 1, 54, srcFile);

    // write the BMP file header
    fwrite(&originalHeader, 1, 54, bmpfile);

    // close BMP file of original image
    fclose(srcFile);

    // calculate number of bytes per each line
    register int bytesPerLine = m * 3;  // for 24 bit images
    // round up to a dword boundary
    if (bytesPerLine & 0x0003) {
        bytesPerLine |= 0x0003;
        ++bytesPerLine;
    }

    // allocate buffer to hold one line of the image
    char *linebuf;
    linebuf = (char *) calloc(1, bytesPerLine);
    if (linebuf == NULL) {
        printf ("Error allocating memory\n");
        // close all open files and free any allocated memory
        exit (1);
    }

    // write the image line by line - start with the lowest line
    register int i, line = m;
    register char* iData = image->data, *ilinebuf;
    for (; line >= 0; --line) {

        ilinebuf = linebuf;
        /*
        * fill line linebuf with the image data for that line
        * remember that the order is BGR
        */
        for (i = bytesPerLine; i > 0 ; i -= 3) {

            *(ilinebuf++) = *(iData + 2);
            *(ilinebuf++) = *(iData + 1);
            *(ilinebuf++) = *iData;

            iData+=3;
        }

        /*
        * remember that the order is BGR and if width is not a multiple
        * of 4 then the last few bytes may be unused
        */
        fwrite(linebuf, 1, bytesPerLine, bmpfile);
    }

    // close the image file
    fclose(bmpfile);
}



void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    register int m3 = 3 * m, mn3 = m3 * n, maxi;
    register unsigned char *dest = (unsigned char *) malloc(mn3), *dest1 = dest + m3, *dest2 = dest;
    register unsigned char *data = (unsigned char *) image->data, *src1 = data + m3;
    register int i, j, until = m - 2;
    register int size = mn3 - m3 - m3, words = size / 8, aligned_size = words * 8, offset = size - aligned_size;
    register int pages = words / 8, offset2 = words - pages * 8;
    register long *src64 = (long *) src1, *dst64 = (long *) dest1;

    if (flag == '1') {
        // blur image
        //doConvolution(image, blurKernel, 9, false);
        //smooth1(image, 9, false);

        dest = dest1;
        data = src1;
        (*(int *) dest) = (*(int *) data);
        data += 3;
        dest += 3;
        for (i = until; i > 0; i--) {
            register int red = 0, green = 0, blue = 0;
            register unsigned char *dataBefore = data - m3, *dataAfter = data + m3;
            register int redL = 0, greenL = 0, blueL = 0, redM = 0, greenM = 0, blueM = 0, redR = 0, greenR = 0, blueR = 0;


            redL += *(dataBefore - 3);
            greenL += *(dataBefore - 2);
            blueL += *(dataBefore - 1);

            redM += *dataBefore;
            greenM += *(dataBefore + 1);
            blueM += *(dataBefore + 2);

            redR += *(dataBefore + 3);
            greenR += *(dataBefore + 4);
            blueR += *(dataBefore + 5);


            redL += *(data - 3);
            greenL += *(data - 2);
            blueL += *(data - 1);

            redM += *data;
            greenM += *(data + 1);
            blueM += *(data + 2);

            redR += *(data + 3);
            greenR += *(data + 4);
            blueR += *(data + 5);


            redL += *(dataAfter - 3);
            greenL += *(dataAfter - 2);
            blueL += *(dataAfter - 1);

            redM += *dataAfter;
            greenM += *(dataAfter + 1);
            blueM += *(dataAfter + 2);

            redR += *(dataAfter + 3);
            greenR += *(dataAfter + 4);
            blueR += *(dataAfter + 5);


            // divide by kernel's weight
            red = (redL + redM + redR) / 9;
            green = (greenL + greenM + greenR) / 9;
            blue = (blueL + blueM + blueR) / 9;


            maxi = (red > 0 ? red : 0);
            (*dest) = (maxi < 255 ? maxi : 255);
            maxi = (green > 0 ? green : 0);
            (*(dest + 1)) = (maxi < 255 ? maxi : 255);
            maxi = (blue > 0 ? blue : 0);
            (*(dest + 2)) = (maxi < 255 ? maxi : 255);


            data += 3;
            dest += 3;
            for (j = until - 1; j > 0; j--) {

                dataBefore = data - m3 + 3, dataAfter = data + m3 + 3;
                redL = redM;
                redM = redR;
                greenL = greenM;
                greenM = greenR;
                blueL = blueM;
                blueM = blueR;

                redR = *dataBefore;
                greenR = *(dataBefore + 1);
                blueR = *(dataBefore + 2);

                redR += *(data + 3);
                greenR += *(data + 4);
                blueR += *(data + 5);

                redR += *dataAfter;
                greenR += *(dataAfter + 1);
                blueR += *(dataAfter + 2);


                // divide by kernel's weight
                red = (redL + redM + redR) / 9;
                green = (greenL + greenM + greenR) / 9;
                blue = (blueL + blueM + blueR) / 9;


                maxi = (red > 0 ? red : 0);
                (*dest) = (maxi < 255 ? maxi : 255);
                maxi = (green > 0 ? green : 0);
                (*(dest + 1)) = (maxi < 255 ? maxi : 255);
                maxi = (blue > 0 ? blue : 0);
                (*(dest + 2)) = (maxi < 255 ? maxi : 255);


                data += 3;
                dest += 3;
            }

            (*(long *) dest) = (*(long *) data);
            data += 6;
            dest += 6;
        }

        while (pages--) {
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
        }
        while (offset2--)
            *(src64++) = *(dst64++);

        if (offset) {
            src1 = &src1[aligned_size];
            dest1 = &dest1[aligned_size];
            while (offset--)
                *(src1++) = *(dest1++);
        }



        // write result image to file
        writeBMPOpt(image, srcImgpName, blurRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        //doConvolution(image, blurKernel, 7, true);
//        smooth1(image, 7, true);


        dest += m3;
        data += m3;
        (*(int *) dest) = (*(int *) data);
        data += 3;
        dest += 3;
        for (i = until; i > 0; i--) {
            for (j = until; j > 0; j--) {

                register unsigned char *dataBefore = data - m3, *dataAfter = data + m3;
                register int red = 0, green = 0, blue = 0;

                register int r, g, b, sums;

                r = *(dataBefore - 3);
                g = *(dataBefore - 2);
                b = *(dataBefore - 1);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                register int maxR = r, maxG = g, maxB = b, minR = r, minG = g, minB = b;
                register int max_intensity = sums, min_intensity = sums;

                r = *dataBefore;
                g = *(dataBefore + 1);
                b = *(dataBefore + 2);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }

                r = *(dataBefore + 3);
                g = *(dataBefore + 4);
                b = *(dataBefore + 5);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }


                r = *(data - 3);
                g = *(data - 2);
                b = *(data - 1);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }

                r = *data;
                g = *(data + 1);
                b = *(data + 2);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }

                r = *(data + 3);
                g = *(data + 4);
                b = *(data + 5);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }


                r = *(dataAfter - 3);
                g = *(dataAfter - 2);
                b = *(dataAfter - 1);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }

                r = *dataAfter;
                g = *(dataAfter + 1);
                b = *(dataAfter + 2);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }

                r = *(dataAfter + 3);
                g = *(dataAfter + 4);
                b = *(dataAfter + 5);
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    minR = r;
                    minG = g;
                    minB = b;
                } else if (sums > max_intensity) {
                    max_intensity = sums;
                    maxR = r;
                    maxG = g;
                    maxB = b;
                }


                red -= minR;
                red -= maxR;
                green -= minG;
                green -= maxG;
                blue -= minB;
                blue -= maxB;


                // divide by kernel's weight
                red = red / 7;
                green = green / 7;
                blue = blue / 7;

                //*******************************************************************
                // optimization- reduce call to max and min on the stack
                //*******************************************************************
                // truncate each pixel's color values to match the range [0,255]
                maxi = (red > 0 ? red : 0);
                (*dest) = (maxi < 255 ? maxi : 255);
                maxi = (green > 0 ? green : 0);
                (*(dest + 1)) = (maxi < 255 ? maxi : 255);
                maxi = (blue > 0 ? blue : 0);
                (*(dest + 2)) = (maxi < 255 ? maxi : 255);


                data += 3;
                dest += 3;
            }
            (*(long *) dest) = (*(long *) data);
            data += 6;
            dest += 6;
        }
        while (pages--) {
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
            *(src64++) = *(dst64++);
        }
        while (offset2--)
            *(src64++) = *(dst64++);

        if (offset) {
            src1 = &src1[aligned_size];
            dest1 = &dest1[aligned_size];
            while (offset--)
                *(src1++) = *(dest1++);
        }

        // write result image to file
        writeBMPOpt(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        //doConvolution(image, sharpKernel, 1, false);
//        smooth2(image, 1, false);
    }

    dest = dest2;
    dest1 = dest + m3;
    data = image->data;
    src1 = data + m3;
    dest = dest1;
    data = src1;
    (*(int *) dest) = (*(int *) data);
    data += 3;
    dest += 3;
    for (i = until; i > 0; i--) {
        for (j = until; j > 0; j--) {

            register unsigned char *dataBefore = data - m3, *dataAfter = data + m3;
            register int red = 0, green = 0, blue = 0;

            red -= *(dataBefore - 3);
            green -= *(dataBefore - 2);
            blue -= *(dataBefore - 1);

            red -= *dataBefore;
            green -= *(dataBefore + 1);
            blue -= *(dataBefore + 2);

            red -= *(dataBefore + 3);
            green -= *(dataBefore + 4);
            blue -= *(dataBefore + 5);


            red -= *(data - 3);
            green -= *(data - 2);
            blue -= *(data - 1);

            red += (*data) * 9;
            green += (*(data + 1)) * 9;
            blue += (*(data + 2)) * 9;

            red -= *(data + 3);
            green -= *(data + 4);
            blue -= *(data + 5);


            red -= *(dataAfter - 3);
            green -= *(dataAfter - 2);
            blue -= *(dataAfter - 1);

            red -= *dataAfter;
            green -= *(dataAfter + 1);
            blue -= *(dataAfter + 2);

            red -= *(dataAfter + 3);
            green -= *(dataAfter + 4);
            blue -= *(dataAfter + 5);

            maxi = (red > 0 ? red : 0);
            (*dest) = (maxi < 255 ? maxi : 255);
            maxi = (green > 0 ? green : 0);
            (*(dest + 1)) = (maxi < 255 ? maxi : 255);
            maxi = (blue > 0 ? blue : 0);
            (*(dest + 2)) = (maxi < 255 ? maxi : 255);


            data += 3;
            dest += 3;
        }

        (*(long *) dest) = (*(long *) data);
        data += 6;
        dest += 6;
    }

    offset = size - aligned_size;
    pages = words / 8;
    offset2 = words - pages * 8;
    src64 = (long *) src1;
    dst64 = (long *) dest1;
    while (pages--) {
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
        *(src64++) = *(dst64++);
    }
    while (offset2--)
        *(src64++) = *(dst64++);

    if (offset) {
        src1 = &src1[aligned_size];
        dest1 = &dest1[aligned_size];
        while (offset--)
            *(src1++) = *(dest1++);
    }

    // write result image to file
    if (flag == '1') {
        writeBMPOpt(image, srcImgpName, sharpRsltImgName);
    } else {
        writeBMPOpt(image, srcImgpName, filteredSharpRsltImgName);
    }
    free(dest2);
}

