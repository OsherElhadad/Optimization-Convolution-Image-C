#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


//static void copy_words(void * dst, const void * src, size_t words)
//{
//    const uint64_t  * src64;
//    uint64_t        * dst64;
//    uint64_t        pages;
//    uint64_t        offset;
//
//    pages = words / 8;
//    offset = words - pages * 8;
//    src64 = (const uint64_t *)src;
//    dst64 = (uint64_t *)dst;
//    while (pages--)
//    {
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//        *dst64++ = *src64++;
//    }
//    while (offset--)
//        *dst64++ = *src64++;
//}
//static void copy_small(void * dst, const void * src, size_t size)
//{
//    const uint64_t  * src64;
//    uint64_t        * dst64;
//
//    src64 = (const uint64_t *)src;
//    dst64 = (uint64_t *)dst;
//    *dst64 = *src64;
//}
//
//static void mem_cpy(void * dst, const void * src, const size_t size)
//{
//    const uint8_t   * src8;
//    uint8_t         * dst8;
//    size_t          offset;
//    size_t          words;
//    size_t          aligned_size;
//
//    if (!src || !dst)
//        return;
//    if (size <= 8)
//    {
//        *(u_int64_t *)dst = *(const uint64_t *)src;
//        return;
//    }
//    words = size / 8;
//    aligned_size = words * 8;
//    offset = size - aligned_size;
//    copy_words(dst, src, words);
//    if (offset)
//    {
//        src8 = (const uint8_t *)src;
//        src8 = &src8[aligned_size];
//        dst8 = (uint8_t *)dst;
//        dst8 = &dst8[aligned_size];
//        while (offset--)
//            *dst8++ = *src8++;
//    }
//    return;
//}

//static void mem_cpy2(void * dst, const void * src, const size_t size)
//{
//    int words = size / 8, aligned_size = words * 8, offset = size - aligned_size;
//
//    int pages = words / 8, offset2 = words - pages * 8;
//    long* src64 = (long *)src, *dst64 = (long *)dst;
//    while (pages--)
//    {
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//        *(dst64++) = *(src64++);
//    }
//    while (offset2--)
//        *(dst64++) = *(src64++);
//
//    if (offset)
//    {
//        char * src8 = (char *)src;
//        src8 = &src8[aligned_size];
//        char * dst8 = (char *)dst;
//        dst8 = &dst8[aligned_size];
//        while (offset--)
//            *(dst8++) = *(src8++);
//    }
//}



void smooth1(Image *image1, int kernelScale, bool filter) {

    int m3 = 3*m, mn3 = m3*n;
    char *dest = (char *)malloc(mn3), *dest1 = dest + m3, *dest2 = dest;
    char *data = image1->data, *src1 = data + m3;
    int i, j, until = m - 2;

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
//    for (int k = 0; k < m3; ++k) {
//        (*dest) = (*data);
//        data++;
//        dest++;
//    }
//    mem_cpy(dest, data, m3);
    dest+=m3;
    data+=m3;
    (*(int*)dest) = (*(int*)data);
    data+=3;
    dest+=3;
    for (i = until; i > 0; i--) {
//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
        for (j = until; j > 0; j--) {

            char *dataBefore = data - m3, *dataAfter = data + m3;
            //pixel loop_pixel;

            //initialize_pixel_sum(&sum);

            //*******************************************************************
            // optimization- reduce using struct pixel_sum
            //*******************************************************************
            int red = 0, green = 0, blue = 0;

            //*******************************************************************
            // optimization- reduce multiple loops
            //*******************************************************************
            if (!filter) {

                // apply kernel on pixel at [ii,jj]
                //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************

                //*******************************************************************
                // optimization- reduce call to sum_pixel_by_weight
                //*******************************************************************

                red += ((unsigned char)*(dataBefore - 3));
                green += ((unsigned char)*(dataBefore - 2));
                blue += ((unsigned char)*(dataBefore - 1));

                red += ((unsigned char)*dataBefore);
                green += ((unsigned char)*(dataBefore + 1));
                blue += ((unsigned char)*(dataBefore + 2));

                red += ((unsigned char)*(dataBefore + 3));
                green += ((unsigned char)*(dataBefore + 4));
                blue += ((unsigned char)*(dataBefore + 5));



                red += ((unsigned char)*(data - 3));
                green += ((unsigned char)*(data - 2));
                blue += ((unsigned char)*(data - 1));

                red += ((unsigned char)*data);
                green += ((unsigned char)*(data + 1));
                blue += ((unsigned char)*(data + 2));

                red += ((unsigned char)*(data + 3));
                green += ((unsigned char)*(data + 4));
                blue += ((unsigned char)*(data + 5));



                red += ((unsigned char)*(dataAfter - 3));
                green += ((unsigned char)*(dataAfter - 2));
                blue += ((unsigned char)*(dataAfter - 1));

                red += ((unsigned char)*dataAfter);
                green += ((unsigned char)*(dataAfter + 1));
                blue += ((unsigned char)*(dataAfter + 2));

                red += ((unsigned char)*(dataAfter + 3));
                green += ((unsigned char)*(dataAfter + 4));
                blue += ((unsigned char)*(dataAfter + 5));
            } else {

                // apply kernel on pixel at [ii,jj]
                // sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************

                //*******************************************************************
                // optimization- reduce call to sum_pixel_by_weight
                //*******************************************************************
                int r, g, b, sums;

                r = ((unsigned char)*(dataBefore - 3));
                g = ((unsigned char)*(dataBefore - 2));
                b = ((unsigned char)*(dataBefore - 1));
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                int maxR = r, maxG = g, maxB = b, minR = r, minG = g, minB = b;
                int max_intensity = sums, min_intensity = sums;

                r = ((unsigned char)*dataBefore);
                g = ((unsigned char)*(dataBefore + 1));
                b = ((unsigned char)*(dataBefore + 2));
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

                r = ((unsigned char)*(dataBefore + 3));
                g = ((unsigned char)*(dataBefore + 4));
                b = ((unsigned char)*(dataBefore + 5));
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



                r = ((unsigned char)*(data - 3));
                g = ((unsigned char)*(data - 2));
                b = ((unsigned char)*(data - 1));
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

                r = ((unsigned char)*data);
                g = ((unsigned char)*(data + 1));
                b = ((unsigned char)*(data + 2));
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

                r = ((unsigned char)*(data + 3));
                g = ((unsigned char)*(data + 4));
                b = ((unsigned char)*(data + 5));
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



                r = ((unsigned char)*(dataAfter - 3));
                g = ((unsigned char)*(dataAfter - 2));
                b = ((unsigned char)*(dataAfter - 1));
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

                r = ((unsigned char)*dataAfter);
                g = ((unsigned char)*(dataAfter + 1));
                b = ((unsigned char)*(dataAfter + 2));
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

                r = ((unsigned char)*(dataAfter + 3));
                g = ((unsigned char)*(dataAfter + 4));
                b = ((unsigned char)*(dataAfter + 5));
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
            }

            //*******************************************************************
            // optimization- reduce call to assign_sum_to_pixel on the stack
            //*******************************************************************
            // assign kernel's result to pixel at [i,j]

            // divide by kernel's weight
            red = red / kernelScale;
            green = green / kernelScale;
            blue = blue / kernelScale;

            //*******************************************************************
            // optimization- reduce call to max and min on the stack
            //*******************************************************************
            // truncate each pixel's color values to match the range [0,255]
            int maxi = (red > 0 ? red : 0);
            (*dest) = (unsigned char) (maxi < 255 ? maxi : 255);
            maxi = (green > 0 ? green : 0);
            (*(dest + 1)) = (unsigned char) (maxi < 255 ? maxi : 255);
            maxi = (blue > 0 ? blue : 0);
            (*(dest + 2)) = (unsigned char) (maxi < 255 ? maxi : 255);


            data += 3;
            dest += 3;
        }

//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
        (*(long*)dest) = (*(long*)data);
        data+=6;
        dest+=6;
    }
//    for (int k = 0; k < m3; ++k) {
//        (*dest) = (*data);
//        data++;
//        dest++;
//    }
//    mem_cpy(dest, data, m3);
//    dest+=m3;
//    data+=m3;
//    mem_cpy(src1, dest1, mn3 - m3 - m3);
    int size = mn3 - m3 - m3, words = size / 8, aligned_size = words * 8, offset = size - aligned_size;
    int pages = words / 8, offset2 = words - pages * 8;
    long* src64 = (long *)src1, *dst64 = (long *)dest1;
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
//    for (int k = 0; k < mn3; ++k) {
//        (*src1) = (*dest1);
//        src1++;
//        dest1++;
//    }
    free(dest2);
}





/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth2(Image *image1, int kernelScale, bool filter) {

    int m3 = 3*m, mn3 = m3*n;
    char *dest = (char *)malloc(mn3), *dest1 = dest + m3, *dest2 = dest;
    char *data = image1->data, *src1 = data + m3;
    int i, j, until = m - 1;

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
//    for (int k = 0; k < m3; ++k) {
//        (*dest) = (*data);
//        data++;
//        dest++;
//    }
//    mem_cpy(dest, data, m3);
    dest+=m3;
    data+=m3;
    for (i = 1; i < until; i++) {
//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
        (*(int*)dest) = (*(int*)data);
        data+=3;
        dest+=3;
        for (j = 1; j < until; j++) {

            char *dataBefore = data - m3, *dataAfter = data + m3;
            //pixel loop_pixel;

            //initialize_pixel_sum(&sum);

            //*******************************************************************
            // optimization- reduce using struct pixel_sum
            //*******************************************************************
            int red = 0, green = 0, blue = 0;

            //*******************************************************************
            // optimization- reduce multiple loops
            //*******************************************************************
            if (!filter) {

                // apply kernel on pixel at [ii,jj]
                //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************

                //*******************************************************************
                // optimization- reduce call to sum_pixel_by_weight
                //*******************************************************************

                red -= ((unsigned char)*(dataBefore - 3));
                green -= ((unsigned char)*(dataBefore - 2));
                blue -= ((unsigned char)*(dataBefore - 1));

                red -= ((unsigned char)*dataBefore);
                green -= ((unsigned char)*(dataBefore + 1));
                blue -= ((unsigned char)*(dataBefore + 2));

                red -= ((unsigned char)*(dataBefore + 3));
                green -= ((unsigned char)*(dataBefore + 4));
                blue -= ((unsigned char)*(dataBefore + 5));



                red -= ((unsigned char)*(data - 3));
                green -= ((unsigned char)*(data - 2));
                blue -= ((unsigned char)*(data - 1));

                red += ((unsigned char)*data) * 9;
                green += ((unsigned char)*(data + 1)) * 9;
                blue += ((unsigned char)*(data + 2)) * 9;

                red -= ((unsigned char)*(data + 3));
                green -= ((unsigned char)*(data + 4));
                blue -= ((unsigned char)*(data + 5));



                red -= ((unsigned char)*(dataAfter - 3));
                green -= ((unsigned char)*(dataAfter - 2));
                blue -= ((unsigned char)*(dataAfter - 1));

                red -= ((unsigned char)*dataAfter);
                green -= ((unsigned char)*(dataAfter + 1));
                blue -= ((unsigned char)*(dataAfter + 2));

                red -= ((unsigned char)*(dataAfter + 3));
                green -= ((unsigned char)*(dataAfter + 4));
                blue -= ((unsigned char)*(dataAfter + 5));
            } else {

                // apply kernel on pixel at [ii,jj]
                // sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************

                //*******************************************************************
                // optimization- reduce call to sum_pixel_by_weight
                //*******************************************************************
                int r, g, b, sums;

                r = -((unsigned char)*(dataBefore - 3));
                g = -((unsigned char)*(dataBefore - 2));
                b = -((unsigned char)*(dataBefore - 1));
                red += r;
                green += g;
                blue += b;

                sums = r + g + b;
                int maxR = r, maxG = g, maxB = b, minR = r, minG = g, minB = b;
                int max_intensity = sums, min_intensity = sums;

                r = -((unsigned char)*dataBefore);
                g = -((unsigned char)*(dataBefore + 1));
                b = -((unsigned char)*(dataBefore + 2)) ;
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

                r = -((unsigned char)*(dataBefore + 3));
                g = -((unsigned char)*(dataBefore + 4));
                b = -((unsigned char)*(dataBefore + 5));
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



                r = -((unsigned char)*(data - 3));
                g = -((unsigned char)*(data - 2));
                b = -((unsigned char)*(data - 1));
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

                r = ((unsigned char)*data) * 9;
                g = ((unsigned char)*(data + 1)) * 9;
                b = ((unsigned char)*(data + 2)) * 9;
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

                r = -((unsigned char)*(data + 3));
                g = -((unsigned char)*(data + 4));
                b = -((unsigned char)*(data + 5));
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



                r = -((unsigned char)*(dataAfter - 3));
                g = -((unsigned char)*(dataAfter - 2));
                b = -((unsigned char)*(dataAfter - 1));
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

                r = -((unsigned char)*dataAfter);
                g = -((unsigned char)*(dataAfter + 1));
                b = -((unsigned char)*(dataAfter + 2));
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

                r = -((unsigned char)*(dataAfter + 3));
                g = -((unsigned char)*(dataAfter + 4));
                b = -((unsigned char)*(dataAfter + 5));
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
            }

            //*******************************************************************
            // optimization- reduce call to assign_sum_to_pixel on the stack
            //*******************************************************************
            // assign kernel's result to pixel at [i,j]

            //*******************************************************************
            // optimization- reduce call to max and min on the stack
            //*******************************************************************
            // truncate each pixel's color values to match the range [0,255]
            int maxi = (red > 0 ? red : 0);
            (*dest) = (unsigned char) (maxi < 255 ? maxi : 255);
            maxi = (green > 0 ? green : 0);
            (*(dest + 1)) = (unsigned char) (maxi < 255 ? maxi : 255);
            maxi = (blue > 0 ? blue : 0);
            (*(dest + 2)) = (unsigned char) (maxi < 255 ? maxi : 255);


            data += 3;
            dest += 3;
        }

//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
//        (*dest) = (*data);
//        data++;
//        dest++;
        (*(int*)dest) = (*(int*)data);
        data+=3;
        dest+=3;
    }
//    for (int k = 0; k < m3; ++k) {
//        (*dest) = (*data);
//        data++;
//        dest++;
//    }
//    mem_cpy(dest, data, m3);
//    dest+=m3;
//    data+=m3;
//    mem_cpy(src1, dest1, mn3 - m3 - m3);

    int size = mn3 - m3 - m3, words = size / 8, aligned_size = words * 8, offset = size - aligned_size;
    int pages = words / 8, offset2 = words - pages * 8;
    long* src64 = (long *)src1, *dst64 = (long *)dest1;
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

//    for (int k = 0; k < mn3; ++k) {
//        (*src1) = (*dest1);
//        src1++;
//        dest1++;
//    }
    free(dest2);
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    //*******************************************************************
    // optimization- use cache by define array and not matrix
    //*******************************************************************
	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	//int blurKernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	//int sharpKernel[9] = {-1,-1,-1,-1,9,-1,-1,-1,-1};

	if (flag == '1') {	
		// blur image
		//doConvolution(image, blurKernel, 9, false);
        smooth1(image, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		//doConvolution(image, sharpKernel, 1, false);
        smooth2(image, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		//doConvolution(image, blurKernel, 7, true);
        smooth1(image, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		//doConvolution(image, sharpKernel, 1, false);
        smooth2(image, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

