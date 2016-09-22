//
//  helps.h
//  RayCasting
//
//  Created by jr2339 on 9/21/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#ifndef helps_h
#define helps_h

#include "image.h"
#include <stdio.h>

int skip_ws(FILE* json);
char* parse_string(FILE* json);
int convert_wxy_index(int width, int x, int y);
void convert_index_xwy(int index, int width, int *x, int *y);

#endif /* helps_h */
