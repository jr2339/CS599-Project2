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
    double alph,delta;
    normalize(Norm);
    /*
     *if the plane is parallel to the ray
     * return -1;
     */
    Vector incident_vector;
    Vector_sub(Pos, Ro, incident_vector);
    
    alph = Vector_dot(Norm, incident_vector); //length(Norm) * length(incident_vector)*cos(thea1)
    
    // the plane is parallel to the ray
    if (fabs(alph) <0.0001) {
        return -1;
    }
    
    
    delta = Vector_dot(Norm, Rd); //length(Norm) * length(Rd) * cos(thea2)
    
    
    double t = alph/delta; // whcih means we check thea1 and thea2
    
    if (t>0.0) { // reflection, no intersection
        return -1;
    }
    
    
    return t; // return something, but not t , need to figure out it

    
    }





























