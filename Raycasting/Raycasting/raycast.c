//
//  raycast.c
//  Raycasting
//
//  Created by jr2339 on 9/30/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#include "raycast.h"

/*Get Object camera's information from the json file*/
int get_camera(OBJECT *objects){
    int i=0;
    while(i < NumberOfObjects && objects[i].type !=0){
        if (objects[i].type == CAM) {
            return i;
        }
        i++;
    }
    //we can't find an object which type is cammera
    return -1;
}


/*
 *fill in pixel color to our image
 *need to flip the y axis due to the placement of the viewplane
*/

void shade_pixel(double *color, int row, int col,Image *image){
    row = (image->height-1) - row;
    image->data[row*image->width+col+0] = color[0]; // R
    image->data[row*image->width+col+1] = color[1]; //G
    image->data[row*image->width+col+2] = color[2]; //B
}


/*
 *Ro means Ray original
 *Rd mwans Ray destination
 
 */
double plane_intersection(double *Ro, double *Rd, double *Pos, double *Norm){
    normalize(Norm);
    /*determine if plane is parallel to the ray
     *if it is parallel to the ray, we can't do
     *intersection
     */
    double denominator = Vector_dot(Norm, Rd);
    if(denominator == 0){
        return -1;
    }
    Vector difference;
    Vector_sub(Ro, Pos, difference);
    
    double t = -((Vector_dot(difference, Norm))/denominator);
    
    if (t < 0.0) {
        return -1;
    }
    return t;
}














