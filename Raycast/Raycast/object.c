//
//  json.c
//  Raycast
//
//  Created by jr2339 on 9/20/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>//fgetc()
#include <string.h> // strdup

#include "object.h"
#include "vector.h"



/*===============================This is Dr.Palmer's code===================*/
//==============================skip the white space==========================//
void skip_ws(FILE* json){
    int c = fgetc(json);
    while(isspace(c)){
        c = fgetc(json);
    }
    ungetc(c, json); // if C is the white space, we put pointer to previous one
}

//==============================================================================//
char* parse_string(FILE* json){
    char buffer[128];
    int i=0;
    int c= fgetc(json);
    if (c != '"') {       //CHECK THE VERY BEGINING
        fprintf(stderr,"String expected\n");
        exit(-1);
    }
    c = fgetc(json);
    while(c != '"'){    // NOTE ADD AWAY TO CHECK FOR END STRING QUOTE
        buffer[i] = c;
        c = fgetc(json);
    }
    buffer[i] = 0;
    return strdup(buffer); // return a malloced buffer
}

//==============================================================================//
/* Read in a scene file and convert the shapes into struct.
 *file_path: the path to the file to be read
 *Return: An array of points to objects structs.
 */






















