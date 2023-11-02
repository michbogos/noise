#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

#define HEIGHT 512
#define WIDTH 512
#define NUM_POINTS 200

int points[NUM_POINTS][2];

float min_dist(float x, float y){
    float mn = 1000000000000.0f;
    for(int i = 0; i < NUM_POINTS; i++){
        float h = hypot(points[i][0]-x, points[i][1]-y);
        if(h<mn){
            mn = h;
        }
    }
    return mn;
}

int main(){
    srand(time(NULL));
    for(int i = 0; i < NUM_POINTS; i++){
        points[i][0] = ((float)rand()/RAND_MAX)*WIDTH;
        points[i][1] = ((float)rand()/RAND_MAX)*HEIGHT;
    }
    char* data = malloc(sizeof(char)*4*WIDTH*HEIGHT);
    for(int y = 0; y < HEIGHT; y++){
        printf("%d\n", y);
        for(int x = 0; x < WIDTH; x++){
            float res = min_dist((float)x/WIDTH, (float)y/HEIGHT);
            data[(WIDTH*y+x)*4+0] = res*255;
            data[(WIDTH*y+x)*4+1] = res*255;
            data[(WIDTH*y+x)*4+2] = res*255;
            data[(WIDTH*y+x)*4+3] = 255;
        }
    stbi_write_png("voronoi.png", WIDTH, HEIGHT, 4, data, WIDTH*4);
    }
    return 0;
}