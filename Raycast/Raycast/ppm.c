//
//  ppm.c
//  Raycast
//
//  Created by jr2339 on 9/21/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"
/*******************This code from project1*****************************
 *Write an image out to a buffer in PPM format 
 *Will exit if file could not be opened for writing
 *PPM: Pointer to an image struct containing the image data to be written
 *buffer_path: String containing the file path taht should be written to
 ***********************************************************************/
void write_image(Image *PPM, char *buffer_path){
    FILE *buffer;
    if ((buffer=fopen(buffer_path,"w"))!=0) {
        fprintf(stderr, "There was a problem opening the output file %s.\n", buffer_path);
        exit(1);
    }
    fprintf(buffer,"P6\n");
    
    fprintf(buffer, "%d\n", PPM->width);
    fprintf(buffer, "%d\n", PPM->height);
    fprintf(buffer, "%d\n", PPM->maxval);
    
    for (int i = 0; i < PPM->height; i++) {
        for (int j = 0; j < PPM->width; j++) {
            fputc(PPM->data[i][j].r, buffer);
            fputc(PPM->data[i][j].g, buffer);
            fputc(PPM->data[i][j].b, buffer);
        }
    }
    
    fclose(buffer);
}


