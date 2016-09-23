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
/**********Use boolean or #include <stdbool.h>*******/
typedef enum {
    false, true
} bool;
/**********This is the Possible Type for Json File*******/
typedef enum {
    J_STRING,
    J_NUMBER,
    J_OBJECT,  // such as sphere, camera, line, plane
    J_ARRAY,
    J_BOOLEAN,
    J_NULL,
}TYPE;
/**********After we have type, Now we define the Vaule for Json*******/
typedef struct VALUE{
    TYPE type;
    union{
        char *String;
        double Number;
        bool   Boolean;
        char Null;
        struct VALUE *value;
    }data;
    int count;
    char **keys;
}VALUE;


#endif /* json_h */
