#include "writeBMP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeBMP(Image *image, const char* originalImgFileName, const char* fileName) {

	// open the file to be written
	FILE * bmpfile;
	bmpfile = fopen(fileName, "wb");
	if (bmpfile == NULL) {
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
	int bytesPerLine;
	bytesPerLine = image->sizeX * 3;  // for 24 bit images)
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
	int line;
	int i;
	char* iData = image->data;
	for (line = 0; line < image->sizeY; ++line) {

		/*
		* fill line linebuf with the image data for that line
		* remember that the order is BGR
		*/
		for (i = 0 ; i < bytesPerLine ; i += 3) {

			linebuf[i] = iData[line*bytesPerLine + i + 2];
			linebuf[i + 1] = iData[line*bytesPerLine + i  + 1];
			linebuf[i + 2] = iData[line*bytesPerLine + i];
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

