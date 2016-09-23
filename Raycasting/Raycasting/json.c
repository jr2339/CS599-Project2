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
void errorCheck(int c, FILE* json, int line);
void Check_symbol(int c, char symbol, int line);
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
            continue;
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
        char* key = nextString(json, &line);
        /*if it is not a type, give me an error*/
        if(strcmp(key, "type")!=0){
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
        
    }
    return object;
}
