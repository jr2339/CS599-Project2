//
//  main.c
//  Raycast
//
//  Created by jr2339 on 9/20/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
/*=============================Dr.Palmer gives a template main function=================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "vector.h"
#include "ppm.h"
/******************************************************************************************/
/* Prints the correct invocation of this program.
 */
void print_usage() {
    printf("USAGE: raycast l|v width outputfile.ppm\n");
    printf("Where l means parallel, v means perspective, and width is an integer.\n");
}

int main(int argc, const char * argv[]) {
    if (argc < 5) {
        //reauire 4 arguments, width of image, height of image, input json file and output ppm file
    }
    return 0;
}
