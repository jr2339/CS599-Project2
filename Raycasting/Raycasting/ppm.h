//
//  ppm.h
//  Project1
//
//  Created by jr2339 on 9/7/16.
//  Copyright © 2016 jr2339. All rights reserved.
//



#ifndef ppm_h
#define ppm_h
#include <sys/types.h>

// Create a new type to define Image
typedef struct Image
{
    int width;
    int height;
    u_char *data; // color
    int magic_number;
}Image;

Image *ImageCreate(int width, int height,int magic_number);
void readPPMHeader(FILE *f_source, int *width, int *height,int *magic_address);
Image *ImageRead(const char *filename);
void ImageWrite(Image *image, const char *filename,int format);

#include <sys/types.h>
// Create a new type to define Image


#endif /* ppm_h */
