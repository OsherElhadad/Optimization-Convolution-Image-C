#include <stdbool.h>
#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <sys/types.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#include <sys/mman.h>
//#include <unistd.h>
//#include <pthread.h>

//void writeBMPOpt(Image *image, const char* originalImgFileName, const char* fileName) {
//
//    // open the file to be written
//    FILE * bmpfile;
//    bmpfile = fopen(fileName, "wb");
//    if (NULL == bmpfile) {
//        printf("Error opening output file\n");
//        // close all open files and free any allocated memory
//        exit (1);
//    }
//
//    // open BMP file of original image
//    FILE * srcFile;
//    if ((srcFile = fopen(originalImgFileName, "rb")) == NULL) {
//        printf("File Not Found : %s\n", originalImgFileName);
//        exit (1);
//    }
//
//    // read header of original image
//    char originalHeader[54];
//    fread(&originalHeader, 1, 54, srcFile);
//
//    // write the BMP file header
//    fwrite(&originalHeader, 1, 54, bmpfile);
//
//    // close BMP file of original image
//    fclose(srcFile);
//
//    // calculate number of bytes per each line
//    register int bytesPerLine = m * 3;  // for 24 bit images
//    // round up to a dword boundary
//    if (bytesPerLine & 0x0003) {
//        bytesPerLine |= 0x0003;
//        ++bytesPerLine;
//    }
//
//    // allocate buffer to hold one line of the image
//    char *linebuf;
//    linebuf = (char *) calloc(1, bytesPerLine);
//    if (linebuf == NULL) {
//        printf ("Error allocating memory\n");
//        // close all open files and free any allocated memory
//        exit (1);
//    }
//
//    // write the image line by line - start with the lowest line
//    register int i, line = m;
//    register char* iData = image->data, *ilinebuf;
//    for (; line >= 0; --line) {
//
//        ilinebuf = linebuf;
//        /*
//        * fill line linebuf with the image data for that line
//        * remember that the order is BGR
//        */
//        for (i = bytesPerLine; i > 0 ; i -= 3) {
//
//            *(ilinebuf++) = *(iData + 2);
//            *(ilinebuf++) = *(iData + 1);
//            *(ilinebuf++) = *iData;
//
//            iData+=3;
//        }
//
//        /*
//        * remember that the order is BGR and if width is not a multiple
//        * of 4 then the last few bytes may be unused
//        */
//        fwrite(linebuf, 1, bytesPerLine, bmpfile);
//    }
//
//    // close the image file
//    fclose(bmpfile);
//}
//
//
//void writeBMPOptMemMap(Image *image, const char* originalImgFileName, const char* fileName) {
//
//    int fdDst = open(originalImgFileName, O_RDWR, S_IRUSR | S_IWUSR);
//    struct stat sbDst;
//    if (fdDst < 0) {
//        printf("Error opening output file\n");
//        // close all open files and free any allocated memory
//        exit (1);
//    }
//
//    int fdOriginal = open(originalImgFileName, O_RDONLY, S_IRUSR | S_IWUSR);
//    if (fdOriginal < 0) {
//        printf("File Not Found : %s\n", originalImgFileName);
//        exit (1);
//    }
//    char * orig = mmap(NULL, 54, PROT_READ, MAP_PRIVATE, fdOriginal, 0);
//    char * dst = mmap(NULL, 54 + (m + 1) * n * 3, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdDst, 0);
//
//    for (int i = 0; i < 54; ++i) {
//        *(dst++) = *(orig++);
//    }
//    close(fdOriginal);
//
//    // calculate number of bytes per each line
//    register int bytesPerLine = m * 3;  // for 24 bit images
//    // round up to a dword boundary
//    if (bytesPerLine & 0x0003) {
//        bytesPerLine |= 0x0003;
//        ++bytesPerLine;
//    }
//
//    // write the image line by line - start with the lowest line
//    register int i, line = m;
//    register char* iData = image->data;
//    for (; line >= 0; --line) {
//        /*
//        * fill line linebuf with the image data for that line
//        * remember that the order is BGR
//        */
//        for (i = bytesPerLine; i > 0 ; i -= 3) {
//
//            *(dst++) = *(iData + 2);
//            *(dst++) = *(iData + 1);
//            *(dst++) = *iData;
//
//            iData+=3;
//        }
//    }
//
//    close(fdDst);
//}
//char f;
//char* srcImgpName1;
//char* blurRsltImgName1;
//char* filteredBlurRsltImgName1;
//Image *image1;
//void *myThreadFun(void *vargp)
//{
//    if(f == '1') {
//        // write result image to file
//        writeBMP(image1, srcImgpName1, blurRsltImgName1);
//    } else {
//        // write result image to file
//        writeBMP(image1, srcImgpName1, filteredBlurRsltImgName1);
//    }
//    return NULL;
//}

void apply1(unsigned char * data1, unsigned char * dest1, int mm3) {
    register unsigned char *data = data1, *dest = dest1;
    register int m3 = mm3;
    register int i, j, until = m - 2, until2 = until - 1;
    (*(int *) dest) = (*(int *) data);
    data += 3;
    dest += 3;
    for (i = until; i > 0; i--) {
        register int red, green, blue;
        register unsigned char *dataBefore = data - m3, *dataAfter = data + m3;
        register int redL, greenL, blueL, redM, greenM, blueM, redR, greenR, blueR;

        redL = *(dataBefore - 3);
        greenL = *(dataBefore - 2);
        blueL = *(dataBefore - 1);

        redM = *dataBefore;
        greenM = *(dataBefore + 1);
        blueM = *(dataBefore + 2);

        redR = *(dataBefore + 3);
        greenR = *(dataBefore + 4);
        blueR = *(dataBefore + 5);


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


        (*dest) = (red < 255 ? red : 255);
        (*(dest + 1)) = (green < 255 ? green : 255);
        (*(dest + 2)) = (blue < 255 ? blue : 255);


        data += 3;
        dest += 3;
        dataBefore += 3;
        dataAfter += 3;
        for (j = until2; j > 0; j--) {

            dataBefore += 3;
            dataAfter += 3;
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


            (*dest) = (red < 255 ? red : 255);
            (*(dest + 1)) = (green < 255 ? green : 255);
            (*(dest + 2)) = (blue < 255 ? blue : 255);


            data += 3;
            dest += 3;
        }

        (*(long *) dest) = (*(long *) data);
        data += 6;
        dest += 6;
    }
}

void apply2(unsigned char * data1, unsigned char * dest1, int mm3) {
    register unsigned char *data = data1, *dest = dest1;
    register int m3 = mm3;
    register int i, j, until = m - 2, until2 = until - 1;;
    register int red, green, blue;
    register int r, g, b, sums;
    register int max_intensity, min_intensity;
    (*(int *) dest) = (*(int *) data);
    data += 3;
    dest += 3;
    register unsigned char *dataBefore = data - m3, *dataAfter = data + m3;
    for (i = until; i > 0; i--) {

        r = *(dataBefore - 3);
        g = *(dataBefore - 2);
        b = *(dataBefore - 1);
        red = r;
        green = g;
        blue = b;

        sums = r + g + b;
        register int maxR = r, maxG = g, maxB = b, minR = r, minG = g, minB = b;
        max_intensity = sums;
        min_intensity = sums;

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
        register int maxi = (red > 0 ? red : 0);
        (*dest) = (maxi < 255 ? maxi : 255);
        maxi = (green > 0 ? green : 0);
        (*(dest + 1)) = (maxi < 255 ? maxi : 255);
        maxi = (blue > 0 ? blue : 0);
        (*(dest + 2)) = (maxi < 255 ? maxi : 255);


        data += 3;
        dest += 3;
        dataBefore += 3;
        dataAfter += 3;

        for (j = until2; j > 0; j--) {

            r = *(dataBefore - 3);
            g = *(dataBefore - 2);
            b = *(dataBefore - 1);
            red = r;
            green = g;
            blue = b;

            sums = r + g + b;
            maxR = r;
            maxG = g;
            maxB = b;
            minR = r;
            minG = g;
            minB = b;
            max_intensity = sums;
            min_intensity = sums;

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
            register int maxi = (red > 0 ? red : 0);
            (*dest) = (maxi < 255 ? maxi : 255);
            maxi = (green > 0 ? green : 0);
            (*(dest + 1)) = (maxi < 255 ? maxi : 255);
            maxi = (blue > 0 ? blue : 0);
            (*(dest + 2)) = (maxi < 255 ? maxi : 255);


            data += 3;
            dest += 3;
            dataBefore += 3;
            dataAfter += 3;
        }
        (*(long *) dest) = (*(long *) data);
        data += 6;
        dest += 6;
        dataBefore += 6;
        dataAfter += 6;
    }
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    register int m3 = m + (m << 1), mn3 = m3 * n;
    register unsigned char *dest = (unsigned char *) malloc(mn3);
    register unsigned char *data = (unsigned char *) image->data, *src1 = data, *dest1 = dest;
    image->data = (char *)dest1;
    register int size = m3, words = size / 8, aligned_size = (words << 3), offset = size - aligned_size;
    register int pages = words / 8, offset2 = words - (pages << 3);
    register long *src64 = (long *) src1, *dst64 = (long *) dest1;

    while (pages--) {
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
    }
    while (offset2--)
        *(dst64++) = *(src64++);

    if (offset) {
        data = &data[aligned_size];
        dest = &dest[aligned_size];
        while (offset--)
            *(dest++) = *(data++);
    }

    if (flag == '1') {
        // blur image
        //doConvolution(image, blurKernel, 9, false);
        //smooth1(image, 9, false);
        apply1(data, dest, m3);
    } else {
        // apply extermum filtered kernel to blur image
        //doConvolution(image, blurKernel, 7, true);
//        smooth1(image, 7, true);
        apply2(data, dest, m3);
    }

    offset = size - aligned_size;
    pages = words / 8;
    offset2 = words - (pages << 3);
    data += (m - 2) * m3;
    dest += (m - 2) * m3;
    src64 = (long *) data;
    dst64 = (long *) dest;
    while (pages--) {
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
        *(dst64++) = *(src64++);
    }
    while (offset2--)
        *(dst64++) = *(src64++);

    if (offset) {
        data = &data[aligned_size];
        dest = &dest[aligned_size];
        while (offset--)
            *(dest++) = *(data++);
    }

    if(flag == '1') {
        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);
//        writeBMPOpt(image, srcImgpName, blurRsltImgName);
    } else {
        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);
//        writeBMPOpt(image, srcImgpName, filteredBlurRsltImgName);
    }

//    f = flag;
//    srcImgpName1 = srcImgpName;
//    filteredBlurRsltImgName1 = filteredBlurRsltImgName;
//    blurRsltImgName1 = blurRsltImgName;
//    image1 = image;
//    pthread_t thread_id;
//    pthread_create(&thread_id, NULL, myThreadFun, NULL);

    image->data = (char *)src1;
//    dest = src1;
//    data = dest1;
    dest = src1 + m3;
    data = dest1 + m3;

//    (*(int *) dest) = (*(int *) data);
    data += 3;
    dest += 3;
    register int i, j, until = m - 2, until2 = until - 1;
    for (i = until; i > 0; i--) {
        register unsigned char *dataBefore = data - m3, *dataAfter = data + m3;
        register int red, green, blue , red9, green9, blue9, red9R, green9R, blue9R;
        register int redL, greenL, blueL, redM, greenM, blueM, redR, greenR, blueR;


        redL = -*(dataBefore - 3);
        greenL = -*(dataBefore - 2);
        blueL = -*(dataBefore - 1);

        redM = -*dataBefore;
        greenM = -*(dataBefore + 1);
        blueM = -*(dataBefore + 2);

        redR = -*(dataBefore + 3);
        greenR = -*(dataBefore + 4);
        blueR = -*(dataBefore + 5);


        redL -= *(data - 3);
        greenL -= *(data - 2);
        blueL -= *(data - 1);

        red9 = -(*data);
        green9 = -*(data + 1);
        blue9 = -*(data + 2);

        red9R = -*(data + 3);
        green9R = -*(data + 4);
        blue9R = -*(data + 5);


        redL -= *(dataAfter - 3);
        greenL -= *(dataAfter - 2);
        blueL -= *(dataAfter - 1);

        redM -= *dataAfter;
        greenM -= *(dataAfter + 1);
        blueM -= *(dataAfter + 2);

        redR -= *(dataAfter + 3);
        greenR -= *(dataAfter + 4);
        blueR -= *(dataAfter + 5);


        red = (redL + redM + ((-red9) << 3) - red9 + redR + red9R);
        green = (greenL + greenM + ((-green9) << 3) - green9 + greenR + green9R);
        blue = (blueL + blueM + ((-blue9) << 3) - blue9 + blueR + blue9R);


        register int maxi = (red > 0 ? red : 0);
        (*dest) = (maxi < 255 ? maxi : 255);
        maxi = (green > 0 ? green : 0);
        (*(dest + 1)) = (maxi < 255 ? maxi : 255);
        maxi = (blue > 0 ? blue : 0);
        (*(dest + 2)) = (maxi < 255 ? maxi : 255);


        data += 3;
        dest += 3;
        dataBefore += 3;
        dataAfter += 3;
        for (j = until2; j > 0; j--) {

            dataBefore += 3;
            dataAfter += 3;
            redL = redM + red9;
            redM = redR;
            greenL = greenM + green9;
            greenM = greenR;
            blueL = blueM + blue9;
            blueM = blueR;

            redR = -(*dataBefore);
            greenR = -(*(dataBefore + 1));
            blueR = -(*(dataBefore + 2));

            red9R = -(*(data + 3));
            green9R = -(*(data + 4));
            blue9R = -(*(data + 5));

            redR -= *dataAfter;
            greenR -= *(dataAfter + 1);
            blueR -= *(dataAfter + 2);

            red9 = -(*data);
            green9 = -(*(data + 1));
            blue9 = -*((data + 2));


            red = (redL + redM + ((-red9) << 3) - red9 + redR + red9R);
            green = (greenL + greenM + ((-green9) << 3) - green9 + greenR + green9R);
            blue = (blueL + blueM + ((-blue9) << 3) - blue9 + blueR + blue9R);


            maxi = (red > 0 ? red : 0);
            (*dest) = (maxi < 255 ? maxi : 255);
            maxi = (green > 0 ? green : 0);
            (*(dest + 1)) = (maxi < 255 ? maxi : 255);
            maxi = (blue > 0 ? blue : 0);
            (*(dest + 2)) = (maxi < 255 ? maxi : 255);


            data += 3;
            dest += 3;
        }

//        (*(long *) dest) = (*(long *) data);
        data += 6;
        dest += 6;
    }

//    pthread_join(thread_id, NULL);

    // write result image to file
    if (flag == '1') {
        writeBMP(image, srcImgpName, sharpRsltImgName);
//        writeBMPOpt(image, srcImgpName, sharpRsltImgName);
    } else {
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
//        writeBMPOpt(image, srcImgpName, filteredSharpRsltImgName);
    }
    free(dest1);
}

