//
//  image.h
//  RayCasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
/*************************This code from Project1*****************/
#ifndef image_h
#define image_h

#include <stdio.h>
typedef  struct {
    u_char r,g,b;
}Color;

typedef struct {
    char type;
    unsigned int width;
    unsigned int height;
    unsigned int maxval;
}Header;

typedef struct{
    Header header;
    Color  *pixels;
}Image;

void Create_Image(Image *PPM, int width, int height, Color color);
#endif /* image_h */
