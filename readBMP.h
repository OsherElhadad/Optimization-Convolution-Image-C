/*
 *  readBMP.h
 *
 *  Created by Nina Amenta on Sun May 23 2004.
 *  Free to good home!
 *
 */

#ifndef READ_BMP_H_
#define READ_BMP_H_

/* Image type - contains height, width, and RGB data */
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;

/* HSV Image type - contains height, width, and HSV data */
struct HSVimage {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct HSVimage HSVimage;

/* Function that reads in the image; first param is filename, second is image struct */
/* As side effect, sets w and h */
int ImageLoad(char* filename, Image* image);

#endif /* READ_BMP_H_ */
