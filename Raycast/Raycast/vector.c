//
//  vector.c
//  Raycast
//
//  Created by jr2339 on 9/20/16.
//  Copyright © 2016 jr2339. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h"
#include "json.h"

/******************************************************
 Initializes three float values into a given vector
 Dr.Palmer also give this code to us in class
*******************************************************/

void vector_init(float x, float y, float z, float *out){
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

//====================================================================
float v_magnitude(float *point){
    return sqrtf(point[0]*point[0]+point[1]*point[1]+point[2]*point[2]);
}
//====================================================================
//vector_unit gives us a direction
void v_unit(float *in, float *out){
    float mag = v_magnitude(in);
    out[0] = in[0]/mag;
    out[1] = in[1]/mag;
    out[2] = in[2]/mag;
    
}
//====================================================================
void v_add(float *f1, float *f2, float *out){
    out[0] = f1[0] + f2[0];
    out[1] = f1[1] + f2[1];
    out[2] = f1[2] + f2[2];
}
//====================================================================
void v_sub(float *f1, float *f2, float *out) {
    out[0] = f1[0] - f2[0];
    out[1] = f1[1] - f2[1];
    out[2] = f1[2] - f2[2];
}
//====================================================================
void v_scale(float *v, float s, float *out) {
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    out[2] = v[2] * s;
}
//====================================================================
float v_dot(float *v1, float *v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
//====================================================================
void v_cross(float *v1, float *v2, float *out) {
    out[0] = v1[1]*v2[2] - v1[2]*v2[1];
    out[1] = v1[2]*v2[0] - v1[0]*v2[2];
    out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
//====================================================================
/*Get the normal of a shape at a given position
 *the_sphere/the_plane: the camera/sphere/plane to get the normal of
 *position: The position on the object to calculate the normal
 *returns: The normal to the camera/sphere/plane at the given position
======================================================================*/
float* sphere_get_normal(sphere *the_sphere, float *position){
    float *normal;
    if(strcmp(the_sphere->type,"sphere")==0){
        normal = (float*)malloc(sizeof(float)*3);
        v_sub(position,the_sphere->center,normal);
        v_unit(normal, normal);
    }
    else{
        fprintf(stderr,"Can't get normal for sphere\n");
        exit(-1);
    }
    return normal;
}


float* plane_get_normal(plane *the_plane, float *position){
    float *normal;
    if(strcmp(the_plane->type,"plane")==0){
        normal = the_plane->normal;
    }
    else{
        fprintf(stderr,"Can't get normal for plane\n");
        exit(-1);
    }
    return normal;
}
//====================================================================
/*Compute the reflection of a vector on an object
 *he_sphere/the_plane: he_sphere/the_plane to reflect off
 *position: The position where lights hit
 *Returns: the reflection of direction on the object at the position
======================================================================*/
float* sphere_reflection(sphere *the_sphere, float *position, float *direction){
    float *reflection = (float*)malloc(sizeof(float)*3);
    float *normal = sphere_get_normal(the_sphere, position);
    v_scale(normal, -2*v_dot(direction, normal), reflection); //−2(d⋅n)n
    v_add(reflection, direction, reflection);                 //r=d−2(d⋅n)n
    v_unit(reflection, reflection);
    return reflection;
}


float* plane_reflection(plane *the_plane, float *position, float *direction){
    float *reflection = (float*)malloc(sizeof(float)*3);
    float *normal = plane_get_normal(the_plane, position);
    v_scale(normal, -2*v_dot(direction, normal), reflection);
    v_add(reflection, direction, reflection);
    v_unit(reflection, reflection);
    return reflection;
}






