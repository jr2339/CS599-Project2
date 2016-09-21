//
//  ppm.h
//  Raycast
//
//  Created by jr2339 on 9/21/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#ifndef ppm_h
#define ppm_h

#include <stdio.h>

typedef struct{
    unsigned char r,g,b;
} pixel;

typedef struct {
    int height, width, maxval;
    pixel** data;
}Image;

void write_image(Image*, char*);
#endif /* ppm_h */
