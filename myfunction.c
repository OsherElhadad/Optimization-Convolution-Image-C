#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

//typedef struct {
//    int red;
//    int green;
//    int blue;
//    // int num;
//} pixel_sum;

//*******************************************************************
// optimization- inline function, reduce call to min/max on the stack
//*******************************************************************
/* Compute min and max of two integers, respectively */
static inline int min(int a, int b) { return (a < b ? a : b); }
static inline int max(int a, int b) { return (a > b ? a : b); }

//*******************************************************************
// optimization- inline function, reduce call to calcIndex on the stack
//*******************************************************************
//static inline int calcIndex(int i, int j, int n) {
//	return ((i)*(n)+(j));
//}

//*******************************************************************
// optimization- inline function, reduce call to initialize_pixel_sum on the stack
//*******************************************************************
/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
//static inline void initialize_pixel_sum(pixel_sum *sum) {
//	sum->red = sum->green = sum->blue = 0;
//	// sum->num = 0;
//	return;
//}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
//static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {
//
//	// divide by kernel's weight
//	int red = sum.red / kernelScale;
//	int green = sum.green / kernelScale;
//	int blue = sum.blue / kernelScale;
//
//	// truncate each pixel's color values to match the range [0,255]
//    int maxi = (red > 0 ? red : 0);
//	current_pixel->red = (unsigned char) (maxi < 255 ? maxi : 255);
//    maxi = (green > 0 ? green : 0);
//	current_pixel->green = (unsigned char) (maxi < 255 ? maxi : 255);
//    maxi = (blue > 0 ? blue : 0);
//	current_pixel->blue = (unsigned char) (maxi < 255 ? maxi : 255);
//	return;
//}

//*******************************************************************
// optimization- inline function, reduce call to sum_pixels_by_weight on the stack
//*******************************************************************
/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
//static inline void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
//	sum->red += ((int) p.red) * weight;
//	sum->green += ((int) p.green) * weight;
//	sum->blue += ((int) p.blue) * weight;
//	// sum->num++;
//	return;
//}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int i, int j, pixel *src, int kernel[3], int kernelScale, bool filter) {
    //pixel_sum sum;
    pixel current_pixel;
    int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    int min_row, min_col, max_row, max_col;
    //pixel loop_pixel;

    //initialize_pixel_sum(&sum);

    //*******************************************************************
    // optimization- reduce using struct pixel_sum
    //*******************************************************************
    int red = 0, green = 0, blue = 0;

    int ii= i-1, jj = j-1;
    int dimMulII = ii*m;

    //*******************************************************************
    // optimization- reduce multiple loops
    //*******************************************************************
    if (!filter) {

        for (; ii <= i+1; ii++) {

            jj = j-1;
            int dimAddJJ = dimMulII + jj;

            for (; jj <= j+1; jj++) {

                // apply kernel on pixel at [ii,jj]
                //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

                int weight = (*kernel);

                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************
                pixel *p = &src[dimAddJJ];

                //*******************************************************************
                // optimization- reduce call to sum_pixel_by_weight
                //*******************************************************************
                red += ((int) p->red) * weight;
                green += ((int) p->green) * weight;
                blue += ((int) p->blue) * weight;

                dimAddJJ++;
                kernel++;
            }

            //*******************************************************************
            // optimization- reduce multiple ii*dim
            //*******************************************************************
            dimMulII += m;
        }
    } else {

        // find min and max coordinates
        for (; ii <= i+1; ii++) {

            jj = j-1;
            int dimAddJJ = dimMulII + jj;

            for (; jj <= j+1; jj++) {

                // apply kernel on pixel at [ii,jj]
                //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

                int weight = (*kernel);

                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************
                pixel *p = &src[dimAddJJ];

                //*******************************************************************
                // optimization- reduce call to sum_pixel_by_weight
                //*******************************************************************
                red += ((int) p->red) * weight;
                green += ((int) p->green) * weight;
                blue += ((int) p->blue) * weight;

                // check if smaller than min or higher than max and update

                //*******************************************************************
                // optimization- reduce calculate of sums from 4 times to one.
                //*******************************************************************
                int sums = (((int) p->red) + ((int) p->green) + ((int) p->blue));
                if (sums <= min_intensity) {
                    min_intensity = sums;
                    min_row = ii;
                    min_col = jj;
                }
                if (sums > max_intensity) {
                    max_intensity = sums;
                    max_row = ii;
                    max_col = jj;
                }
                dimAddJJ++;
                kernel++;
            }

            //*******************************************************************
            // optimization- reduce multiple ii*dim
            //*******************************************************************
            dimMulII += m;
        }

        // filter out min and max
        int weight = -1;

        //*******************************************************************
        // optimization- reduce call to calcIndex
        //*******************************************************************
        pixel* p = &src[(min_row*m + min_col)];

        //*******************************************************************
        // optimization- reduce call to sum_pixel_by_weight
        //*******************************************************************
        red += ((int) p->red) * weight;
        green += ((int) p->green) * weight;
        blue += ((int) p->blue) * weight;

        //*******************************************************************
        // optimization- reduce call to calcIndex
        //*******************************************************************
        p = &src[(max_row*m + max_col)];

        //*******************************************************************
        // optimization- reduce call to sum_pixel_by_weight
        //*******************************************************************
        red += ((int) p->red) * weight;
        green += ((int) p->green) * weight;
        blue += ((int) p->blue) * weight;
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
    current_pixel.red = (unsigned char) (maxi < 255 ? maxi : 255);
    maxi = (green > 0 ? green : 0);
    current_pixel.green = (unsigned char) (maxi < 255 ? maxi : 255);
    maxi = (blue > 0 ? blue : 0);
    current_pixel.blue = (unsigned char) (maxi < 255 ? maxi : 255);
    return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(pixel *src, pixel *dst, int kernel[3], int kernelScale, bool filter) {

	int i, j;

    //*******************************************************************
    // optimization- calculate (dim - kernelSize / 2) before the loop
    //*******************************************************************
    int until = m - 1;

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
    dst += m;
	for (i = 1; i < until; i++) {
        dst++;
		for (j =  1; j < until ; j++) {
            (*dst) = applyKernel(i, j, src, kernel, kernelScale, filter);
            dst++;
		}
        dst++;
	}
}



/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth2(Image *image1, int weight1, int weight2, int kernelScale, bool filter) {

    char *dest = (char *)malloc(3*m*n), *dest1 = dest, *dest2 = dest;
    char *data = image1->data, *src1 = data;
    int i, j, m3 = 3*m;

    //*******************************************************************
    // optimization- calculate (dim - kernelSize / 2) before the loop
    //*******************************************************************
    int until = m - 1;

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
    for (int k = 0; k < m3; ++k) {
        (*dest) = (*data);
        data++;
        dest++;
    }
    for (i = 1; i < until; i++) {
        (*dest) = (*data);
        data++;
        dest++;
        (*dest) = (*data);
        data++;
        dest++;
        (*dest) = (*data);
        data++;
        dest++;
        for (j = 1; j < until; j++) {

            int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
            int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
            int min_row, min_col, max_row, max_col;
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

                red += ((unsigned char)*(dataBefore - 3)) * weight1;
                green += ((unsigned char)*(dataBefore - 2)) * weight1;
                blue += ((unsigned char)*(dataBefore - 1)) * weight1;

                red += ((unsigned char)*dataBefore) * weight1;
                green += ((unsigned char)*(dataBefore + 1)) * weight1;
                blue += ((unsigned char)*(dataBefore + 2)) * weight1;

                red += ((unsigned char)*(dataBefore + 3)) * weight1;
                green += ((unsigned char)*(dataBefore + 4)) * weight1;
                blue += ((unsigned char)*(dataBefore + 5)) * weight1;



                red += ((unsigned char)*(data - 3)) * weight1;
                green += ((unsigned char)*(data - 2)) * weight1;
                blue += ((unsigned char)*(data - 1)) * weight1;

                red += ((unsigned char)*data) * weight2;
                green += ((unsigned char)*(data + 1)) * weight2;
                blue += ((unsigned char)*(data + 2)) * weight2;

                red += ((unsigned char)*(data + 3)) * weight1;
                green += ((unsigned char)*(data + 4)) * weight1;
                blue += ((unsigned char)*(data + 5)) * weight1;



                red += ((unsigned char)*(dataAfter - 3)) * weight1;
                green += ((unsigned char)*(dataAfter - 2)) * weight1;
                blue += ((unsigned char)*(dataAfter - 1)) * weight1;

                red += ((unsigned char)*dataAfter) * weight1;
                green += ((unsigned char)*(dataAfter + 1)) * weight1;
                blue += ((unsigned char)*(dataAfter + 2)) * weight1;

                red += ((unsigned char)*(dataAfter + 3)) * weight1;
                green += ((unsigned char)*(dataAfter + 4)) * weight1;
                blue += ((unsigned char)*(dataAfter + 5)) * weight1;
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

                r = ((unsigned char)*(dataBefore - 3)) * weight1;
                red += r;
                g = ((unsigned char)*(dataBefore - 2)) * weight1;
                green += g;
                b = ((unsigned char)*(dataBefore - 1)) * weight1;
                blue += b;

                sums = r + g + b;
                int maxR = r, maxG = g, maxB = b, minR = r, minG = g, minB = b;
                max_intensity = sums;
                min_intensity = sums;

                r = ((unsigned char)*dataBefore) * weight1;
                red += r;
                g = ((unsigned char)*(dataBefore + 1)) * weight1;
                green += g;
                b = ((unsigned char)*(dataBefore + 2)) * weight1;
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

                r = ((unsigned char)*(dataBefore + 3)) * weight1;
                red += r;
                g = ((unsigned char)*(dataBefore + 4)) * weight1;
                green += g;
                b = ((unsigned char)*(dataBefore + 5)) * weight1;
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



                r = ((unsigned char)*(data - 3)) * weight1;
                red += r;
                g = ((unsigned char)*(data - 2)) * weight1;
                green += g;
                b = ((unsigned char)*(data - 1)) * weight1;
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

                r = ((unsigned char)*data) * weight2;
                red += r;
                g = ((unsigned char)*(data + 1)) * weight2;
                green += g;
                b = ((unsigned char)*(data + 2)) * weight2;
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

                r = ((unsigned char)*(data + 3)) * weight1;
                red += r;
                g = ((unsigned char)*(data + 4)) * weight1;
                green += g;
                b = ((unsigned char)*(data + 5)) * weight1;
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



                r = ((unsigned char)*(dataAfter - 3)) * weight1;
                red += r;
                g = ((unsigned char)*(dataAfter - 2)) * weight1;
                green += g;
                b = ((unsigned char)*(dataAfter - 1)) * weight1;
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

                r = ((unsigned char)*dataAfter) * weight1;
                red += r;
                g = ((unsigned char)*(dataAfter + 1)) * weight1;
                green += g;
                b = ((unsigned char)*(dataAfter + 2)) * weight1;
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

                r = ((unsigned char)*(dataAfter + 3)) * weight1;
                red += r;
                g = ((unsigned char)*(dataAfter + 4)) * weight1;
                green += g;
                b = ((unsigned char)*(dataAfter + 5)) * weight1;
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

        (*dest) = (*data);
        data++;
        dest++;
        (*dest) = (*data);
        data++;
        dest++;
        (*dest) = (*data);
        data++;
        dest++;
    }
    for (int k = 0; k < m3; ++k) {
        (*dest) = (*data);
        data++;
        dest++;
    }
    unsigned long mn3 = m3 * n;
    for (int k = 0; k < mn3; ++k) {
        (*src1) = (*dest1);
        src1++;
        dest1++;
    }
    free(dest2);
}




void charsToPixels(Image *charsImg, pixel* pixels) {

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
    unsigned long nm = n*m;
    char *data = charsImg->data;
    for (int i = 0; i < nm ; i++) {
        (*pixels).red = (*data);
        data++;
        (*pixels).green = (*data);
        data++;
        (*pixels).blue = (*data);
        data++;
        pixels++;
    }
}

//*******************************************************************
// optimization- copy to pixels2 also to use cache and not use copy
//*******************************************************************
void charsToPixels2(Image *charsImg, pixel* pixels, pixel* pixels2) {

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
    unsigned long nm = n*m;
    char *data = charsImg->data;
    unsigned char red, green, blue;
    for (int i = 0; i < nm ; i++) {
        red = (*data);
        (*pixels).red = red;
        (*pixels2).red = red;
        data++;
        green = (*data);
        (*pixels).green = green;
        (*pixels2).green = green;
        data++;
        blue = (*data);
        (*pixels).blue = blue;
        (*pixels2).blue = blue;
        data++;
        pixels++;
        pixels2++;
    }
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

    //*******************************************************************
    // optimization- calculate pointer add 1 without calculate index
    //*******************************************************************
    unsigned long nm = n*m;
    char *data = charsImg->data;
    for (int i = 0; i < nm ; i++) {
        *data = pixels->red;
        data++;
        *data = pixels->green;
        data++;
        *data = pixels->blue;
        data++;
        pixels++;
    }
}

//void copyPixels(pixel* src, pixel* dst) {
//
//	int row, col;
//	for (row = 0 ; row < m ; row++) {
//
//        //*******************************************************************
//        // optimization- calculate (row*n) only once every row
//        //*******************************************************************
//        int rowN = row*n;
//		for (col = 0 ; col < n - 4 ; col += 4) {
//
//            //*******************************************************************
//            // optimization- calculate (rowN + col) only once
//            //*******************************************************************
//            int position = rowN + col, position2 = position + 1, position3 = position2 + 1, position4 = position3 + 1;
//			dst[position].red = src[position].red;
//			dst[position].green = src[position].green;
//			dst[position].blue = src[position].blue;
//
//            //*******************************************************************
//            // optimization- loop unrolling- do 4 times every iteration
//            //*******************************************************************
//            dst[position2].red = src[position2].red;
//            dst[position2].green = src[position2].green;
//            dst[position2].blue = src[position2].blue;
//
//            dst[position3].red = src[position3].red;
//            dst[position3].green = src[position3].green;
//            dst[position3].blue = src[position3].blue;
//
//            dst[position4].red = src[position4].red;
//            dst[position4].green = src[position4].green;
//            dst[position4].blue = src[position4].blue;
//		}
//
//        for (int i = col; i < n; ++i) {
//
//            //*******************************************************************
//            // optimization- calculate (rowN + col) only once
//            //*******************************************************************
//            int position = rowN + col;
//            dst[position].red = src[position].red;
//            dst[position].green = src[position].green;
//            dst[position].blue = src[position].blue;
//        }
//	}
//}


void doConvolution(Image *image, int kernel[3], int kernelScale, bool filter) {

    pixel* pixelsImg = malloc(m*n*sizeof(pixel));
    pixel* backupOrg = malloc(m*n*sizeof(pixel));

    charsToPixels2(image, pixelsImg, backupOrg);
    //copyPixels(pixelsImg, backupOrg);

    smooth(backupOrg, pixelsImg, kernel, kernelScale, filter);

    pixelsToChars(pixelsImg, image);

    free(pixelsImg);
    free(backupOrg);
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
	int blurKernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[9] = {-1,-1,-1,-1,9,-1,-1,-1,-1};

	if (flag == '1') {	
		// blur image
		//doConvolution(image, blurKernel, 9, false);
        smooth2(image, 1, 1, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		//doConvolution(image, sharpKernel, 1, false);
        smooth2(image, -1, 9, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		//doConvolution(image, blurKernel, 7, true);
        smooth2(image, 1, 1, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		//doConvolution(image, sharpKernel, 1, false);
        smooth2(image, -1, 9, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

