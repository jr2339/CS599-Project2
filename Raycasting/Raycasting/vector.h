//
//  vector.h
//  Raycasting
//
//  Created by jr2339 on 9/30/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "json.h"

typedef double Vector[3]; // array of double values to represent


double sqr(double v);
double Vector_len(Vector a);
void Vector_add(Vector a, Vector b, Vector c);
void Vector_sub(Vector a, Vector b, Vector c);
void Vector_scale(Vector a, double s, Vector b);
double Vector_dot(Vector a, Vector b);
void Vector_corss(Vector a, Vector b, Vector c);
void normalize(double *v);
#endif /* vector_h */
