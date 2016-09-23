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
typedef struct Object{
    char *type;
    float width;
    float height;
    float radius;
    float position[3];
    float color[3];
    float normal[3];
}Object;

#endif /* json_h */
