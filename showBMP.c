/*
 *  showBMP.c
 *
 *  Nina Amenta, May 2004
 *  No copyright - free to good home!
 *
 */

//sudo apt-get install freeglut3-dev
//http://www.cs.ucdavis.edu/~amenta/s04/image/files.html
#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include "readBMP.h"
#include <string.h>

#include "writeBMP.h"

// Measure Performance
#include <sys/time.h>
#include <sys/resource.h>

/* ascii code for the escape key */
#define ESCAPE 27

// title for result image window
#define WINDOW_TITLE "result"

int window;  // id of glut window
Image *image; // data structure for image
unsigned long n, m; // width and height
char picName[80];
char flag; // chooses which kernel to execute

void getImage(char* filename) {

	// allocate space for image data structure
	image = (Image *) malloc(sizeof(Image));
	if (image == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}

	if (!ImageLoad(filename, image)) {
		exit(1);
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	//printf("n=%ld,m=%ld\n",n,m);
	glDrawPixels(n, m, GL_RGB, GL_UNSIGNED_BYTE, image->data);
	glFlush();
}

void myreshape(int h, int w) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) n, 0.0, (GLfloat) m);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, h, w);
}

#include "myfunction.c"
char* blurRsltImgName = "Blur.bmp";
char* sharpRsltImgName = "Sharpen.bmp";
char* filteredBlurRsltImgName = "Filtered_Blur.bmp";
char* filteredSharpRsltImgName = "Filtered_Sharpen.bmp";

void optimize(Image* image, char flag) {

	struct timeval startTime;
	struct timeval endTime;
	struct rusage ru;
	getrusage(RUSAGE_SELF, &ru); // start timer
	startTime = ru.ru_utime;

	myfunction(image, picName, blurRsltImgName, sharpRsltImgName, filteredBlurRsltImgName, filteredSharpRsltImgName, flag);

	getrusage(RUSAGE_SELF, &ru); // end timer
	endTime = ru.ru_utime;
	double tS = startTime.tv_sec * 1000000.0 + (startTime.tv_usec);
	double tE = endTime.tv_sec * 1000000.0 + (endTime.tv_usec);
	printf("Total runtime: %f ms\n", (tE - tS) / 1000.0);
}

static void key(unsigned char c, int x, int y) {
	switch (c) {
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char **argv) {

	getImage(argv[1]);
	n = image->sizeX; // width
	m = image->sizeY; // height
	char buffer[256];
	if (argc != 3) {
		printf("please specify a kernel option\n");
		return 0;
	}
	flag = argv[2][0];
	if (flag == '1') {
		printf("kernel number 1 was chosen\n");
	}
	else {
		printf("kernel number 2 was chosen\n");
	}
	
	

	// get pic name
	strcpy(picName, argv[1]);
	picName[strlen(argv[1])] = 0;

	// build window title
	sprintf(buffer, "%s of %s", WINDOW_TITLE, picName);

	optimize(image, flag);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(n, m);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(buffer);
	glutReshapeFunc(myreshape);
	glutKeyboardFunc(key);
	glutDisplayFunc(display);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutMainLoop();
	return (0);
}
