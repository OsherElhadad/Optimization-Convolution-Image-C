#include <stdbool.h>

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;

//*******************************************************************
// optimization- inline function, reduce call to min/max on the stack
//*******************************************************************
/* Compute min and max of two integers, respectively */
static inline int min(int a, int b) { return (a < b ? a : b); }
static inline int max(int a, int b) { return (a > b ? a : b); }

//*******************************************************************
// optimization- inline function, reduce call to calcIndex on the stack
//*******************************************************************
static inline int calcIndex(int i, int j, int n) {
	return ((i)*(n)+(j));
}

//*******************************************************************
// optimization- inline function, reduce call to initialize_pixel_sum on the stack
//*******************************************************************
/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static inline void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	int red = sum.red / kernelScale;
	int green = sum.green / kernelScale;
	int blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
    int maxi = (red > 0 ? red : 0);
	current_pixel->red = (unsigned char) (maxi < 255 ? maxi : 255);
    maxi = (green > 0 ? green : 0);
	current_pixel->green = (unsigned char) (maxi < 255 ? maxi : 255);
    maxi = (blue > 0 ? blue : 0);
	current_pixel->blue = (unsigned char) (maxi < 255 ? maxi : 255);
	return;
}

//*******************************************************************
// optimization- inline function, reduce call to sum_pixels_by_weight on the stack
//*******************************************************************
/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static inline void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	// sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {
	int currRow, currCol;
	//pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	//initialize_pixel_sum(&sum);

    //*******************************************************************
    // optimization- reduce using struct pixel_sum
    //*******************************************************************
    int red = 0, green = 0, blue = 0;

    int minIDim = min(i+1, dim-1), minJDim = min(j+1, dim-1);

    int ii = max(i-1, 0), jj;

	for(; ii <= minIDim; ii++) {
        int kRow;

        // compute row index in kernel
        if (ii < i) {
            kRow = 0;
        } else if (ii > i) {
            kRow = 2;
        } else {
            kRow = 1;
        }

        //*******************************************************************
        // optimization- reduce multiple ii*dim
        //*******************************************************************
        int dimMulII = ii*dim;
		for(jj = max(j-1, 0); jj <= minJDim; jj++) {

			int kCol;

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			//sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

            int weight = kernel[kRow][kCol];

            //*******************************************************************
            // optimization- reduce call to calcIndex
            //*******************************************************************
            pixel* p = &src[(dimMulII + jj)];

            //*******************************************************************
            // optimization- reduce call to sum_pixel_by_weight
            //*******************************************************************
            red += ((int) p->red) * weight;
            green += ((int) p->green) * weight;
            blue += ((int) p->blue) * weight;
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = max(i-1, 0); ii <= minIDim; ii++) {

            //*******************************************************************
            // optimization- reduce multiple ii*dim
            //*******************************************************************
            int dimMulII = ii*dim;
			for(jj = max(j-1, 0); jj <= minJDim; jj++) {
				// check if smaller than min or higher than max and update
                //*******************************************************************
                // optimization- reduce call to calcIndex
                //*******************************************************************
				loop_pixel = src[(dimMulII + jj)];

                //*******************************************************************
                // optimization- reduce calculate of sums from 4 times to one.
                //*******************************************************************
                int sums = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
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
			}
		}

        // filter out min and max
        int weight = -1;

        //*******************************************************************
        // optimization- reduce call to calcIndex
        //*******************************************************************
        pixel* p = &src[(min_row*dim + min_col)];

        //*******************************************************************
        // optimization- reduce call to sum_pixel_by_weight
        //*******************************************************************
        red += ((int) p->red) * weight;
        green += ((int) p->green) * weight;
        blue += ((int) p->blue) * weight;

        //*******************************************************************
        // optimization- reduce call to calcIndex
        //*******************************************************************
        p = &src[(max_row*dim + max_col)];

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

    // truncate each pixel's color values to match the range [0,255]
    current_pixel.red = (unsigned char) (min(max(red, 0), 255));
    current_pixel.green = (unsigned char) (min(max(green, 0), 255));
    current_pixel.blue = (unsigned char) (min(max(blue, 0), 255));
	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int i, j;

    //*******************************************************************
    // optimization- calculate (dim - kernelSize / 2) before the loop
    //*******************************************************************
    int until = dim - kernelSize / 2;
	for (i = kernelSize / 2 ; i < until; i++) {
        int dimMulII = i*dim;
		for (j =  kernelSize / 2 ; j < until ; j++) {
			dst[dimMulII + j] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);
		}
	}
}

void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {

        //*******************************************************************
        // optimization- calculate (row*n) only once every row
        //*******************************************************************
        int rowN = row*n;
		for (col = 0 ; col < n ; col++) {

            //*******************************************************************
            // optimization- calculate (rowN + col) only once
            //*******************************************************************
            int position = rowN + col, dataPosition = 3*position;
			pixels[position].red = image->data[dataPosition];
			pixels[position].green = image->data[dataPosition + 1];
			pixels[position].blue = image->data[dataPosition + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {

        //*******************************************************************
        // optimization- calculate (row*n) only once every row
        //*******************************************************************
        int rowN = row*n;
		for (col = 0 ; col < n ; col++) {

            //*******************************************************************
            // optimization- calculate (rowN + col) only once
            //*******************************************************************
            int position = rowN + col, dataPosition = 3*position;
			image->data[dataPosition] = pixels[position].red;
			image->data[dataPosition + 1] = pixels[position].green;
			image->data[dataPosition + 2] = pixels[position].blue;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {

	int row, col;
	for (row = 0 ; row < m ; row++) {

        //*******************************************************************
        // optimization- calculate (row*n) only once every row
        //*******************************************************************
        int rowN = row*n;
		for (col = 0 ; col < n - 4 ; col += 4) {

            //*******************************************************************
            // optimization- calculate (rowN + col) only once
            //*******************************************************************
            int position = rowN + col, position2 = position + 1, position3 = position2 + 1, position4 = position3 + 1;
			dst[position].red = src[position].red;
			dst[position].green = src[position].green;
			dst[position].blue = src[position].blue;

            //*******************************************************************
            // optimization- loop unrolling- do 4 times every iteration
            //*******************************************************************
            dst[position2].red = src[position2].red;
            dst[position2].green = src[position2].green;
            dst[position2].blue = src[position2].blue;

            dst[position3].red = src[position3].red;
            dst[position3].green = src[position3].green;
            dst[position3].blue = src[position3].blue;

            dst[position4].red = src[position4].red;
            dst[position4].green = src[position4].green;
            dst[position4].blue = src[position4].blue;
		}

        for (int i = col; i < n; ++i) {

            //*******************************************************************
            // optimization- calculate (rowN + col) only once
            //*******************************************************************
            int position = rowN + col;
            dst[position].red = src[position].red;
            dst[position].green = src[position].green;
            dst[position].blue = src[position].blue;
        }
	}
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);

	smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	if (flag == '1') {	
		// blur image
		doConvolution(image, 3, blurKernel, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, 3, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

