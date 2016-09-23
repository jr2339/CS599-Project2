//
//  helpers.c
//  Raycasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
/****************************Code from Dr.palmer and Project1****************************/
#include <ctype.h>
#include "helpers.h"
/****************************Code from Dr.palmer and Project1****************************/
void skip_ws(FILE* json)
{
    
    int c = fgetc(json);
    while(isspace(c))
    {
        c = fgetc(json);
    }
    ungetc(c, json);
}
/******************************************************************************************
 
 *******************************************************************************************/
long wxy_to_index(int width, int x, int y) {
    return (long)x + ((long)y)*((long)width);
}

void index_to_xwy(long index, int width, int* x, int* y) {
    *x = (int)(index % width);
    *y = (int)(index / width);
}
