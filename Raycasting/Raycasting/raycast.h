//
//  raycast.h
//  Raycasting
//
//  Created by jr2339 on 9/30/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#ifndef raycast_h
#define raycast_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json.h"
#include "vector.h"
#include "raycast.h"
#include "ppm.h"

typedef struct ray{
    double origin[3];
    double direction[3];
} ray;

int get_camera(OBJECT *objects);
void shade_pixel(double *color, int row, int col,Image *image);
double plane_intersection(double *Ro, double *Rd, double *Pos, double *Norm);
double sphere_intersection(double *Ro, double *Rd, double *C, double r);
void raycast_scene(Image *image, double cam_width, double cam_height, OBJECT *object);


#endif /* raycast_h */
