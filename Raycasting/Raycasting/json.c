//
//  json.c
//  Raycasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<errno.h>

#include "json.h"
/*******************************************This is Dr.Palmer's code*******************************/
// next_c wraps the getc function that provides error checking and line #
// Problem: if we do ungetc, it could screw us up on the line #
#define NumberOfObjects 20 // In our Json File, we have 20 Objects

int line = 1;  // global variable, it will tells us which line is not correct

OBJECT objects[NumberOfObjects]; // Allocate an array for All Objects in Json File

// next_c() wraps the getc() function and provides error checking and line
// number maintenance
int next_c(FILE* json) {
    int c = fgetc(json);
#ifdef DEBUG
    printf("next_c: '%c'\n", c);
#endif
    if (c == '\n') {
        line++;;
    }
    if (c == EOF) {
        fprintf(stderr, "Error: next_c: Unexpected EOF: %d\n", line);
        exit(1);
    }
    return c;
}


// expect_c() checks that the next character is d.  If it is not it emits
// an error.
void expect_c(FILE* json, int d) {
    int c = next_c(json);
    if (c == d) return;
    fprintf(stderr, "Error: Expected '%c': %d\n", d, line);
    exit(1);
}


// skip_ws() skips white space in the file.
void skip_ws(FILE* json) {
    int c = next_c(json);
    while (isspace(c)) {
        c = next_c(json);
    }
    ungetc(c, json);
}


// next_string() gets the next string from the file handle and emits an error
// if a string can not be obtained.
char* next_string(FILE* json) {
    char buffer[129];
    int c = next_c(json);
    if (c != '"') {
        fprintf(stderr, "Error: Expected string on line %d.\n", line);
        exit(1);
    }
    c = next_c(json);
    int i = 0;
    while (c != '"') {
        if (i >= 128) {
            fprintf(stderr, "Error: Strings longer than 128 characters in length are not supported.\n");
            exit(1);
        }
        if (c == '\\') {
            fprintf(stderr, "Error: Strings with escape codes are not supported.\n");
            exit(1);
        }
        if (c < 32 || c > 126) {
            fprintf(stderr, "Error: Strings may contain only ascii characters.\n");
            exit(1);
        }
        buffer[i] = c;
        i += 1;
        c = next_c(json);
    }
    buffer[i] = 0;
    return strdup(buffer);
}

double next_number(FILE* json) {
    double value;
    fscanf(json, "%lf", &value);
    // Error check this..
    if (value == EOF) {
        fprintf(stderr, "Error: Expected a number but found EOF: %d\n", line);
        return -1;;
    }
    printf("next_number: %lf\n", value);
    return value;
}

double* next_vector(FILE* json) {
    double* v = malloc(3*sizeof(double));
    expect_c(json, '[');
    skip_ws(json);
    v[0] = next_number(json);
    skip_ws(json);
    expect_c(json, ',');
    skip_ws(json);
    v[1] = next_number(json);
    skip_ws(json);
    expect_c(json, ',');
    skip_ws(json);
    v[2] = next_number(json);
    skip_ws(json);
    expect_c(json, ']');
    return v;
}

double* next_coefficient(FILE* json){
    double* v = malloc(10*sizeof(double));
    expect_c(json, '[');
    skip_ws(json);
    v[0] = next_number(json);
    for(int i=1;i<10;i++){
        skip_ws(json);
        expect_c(json, ',');
        skip_ws(json);
        v[i] = next_number(json);
        printf("the v %d is %f\n",i,v[i]);
    }
    skip_ws(json);
    expect_c(json, ']');
    return v;
}

void read_scene(char* filename) {
    FILE* json = fopen(filename, "r");
    
    if (json == NULL) {
        fprintf(stderr, "Error: Could not open file \"%s\"\n", filename);
        exit(1);
    }
    skip_ws(json);
    
    // find beginning of the list
    int c  = next_c(json);
    if (c != '[') {
        fprintf(stderr, "Error: read_json: JSON file must begin with [\n");
        exit(1);
    }
    skip_ws(json);
    c = next_c(json);
    
    // check if file empty
    if (c == ']' || c == EOF) {
        fprintf(stderr, "Error: read_json: Empty json file\n");
        exit(1);
    }
    skip_ws(json);
    
    int counter = 0;
    
    // find the objects
    while (1) {
        //c  = next_c(json);
        if (counter > NumberOfObjects) {
            fprintf(stderr, "Error: read_json: Number of objects is too large: %d\n", line);
            exit(1);
        }
        if (c == ']') {
            fprintf(stderr, "Error: read_json: Unexpected ']': %d\n", line);
            fclose(json);
            return;
        }
        if (c == '{') {     // found an object
            printf("parsing object...\n");
            skip_ws(json);
            char *key = next_string(json);
            if (strcmp(key, "type") != 0) {
                fprintf(stderr, "Error: read_json: First key of an object must be 'type': %d\n", line);
                exit(1);
            }
            skip_ws(json);
            // get the colon
            expect_c(json, ':');
            skip_ws(json);
            
            char *type = next_string(json);
            OBJECT_TYPE object_type;
            printf("type is '%s'\n", type);
            if (strcmp(type, "camera") == 0) {
                printf("found camera...\n");
                object_type = camera;
                strcpy(objects[counter].type, "camera");
                printf("============\n");
                
            }
            else if (strcmp(type, "sphere") == 0) {
                printf("found sphere...\n");
                object_type = sphere;
                strcpy(objects[counter].type, "sphere");
            }
            else if (strcmp(type, "plane") == 0) {
                printf("found plane...\n");
                object_type = plane;
                strcpy(objects[counter].type, "plane");
            }
            else if (strcmp(type, "quadric") == 0) {
                printf("found quadric...\n");
                object_type = quadric;
                strcpy(objects[counter].type, "quadric");
            }
            else {
                exit(1);
            }
            
            skip_ws(json);
            
            while (1) {
                //  , }
                c = next_c(json);
                if (c == '}') {
                    // stop parsing this object
                    break;
                }
                else if (c == ',') {
                    // read another field
                    skip_ws(json);
                    char* key = next_string(json);
                    skip_ws(json);
                    expect_c(json, ':');
                    skip_ws(json);
                    if (strcmp(key, "width") == 0) {
                        objects[counter].data.camera.width = next_number(json);
                    }
                    else if (strcmp(key, "height") == 0) {
                        objects[counter].data.camera.height = next_number(json);
                    }
                    else if (strcmp(key, "radius") == 0) {
                        objects[counter].data.sphere.radius = next_number(json);
                    }
                    else if (strcmp(key, "color") == 0) {
                        if (object_type == sphere)
                            objects[counter].data.sphere.color = next_vector(json);
                        else if (object_type == plane)
                            objects[counter].data.plane.color = next_vector(json);
                        else if (object_type == quadric)
                            objects[counter].data.quadric.color = next_vector(json);
                        else {
                            fprintf(stderr, "Error: read_json: Color vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                    }
                    else if (strcmp(key, "position") == 0) {
                        if (object_type == sphere)
                            objects[counter].data.sphere.position = next_vector(json);
                        else if (object_type == plane)
                            objects[counter].data.plane.position = next_vector(json);
                        else {
                            fprintf(stderr, "Error: read_json: Position vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                        
                    }
                    else if (strcmp(key, "normal") == 0) {
                        if (object_type != plane) {
                            fprintf(stderr, "Error: read_json: Normal vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                        else
                            objects[counter].data.plane.normal = next_vector(json);
                    }
                    
                    else if (strcmp(key, "coefficient") == 0) {
                        if (object_type != quadric) {
                            fprintf(stderr, "Error: read_json: Normal vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                        else
                            objects[counter].data.quadric.coefficient = next_coefficient(json);
                        exit(1);
                    }
                    else {
                        fprintf(stderr, "Error: read_json: '%s' not a valid object: %d\n", key, line);
                        exit(1);
                    }
                    
                    // do something with key and value
                    skip_ws(json);
                }
                else {
                    fprintf(stderr, "Error: read_json: Unexpected value '%c': %d\n", c, line);
                    exit(1);
                }
            }
            skip_ws(json);
            c = next_c(json);
            if (c == ',') {
                // noop
                skip_ws(json);
            }
            else if (c == ']') {
                printf("end of file\n");
                fclose(json);
                return;
            }
            else {
                fprintf(stderr, "Error: read_json: Expecting comma or ]: %d\n", line);
                exit(1);
            }
        }
        c = next_c(json);
        counter++;
    }
}












