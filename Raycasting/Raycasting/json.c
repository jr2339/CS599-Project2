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

#define MAX_COLOR_VAL 255       // maximum value to use for colors 0-255

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


// expect_c() checks that the next character is d.
// It is not d, give us an error.
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
/* since we could use 0-255 or 0-1 or whatever, this function checks bounds */
int check_color_val(double v) {
    if (v < 0.0 || v > MAX_COLOR_VAL)
        return 0;
    return 1;
}
/* Checks that the next 3 values in the FILE are valid rgb numbers */
double* next_rgb_color(FILE* json) {
    double* v = malloc(sizeof(double)*3);
    skip_ws(json);
    expect_c(json, '[');
    skip_ws(json);
    v[0] = MAX_COLOR_VAL * next_number(json);
    skip_ws(json);
    expect_c(json, ',');
    skip_ws(json);
    v[1] = MAX_COLOR_VAL * next_number(json);
    skip_ws(json);
    expect_c(json, ',');
    skip_ws(json);
    v[2] = MAX_COLOR_VAL * next_number(json);
    skip_ws(json);
    expect_c(json, ']');
    // check that all values are valid
    printf("rgb: %lf %lf %lf\n", v[0], v[1], v[2]);
    if (!check_color_val(v[0]) ||
        !check_color_val(v[1]) ||
        !check_color_val(v[2])) {
        fprintf(stderr, "Error: next_rgb_color: rgb value out of range: %d\n", line);
        exit(1);
    }
    return v;
}
// This function at here is for quadric
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
    }
    skip_ws(json);
    expect_c(json, ']');
    return v;
}

// Parasing the Json file
void read_scene(FILE* json) {
    
    if (json == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
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
            printf("***************** We find A New Object. Now, we will parsing it *****************\n");
            printf("*********************************************************************************\n");
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
            int object_type;
            if (strcmp(type, "camera") == 0) {
                printf("============================== This Object is camera =============================\n");
                object_type = CAM;
                objects[counter].type = CAM;
            }
            else if (strcmp(type, "sphere") == 0) {
                printf("============================== This Object is sphere =============================\n");
                object_type = SPH;
                objects[counter].type = SPH;
            }
            else if (strcmp(type, "plane") == 0) {
                printf("============================== This Object is plane ==============================\n");
                object_type = PLAN;
                objects[counter].type = PLAN;
            }
            else if (strcmp(type, "cylinder") == 0) {
                printf("============================== This Object is plane ==============================\n");
                object_type = CYLIN;
                objects[counter].type = CYLIN;
            }
            else if (strcmp(type, "quadric") == 0) {
                printf("============================= This Object is quadric =============================\n");
                object_type = QUAD;
                objects[counter].type = QUAD;
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
                        if (object_type == SPH)
                            objects[counter].data.sphere.radius = next_number(json);
                        else if (object_type == CYLIN)
                            objects[counter].data.cylinder.radius = next_number(json);
                    }
                    else if (strcmp(key, "color") == 0) {
                        if (object_type == SPH)
                            objects[counter].data.sphere.color = next_rgb_color(json);
                        else if (object_type == PLAN)
                            objects[counter].data.plane.color = next_rgb_color(json);
                        else if (object_type == CYLIN)
                            objects[counter].data.cylinder.color = next_rgb_color(json);
                        else if (object_type == QUAD)
                            objects[counter].data.quadric.color = next_rgb_color(json);
                        else {
                            fprintf(stderr, "Error: read_json: Color vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                    }
                    else if (strcmp(key, "position") == 0) {
                        if (object_type == SPH)
                            objects[counter].data.sphere.position = next_vector(json);
                        else if (object_type == PLAN)
                            objects[counter].data.plane.position = next_vector(json);
                        else if (object_type == CYLIN)
                            objects[counter].data.cylinder.position = next_vector(json);
                        else {
                            fprintf(stderr, "Error: read_json: Position vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                        
                    }
                    else if (strcmp(key, "normal") == 0) {
                        if (object_type != PLAN) {
                            fprintf(stderr, "Error: read_json: Normal vector can't be applied here: %d\n", line);
                            exit(1);
                        }
                        else
                            objects[counter].data.plane.normal = next_vector(json);
                    }
                    
                    else if (strcmp(key, "coefficient") == 0) {
                        if (object_type != QUAD) {
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

//Get Objects
void get_objects(OBJECT *object) {
    int i = 0;
    while (i < NumberOfObjects && object[i].type > 0) {
        printf("object type: %d\n", object[i].type);
        if (object[i].type == CAM) {
            printf("height: %lf\n", object[i].data.camera.height);
            printf("width: %lf\n", object[i].data.camera.width);
        }
        else if (object[i].type == SPH) {
            printf("color: %lf %lf %lf\n", object[i].data.sphere.color[0],
                   object[i].data.sphere.color[1],
                   object[i].data.sphere.color[2]);
            printf("position: %lf %lf %lf\n", object[i].data.sphere.position[0],
                   object[i].data.sphere.position[1],
                   object[i].data.sphere.position[2]);
            printf("radius: %lf\n", object[i].data.sphere.radius);
        }
        else if (object[i].type == PLAN) {
            printf("color: %lf %lf %lf\n", object[i].data.plane.color[0],
                   object[i].data.plane.color[1],
                   object[i].data.plane.color[2]);
            printf("position: %lf %lf %lf\n", object[i].data.plane.position[0],
                   object[i].data.plane.position[1],
                   object[i].data.plane.position[2]);
            printf("normal: %lf %lf %lf\n", object[i].data.plane.normal[0],
                   object[i].data.plane.normal[1],
                   object[i].data.plane.normal[2]);
        }
        else {
            printf("unsupported value\n");
        }
        i++;
    }
    printf("end at i=%d\n", i);
}
