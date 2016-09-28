//
//  json.h
//  Raycasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//


#ifndef json_h
#define json_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/********************Here is what I am doing for the first time******************
typedef struct Object{
    char *type;
    float width;
    float height;
    float radius;
    float position[3];
    float color[3];
    float normal[3];
    float coefficient[10]; // this is for quadric
}Object;
*******************************************************************************/
/*****************************Dr.Palmer's Structure**********************/
/*Each Structure is a type of Objects in Jason File*/
typedef struct CAMERA {
    double width;
    double height;
} CAMERA;


typedef struct SPHERE {
    double *color;
    double *position;
    double radius;
} SPHERE;



typedef struct PLANE {
    double *color;
    double *position;
    double *normal;
} PLANE;


typedef struct QUADRIC {
    double *color;
    double *coefficient;
} QUADRIC;

/*DEFINE STRUCTURE  FOR JASON FILE WHICH STROE OUR OBJECT*/
typedef struct OBJECT{
    char type[10];
    union {
        CAMERA camera;
        SPHERE sphere;
        PLANE plane;
        QUADRIC quadric;
    } data;
} OBJECT;



typedef enum OBJECT_TYPE{
    camera,
    sphere,
    plane,
    quadric
} OBJECT_TYPE;










#endif /* json_h */
