//
//  main.c
//  Raycasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raycast.h"


int main(int argc, const char * argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Error: main: You must have 4 arguments\n");
        exit(1);
    }
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0) {
        fprintf(stderr, "Error: main: width and height parameters must be > 0\n");
        exit(1);
    }
    
    /*open the input json file*/
    const char *input = argv[3];
    //Error Check
    if (input == NULL) {
        fprintf(stderr, "Error: main: Failed to open input file '%s'\n", argv[3]);
        exit(1);
    }
    read_scene(input);
    printf("Satring get objects...\n");
    get_objects(objects);
    printf("get_objects finished work.....\n");
    
    Image *image =(Image *)malloc(sizeof(Image));
    printf("Creeate a Image\n");
   
    image->width = atoi(argv[1]);
    printf("image width is %d \n",image->width);
    image->height = atoi(argv[2]);
    printf("image height is %d \n",image->height);
    image->maxval = 255;
    image->data =(unsigned char*) malloc(sizeof(unsigned char) * image->width * image->height*4);

    
    
    printf("get_camera function starting work\n");
    int pos = get_camera(objects);
    printf("get_camera function finished work\n");
    raycast_scene(image, objects[pos].data.camera.width, objects[pos].data.camera.height, objects);
    
    const char *output = argv[4];
    //create_ppm(output, 6, &image);
    ImageWrite(image, output,6);
    return 0;
}
