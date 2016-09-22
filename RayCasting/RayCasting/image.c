//
//  image.c
//  RayCasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include "image.h"

/***************************This code from Project1***********************/
/*
 * We will create a blank image which is a buffer to store Image
*/
void Create_Image(Image *PPM, int width, int height, Color color){
    /*******We set Header**********/
    PPM->header.width = width;
    PPM->header.height = height;
    PPM->header.maxval = 255 ;// Default value
    /**********We get value for color*****/
    PPM->pixels = malloc(sizeof(Color)*height*width);
    if(PPM->pixels == NULL){
        fprintf(stderr, "Error: Unable to allocate enough memory to store an image of size %dx%d.\n", width, height);
        exit(-1);
    }
    /***************************Set the background color******************/
    for(int i=0;i<width*height;i++){
        PPM->pixels[i] = color;
    }
    
}
