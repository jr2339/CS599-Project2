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
    //printf("shade_pixel starting works\n");
    image->data[row * image->width*4 + col*4] = (u_char)color[0];
    image->data[row * image->width*4 + col*4+1] = (u_char)color[1];
    image->data[row * image->width*4 + col*4+2]= (u_char)color[2];
    image->data[row * image->width*4 + col*4+3]= 255;
    //printf("shade_pixel finished works\n");
}


/*
 *Ro means Ray original
 *Rd mwans Ray destination
 
 */
double plane_intersection(double *Ro, double *Rd, double *Pos, double *Norm){
    //printf("plane_intersection starting works\n");
    double alph,delta;
    normalize(Norm);
    /*
     *if the plane is parallel to the ray
     * return -1;
     */
    alph = Vector_dot(Norm, Rd);
    
    // the plane is parallel to the ray
    if (fabs(alph) <0.0001) {
        return -1;
    }
    
    Vector incident_vector;
    Vector_sub(Pos, Ro, incident_vector);
    delta = Vector_dot(incident_vector, Norm);
    
    
    double t = delta/alph; // whcih means we check thea1 and thea2
    
    if (t<0.0) { // reflection, no intersection
        return -1;
    }
    
    //printf("plane_intersection finished works\n");
    return t; // return something, but not t , need to figure out it

    
    }

double sphere_intersection(double *Ro, double *Rd, double *C, double r){
    //printf("sphere_intersection starting works\n");
    double a, b, c;
    double t0,t1; // we have two solutions if delta >0
    //calculate quadratic formula
    //First find a, b, c
    a = sqr(Rd[0]) + sqr(Rd[1]) + sqr(Rd[2]);
    b = 2 * (Rd[0]*(Ro[0]-C[0]) + Rd[1]*(Ro[1]-C[1]) + Rd[2]*(Ro[2]-C[2]));
    c = sqr(Ro[0]-C[0]) + sqr(Ro[1]-C[1]) + sqr(Ro[2]-C[2]) - sqr(r);
     // make sure a is 1 (means the ray direction was normalized)
    if (a > 1.0001 || a < .9999) {
        printf("a = %lf\n", a);
        fprintf(stderr, "Ray direction was not normalized\n");
        exit(1);
    }
    // check that discriminant is <, =, or > 0
    double disc = sqr(b) - 4*a*c;
    
    if (disc < 0) {
        //printf("disc was < 0\n");
        return -1; // no solution
    }
    else if (disc == 0) {
        t0 = -1*(b / (2*a)); // single solution
        t1 = -1*(b / (2*a));
        //printf("t0 = %lf\n", t0);
    }
    else {  // 2 solutions: find the smaller
        t0 = (-1*b - sqrt(sqr(b) - 4*c))/2;
        t1 = (-1*b + sqrt(sqr(b) - 4*c))/2;

    }
    
    
    if (t0 < 0 && t1 < 0) {
        // no intersection
        return -1;
    }
    else if (t0 < 0 && t1 > 0) {
        return t1;
    }
    else if (t0 > 0 && t1 < 0) {
        return t0;
    }
    else { // they were both positive
        if (t0 <= t1)
            return t0;
        else
            return t1;
    }
    
}

 double quadric_intersection(double *Ro, double *Rd, double *Co){
    double a,b,c;
    double t0,t1; // we have two solutions if delta >0
     a = Co[0]*sqr(Rd[0]) //A(xd)^2
        +Co[1]*sqr(Rd[1]) //B(yd)^2
        +Co[2]*sqr(Rd[2]) //C(zd)^2
        +Co[3]*Rd[0]*Rd[1] //D * xd *yd
        +Co[4]*Rd[0]*Rd[2] //E * xd *zd
        +Co[5]*Rd[1]*Rd[2];//F *yd *zd
     //printf("quadric a = %lf\n", a);
     b = 2*Co[0]*Ro[0]*Rd[0]   // 2*A*xo*xd
        +2*Co[1]*Ro[1]*Rd[1]   //2*B*yo*yd
        +2*Co[2]*Ro[2]*Rd[2]   //2*C*zo*zd
        +  Co[3]*((Ro[0]*Rd[1])+(Ro[1]*Rd[0]))   //D*((xo*yd)+(yo*xd))
        +  Co[4]*((Ro[0]*Rd[2])+(Ro[2]*Rd[0]))  //E*(xo*zd+zo*xd)
        +  Co[5]*((Ro[1]*Rd[2])+(Rd[1]*Ro[2]))  //F*(yo*zd+yd*zo)
        +  Co[6]*Rd[0]          //G*xd
        +  Co[7]*Rd[1]          //H*yd
        +  Co[8]*Rd[2];         //I*zd
     //printf("quadric b = %lf\n", b);
     c = Co[0]*sqr(Ro[0])       //A*xo^2
        +Co[1]*sqr(Ro[1])       //B*yo^2
        +Co[2]*sqr(Ro[2])       //C*zo^2
        +Co[3]*Ro[0]*Ro[1]      //D*xo*yo
        +Co[4]*Ro[0]*Ro[2]      //E*xo*zo
        +Co[5]*Ro[1]*Ro[2]      //F*yo*zo
        +Co[6]*Ro[0]            //G*xo
        +Co[7]*Ro[1]            //H*yo
        +Co[8]*Ro[2]            //I*zo
        +Co[9];                 //J
     //printf("quadric c = %lf\n", c);
     if (a > 0.00001 || a < -0.00001) { // if a !=0
         // check that discriminant is <, =, or > 0
         double disc = sqr(b) - 4*a*c;
         if (disc < 0) {
             //printf("disc was < 0\n");
             return -1; // no solution
         }
         else{
             t0 = (-1*b - sqrt(sqr(b) - 4*a*c))/(2*a);
             t1 = (-1*b + sqrt(sqr(b) - 4*a*c))/(2*a);
         }

     }
     else{
         t0 = -c/b;
         t1 = -c/b;
     }
     //We campare t0 and t1
     if (t0 < 0 && t1 < 0) {
         // no intersection
         return -1;
     }
     else if (t0 < 0 && t1 > 0) {
         return t1;
     }
     else if (t0 > 0 && t1 < 0) {
         return t0;
     }
     else { // they were both positive
         if (t0 <= t1)
             return t0;
         else
             return t1;
     }

     
}
 
 

void raycast_scene(Image *image, double cam_width, double cam_height, OBJECT *objects){
    // loop over all pixels and test for intesections with objects
    //store results in image->data which store pixels
    int x; // x coordinate iterator
    int y; // y coordinate iterator
    int counter; // object iterator
    
    Vector vp_pos= {0,0,1}; //view plane position
    Vector Ro = {0,0,0}; // Ray origin position or Camera position
    Vector point = {0,0,0}; //point on viewplane where intersection happens
    
    double pixheight = cam_height / image->height;
    double pixwidth = cam_width / image->width;
    
    Vector Rd = {0,0,0}; // direction of Ray
    point[2] = vp_pos[2]; // set intersecting point Z to viewplane Z
    
    for(x=0;x<image->height;x++){
        point[1] = -(vp_pos[1] - cam_height/2.0 + pixheight*(x + 0.5)); // need more math information at here
        for (y=0; y<image->width; y++) {
             point[0] = vp_pos[0] - cam_width/2.0 + pixwidth*(y + 0.5);
            normalize(point);   //normalize the point
            //store normalized point as our direction
            Rd[0] = point[0];
            Rd[1] = point[1];
            Rd[2] = point[2];
            
            int best_counter =0;
            double best_t = INFINITY;
            for (counter=0; objects[counter].type!=0; counter++) {
                // we need to return intersection test on each object
                double t =0;
                switch (objects[counter].type) {
                    case 0:
                        printf("no object found\n");
                        break;
                        
                    case CAM:
                        break;
                        
                    case SPH:
                        t = sphere_intersection(Ro, Rd, objects[counter].data.sphere.position, objects[counter].data.sphere.radius);
                        break;
                        
                    case PLAN:
                        t= plane_intersection(Ro, Rd, objects[counter].data.plane.position, objects[counter].data.plane.normal);
                        break;
                        
                    case QUAD:
                        t = quadric_intersection(Ro, Rd, objects[counter].data.quadric.coefficient);
                        break;
                    default:
                        exit(1);
                }
                if (t > 0 && t < best_t) {
                    best_t = t;
                    best_counter = counter;
                }
                
            }
            if (best_t > 0 && best_t != INFINITY) {
                // there was an intersection
                if (objects[best_counter].type == PLAN) {
                    shade_pixel(objects[best_counter].data.plane.color, x, y, image);
                }
                else if (objects[best_counter].type == SPH){
                    shade_pixel(objects[best_counter].data.sphere.color, x, y, image);
                }
                else if (objects[best_counter].type == QUAD){
                    shade_pixel(objects[best_counter].data.quadric.color,x,y,image);
                }
                
            }
            else{
                Vector white ={1,1,1};
                shade_pixel(white,x,y,image);
            }
            
        }
    }
    
}
























