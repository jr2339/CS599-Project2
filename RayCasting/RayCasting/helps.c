//
//  helps.c
//  RayCasting
//
//  Created by jr2339 on 9/21/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helps.h"
/**********************This is Dr.Palmer's code*******************/
int skip_ws(FILE* json){
    int c, count=0;
    c = fgetc(json);
    while(isspace(c)){
        c = fgetc(json);
        count++;
    }
    ungetc(c, json);
    return count;
}


char* parse_string(FILE* json){
    char buffer[128];
    int i=0;
    int c = fgetc(json);
    if(c!='"'){
        fprintf(stderr,"String expected!\n");
        exit(-1);
    }
    c=fgetc(json);
    while(c!='"'){
        buffer[i]=c;
        i +=1;
        c = fgetc(json);
    }
    buffer[i]=0;
    //return a malloced buffer
    return strdup(buffer);
}

/*
 *Convet height, width into x and y coordinates
 *which means we convert 2D array to 1D
*/
int convert_wxy_index(int width, int x, int y){
    return x+y*width;
}

/*
 *Convert index to height and width
 *looks like convert 1D array to 2D array
*/
void convert_index_xwy(int index, int width, int *x, int *y){
    *x = index % width;
    *y = index /width;
}





















