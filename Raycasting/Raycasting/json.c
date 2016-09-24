//
//  json.c
//  Raycasting
//
//  Created by jr2339 on 9/22/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<errno.h>

#include "json.h"
/*******************************************This is Dr.Palmer's code*******************************/
/*******************************************Declaration*******************************/
void Check_Error(char ch, FILE* json, int line);
void Check_symbol(char ch, char symbol, int line);
char Get_Char(FILE* json, int* line);
void skip_ws(FILE* json, int* line);
char* nextString(FILE* json, int* line);
double nextNumber(FILE* json, int* line);
double* nextVector3d(FILE* json, int* line);
/*******************************************Declaration*******************************/
Object* readScene(const char* json_file_path){
        Object* object = NULL; // init
        int object_size = 0;
        int line =1; // line at here is a indicator, could tell us our input jason file error
        double *vector;
        char ch;
        FILE* json = fopen(json_file_path, "r");
        if(json == NULL){
            fprintf(stderr, "Error: Can't Opening input jason file\n");
            exit(1);
        }
        /* We will check our input file is a json file or not*/
        /*the first step is skip the white space*/
        skip_ws(json,&line);
        /*After we skip the white space, we get the first symbol or char */
        ch = Get_Char(json, &line);
        /*Then Check this symbol is '[' or not, more details see structure for json file*/
        Check_symbol(ch, '[', line);
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ DO While Lopp @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    do{
            /*If it is we will skip the white space, and get char and compare with '{'*/
            skip_ws(json, &line);
            ch = Get_Char(json, &line);
            Check_symbol(ch, '{', line);
            /*Skip the white space, and get the first charcater*/
            skip_ws(json, &line);
            ch = Get_Char(json, &line);
            if(ch=='}'){
            fprintf(stderr, "Waring: Line %d: Empty object\n",line);
                skip_ws(json,&line);
            }
            else if(ungetc(ch,json)==EOF){
                fprintf(stderr, "Error: Line %d: Read erroe\n",line);
                perror("");
                exit(1);
            }
            if((object = realloc(object,++object_size))==NULL){
                fprintf(stderr, "Error：Line %d: mEMORY reallocation error\n",line);
                perror("");
                exit(1);
            }
            /*attribute is object attribute, such as type, width, height*/
            char* attribute = nextString(json, &line);
            /*if it is not a type, give me an error*/
            if(strcmp(attribute, "type")!=0){
                fprintf(stderr, "Error: First key must be 'type'\n");
                exit(1);
            }
        
            /*if it is a type, we skip the white space
            *and then get the symbol
            *compare with :
            */
            skip_ws(json, &line);
            ch = Get_Char(json, &line);
            Check_symbol(ch, ':', line);
        
            /*We skip the white space
             ************************
             */
            skip_ws(json, &line);
            object[object_size-1].type = nextString(json, &line);
            /***********************
            ***********************
            */
            skip_ws(json, &line);
            /*====================================================start while loop=================================================*/
            while((ch = Get_Char(json, &line)==',')){
                skip_ws(json, &line);
                //Get attribute
                attribute = nextString(json, &line);
                //Get ':'
                skip_ws(json, &line);
                ch = Get_Char(json, &line);
                Check_symbol(ch, ':', line);
            
                skip_ws(json, &line);
                /*************************************************if it is camera*************************************/
                if (strcmp(object[object_size-1].type, "camera")==0) {
                    if (strcmp(attribute,"width")==0) {
                        object[object_size-1].width = nextNumber(json, &line);
                        if(object[object_size-1].width<0){
                            fprintf(stderr, "Error: Line %d: Width cannot be negative\n",line);
                            exit(-1);
                        
                        }
                    }
                    else if (strcmp(attribute,"height")==0){
                        object[object_size - 1].height = nextNumber(json, &line);
                        fprintf(stderr, "Error: Line %d: Height cannot be negative\n",line);
                        exit(1);
                    }
                    else{ // give us an error, this is not an camera
                        fprintf(stderr, "Error: Line %d: Key '%s' not supported under 'camera'\n", line, attribute);
                        exit(1);
                    
                    }
                }
                /*************************************************if it is sphere*************************************/
                else if(strcmp(object[object_size-1].type, "sphere")==0){
                    if(strcmp(attribute, "color")) {
                        vector = nextVector3d(json, &line);
                        for(int i = 0; i < 3; i++) {
                            if(vector[i] < 0 || vector[i] > 1) {
                                fprintf(stderr, "Error: Line %d: Color must be between 0.0 and 1.0\n", line);
                                exit(1);
                            }
                            object[object_size - 1].color[i] = vector[i];
                        }
                        free(vector);
                    }
                    else if(strcmp(attribute, "position")){
                        vector = nextVector3d(json, &line);
                        for(int i = 0; i < 3; i++) {
                            object[object_size - 1].position[i] = vector[i];
                        }
                        free(vector);
                    }
                    else if(strcmp(attribute, "radius")){
                        object[object_size - 1].radius = nextNumber(json, &line);
                        if(object[object_size - 1].radius < 0) {
                            fprintf(stderr, "Error: Line %d: Radius cannot be negative\n", line);
                            exit(1);
                        }
                        else {
                            fprintf(stderr, "Error: Line %d: Key '%s' not supported under 'sphere'\n", line, attribute);
                            exit(1);
                        }
                    }
                }
                /*************************************************if it is plane*************************************/
                else if(strcmp(object[object_size - 1].type, "plane") == 0){
                    if(strcmp(attribute, "color")) {
                        vector = nextVector3d(json, &line);
                        for(int i = 0; i < 3; i++) {
                            if(vector[i] < 0 || vector[i] > 1) {
                                fprintf(stderr, "Error: Line %d: Color must be between 0.0 and 1.0\n", line);
                                exit(1);
                            }
                            object[object_size - 1].color[i] = vector[i];
                        }
                        free(vector);
                    }
                    else if(strcmp(attribute, "position")) {
                        vector = nextVector3d(json, &line);
                        for(int i = 0; i < 3; i++) {
                            object[object_size - 1].position[i] = vector[i];
                        }
                        free(vector);
                    }
                    else if(strcmp(attribute, "normal")) {
                        vector = nextVector3d(json, &line);
                        for(int i = 0; i < 3; i++) {
                            object[object_size - 1].normal[i] = vector[i];
                        }
                        free(vector);
                    }
                    else {
                        fprintf(stderr, "Error: Line %d: Key '%s' not supported under 'plane'\n", line, attribute);
                        exit(1);
                    }
                }
            
                /*************************************************unknown object*************************************/
                else {
                    fprintf(stderr, "Error: Line %d: Unknown type %s", line, object[object_size- 1].type);
                    exit(1);
                }
                skip_ws(json, &line);
            }
            /*====================================================end while loop=================================================*/
        if(ungetc(ch, json)==EOF){
            fprintf(stderr, "Error: Line %d: Read error\n", line);
            exit(1);
        }
        skip_ws(json, &line);
        ch = Get_Char(json, &line);
        Check_symbol(ch, '}', line);
        skip_ws(json, &line);
    }while((ch = Get_Char(json, &line)) == ',');
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ DO While Lopp END @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    Check_symbol(ch, ']', line);
    // Manually get trailing whitespace
    while((ch = fgetc(json)) != EOF && isspace(ch)) {
        if(ch == '\n') {
            line += 1;
        }
    }
    
    if (ch!=EOF) {
        fprintf(stderr, "Error: Line %d: Unkown symbol at end-of-file\n", line);
        exit(1);
    }
    else if (!feof(json) && ferror(json)){
        fprintf(stderr, "Error: Line %d: Read error\n", line);
        perror("");
        exit(1);
    }
    
    if(object == NULL) {
        fprintf(stderr, "Warning: Line %d: Empty array\n", line);
    }
    return object;
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void Check_Error(char ch, FILE* json, int line){
    if (ch == EOF) {
        if(feof(json)) {
            fprintf(stderr, "Error: Line %d: Premature end-of-file\n", line);
            exit(1);
        }
        else if(ferror(json)) {
            fprintf(stderr, "Error: Line %d: Read error\n", line);
            perror("");
            exit(1);
        }
    }
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void Check_symbol(char ch, char symbol, int line){
    if(ch != symbol) {
        fprintf(stderr, "Error: Line %d: Expected '%c'\n", line, symbol);
        exit(1);
    }
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
char Get_Char(FILE* json, int* line){
    char ch = fgetc(json);
    Check_Error(ch, json, *line);
    if(ch == '\n') {
        *line += 1;
    }
    return ch;
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void skip_ws(FILE* json, int* line){
    char ch;
    ch = Get_Char(json, line);
    while(isspace(ch)) {
        ch = Get_Char(json, line);
    }
    if(ungetc(ch, json) == EOF) {
        fprintf(stderr, "Error: Line %d: Read error\n", *line);
        perror("");
        exit(1);
    }
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
char* nextString(FILE* json, int* line){
    int buffer_size = 64;
    int old_size = buffer_size;
    char ch;
    int i=0;
    char* buffer = malloc(sizeof(char)*buffer_size);
    if(buffer == NULL) {
        fprintf(stderr, "Error: Line %d: Memory allocation error\n", *line);
        perror("");
        exit(1);
    }
    
    ch = Get_Char(json, line);
    Check_symbol(ch, '"', *line);
    while(i<buffer_size-1&&(ch = Get_Char(json, line)!='"')){
        if (i==buffer_size-2) {
            buffer_size = buffer_size*2;
            if(old_size != 0 && buffer_size / old_size != 2) {
                fprintf(stderr, "Error: Line %d: Integer overflow on size\n",*line);
                exit(1);
            }
            
            old_size = buffer_size;
            buffer = realloc(buffer, buffer_size);
            if(old_size != 0 && buffer_size / old_size != 2) {
                fprintf(stderr, "Error: Line %d: Integer overflow on size\n",*line);
                perror("");
                exit(1);
            }
        }
        buffer[i++] = ch;
    }
    buffer[i] ='\0';
    
    return buffer;
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
double nextNumber(FILE* json, int* line) {
    double value;
    int status = fscanf(json, "%lf", &value);
    Check_Error(status, json, *line);
    if(status < 1) {
        fprintf(stderr, "Error: Line %d: Invalid number\n", *line);
        exit(1);
    }
    
    if(errno == ERANGE) {
        if(value == 0) {
            fprintf(stderr, "Error: Line %d: Number underflow\n", *line);
            exit(1);
        }
        if(value == HUGE_VAL || value == -HUGE_VAL) {
            fprintf(stderr, "Error: Line %d: Number overflow\n", *line);
            exit(1);
        }
    }
    
    return value;
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

double* nextVector3d(FILE* json, int* line) {
    int SIZE = 3;
    double* vector = malloc(SIZE * sizeof(*vector));
    if(vector == NULL) {
        fprintf(stderr, "Error: Line %d: Memory allocation error\n", *line);
        perror("");
        exit(1);
    }
    char ch = Get_Char(json, line);
    Check_symbol(ch, '[', *line);
    
    for(int i = 0; i < SIZE; i++) {
        skip_ws(json, line);
        vector[i] = nextNumber(json, line);
        
        if(i < SIZE - 1) {
            skip_ws(json, line);
            ch = Get_Char(json, line);
            Check_symbol(ch, ',', *line);
        }
    }
    
    skip_ws(json, line);
    ch = Get_Char(json, line);
   Check_symbol(ch, ']', *line);
    
    return vector;
}



















