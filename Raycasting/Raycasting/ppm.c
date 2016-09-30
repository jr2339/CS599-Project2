//
//  ppm.c
//  Project1
//
//  Created by jr2339 on 9/8/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ppm.h"


Image *BufferCreate(int width, int height,int magic_number){
    Image *buffer = (Image *)malloc(sizeof(Image));
    if(!buffer){
        fprintf(stderr, "can't allocate memory for new image");
        exit(1);
    }
    else{
        buffer->width = width;
        buffer->height = height;
        buffer->magic_number = magic_number;
        //allocate buffer
        buffer->data = (u_char *)malloc(width*height*3);// G, R, B three colors
    }
    if(!buffer->data){
        perror("can't allocate memory for new image");
    }
    return buffer;
}
/**************************************************************************************************************
 **************************************************************************************************************/
// readPPMHeader help us to detemine which format(such as P1, P2...) for our source image

void readPPMHeader(FILE *f_source, int *width, int *height,int *magic_address){
    char ch;
    int maxval;
    
    // if the format is not P3 or P6 or P7, it gives us a error
    if(fscanf(f_source, "P%c\n",&ch) !=1 || (ch!='3'&& ch!='6'&& ch!='7')){
        //ch will point to the first word in our file which should be P1, P3 or P7
        fprintf(stderr,"file is not in PPM format, we can't read it");
        exit(1);
    };
    *magic_address = ch - '0';
    printf("magic address %d \n",*magic_address);
    /*skip the comments*/
    ch = getc(f_source); // ch is int
    /*
     gets the next character (an unsigned char) from the specified
     stream and advances the position indicator for the stream.
     */
    while(ch =='#'){
        do{
            ch=getc(f_source);
        }
        while(ch!='\n'); //read to the end of the line
        ch=getc(f_source);
    }
    if(!isdigit(ch)){
        fprintf(stderr,"can't read header information from PPM format");
    }
    else{
        ungetc(ch, f_source); //put that digital back
    }
    //read the width, height,amd maximum value for a pixel
    fscanf(f_source, "%d %d %d\n",width,height,&maxval);
    if(maxval >= 65336 ||maxval <= 0){
        fprintf(stderr,"image is not ture-color(8byte), read failed");
        exit(1);
    }
}

/**************************************************************************************************************
 **************************************************************************************************************/



Image *ImageRead(const char *filename){
    int width,height,size,magic_number;
    size_t num;
    //u_char *p;
    Image *buffer = (Image *)malloc(sizeof(Image));
    FILE *fp = fopen(filename,"r");
    if(!buffer){
        fprintf(stderr,"Can't allocate memory for new image");
        exit(1);
    }
    if(!fp){
        fprintf(stderr,"can't open file for reading");
        exit(1);
        
    }
    readPPMHeader(fp, &width, &height,&magic_number);
    size = width * height *3;
    buffer = BufferCreate(width, height, magic_number);
    if(!buffer->data){
        fprintf(stderr,"can't allocate memory");
        exit(1);
    }else{
        if(magic_number>=4 &&magic_number<=7){
            num = fread((void *) buffer->data, 1, (size_t) size, fp);
            if (num != size) {
                fprintf(stderr,"cannot read image datat from file \n");
                exit(1);
            }

        }
        else if(magic_number>=0 &&magic_number<=3){
            int i,j, *pixel;
            for(i=0;i<height;i++){
                for(j=0;j<width;j++){
                    fscanf(fp, "%d ",pixel);
                    buffer->data[i*width*3+3*j] = *pixel;
                    fscanf(fp, "%d ",pixel);
                    buffer->data[i*width*3+3*j+1] = *pixel;
                    fscanf(fp, "%d ",pixel);
                    buffer->data[i*width*3+3*j+2] = *pixel;

                }
                
                //fprintf(buffer, "\n");
            }

        }
    }
        return buffer;
}

/**************************************************************************************************************
 **************************************************************************************************************/

void ImageWrite(Image *buffer, const char *filename,int format){
    printf("the magic number is P%d \n", buffer->magic_number);
    size_t num;
    int size = buffer->width * buffer->height * 3;
    //printf("the size is %d \n", size);
    FILE *f_des = fopen(filename, "w"); //filename looks like /home/jun
    if (!f_des){
        fprintf(stderr,"cannot open file for writing");
    }else{
        fprintf(f_des, "P%d\n%d %d\n%d\n",format,buffer->width, buffer->height, 255);
        //p4-p7
        if(format >=4 &&format<=7){
            num = fwrite((void *) buffer->data, 1, (size_t) size, f_des);
        }
        else if(format>=0 && format<=3){
            int i,j;
            for(i=0;i<buffer->height;i++){
                for(j=0;j<buffer->width;j++){
                     fprintf(f_des, "%d ",buffer->data[i*buffer->width*3+3*j]);
                     fprintf(f_des, "%d ",buffer->data[i*buffer->width*3+3*j+1]);
                     fprintf(f_des, "%d ",buffer->data[i*buffer->width*3+3*j+2]);
                }
                
                 fprintf(f_des, "\n");
            }
        }
    fclose(f_des);
    }
}
