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


static inline double sqr(double v);
static inline double Vector_len(Vector a);
static inline void Vector_add(Vector a, Vector b, Vector c);
static inline void Vector_sub(Vector a, Vector b, Vector c);
static inline void Vector_scale(Vector a, double s, Vector b);
static inline double Vector_dot(Vector a, Vector b);
static inline void Vector_corss(Vector a, Vector b, Vector c);
static inline void normalize(double *v);
#endif /* vector_h */
