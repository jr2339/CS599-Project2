//
//  vector.h
//  Raycast
//
//  Created by jr2339 on 9/20/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <stdio.h>

void vector_init(float x, float y, float z, float *out);
float v_magnitude(float *point);
void v_unit(float *in, float *out);
void v_add(float *f1, float *f2, float *out);
void v_sub(float *f1, float *f2, float *out);
void v_scale(float *v, float s, float *out);
float v_dot(float *v1, float *v2);
void v_cross(float *v1, float *v2, float *out);

#endif /* vector_h */
