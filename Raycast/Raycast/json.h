//
//  json.h
//  Raycast
//
//  Created by jr2339 on 9/20/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
/**********************************************/
#include "ppm.h"
#ifndef json_h
#define json_h
//===========================================//
/*
#define CAMERA_TYPE 1
#define SPHERE_TYPE 2
#define PLANE_TYPE 3
 */
//============================================
 /********************************************
    try to separate

 *********************************************/
void skip_ws(FILE* json);
char* parse_string(FILE* json);
//===========================================//

typedef struct camera{
    char *type;
    float width;
    float height;
}camera;

typedef struct sphere{
    char *type;
    pixel color;
    float *center;
    float radius;
}sphere;

typedef struct plane{
    char *type;
    pixel color;
    float *position;
    float *normal;
}plane;


/*===================================================
//All camera, sphere and plane are josn
tydef struct josn{
    int type;
    union{
        struct camera{
            float width;
            float height;
        }camera;
        struct sphere{
            float *center;
            float radius;
        }sphere;
        struct plane{
            float *position;
            float *normal;
        }plane
    }definition;
    
    pixel color;
    
}josh;
 */
#endif /* json_h */
