//
//  helpers.h
//  Raycasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#ifndef helpers_h
#define helpers_h

#include <stdio.h>

void skip_ws(FILE* json);
long wxy_to_index(int width, int x, int y);
void index_to_xwy(long index, int width, int* x, int* y);

#endif /* helpers_h */
