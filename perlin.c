#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

#define HEIGHT 1080
#define WIDTH 1920

#define GRIDX 32
#define GRIDY 18

float smoothstep(float x0, float x1, float t){
    float res = t*t*t*(t*(6.0f*t-15.0f)+10.0f);
    return x0 + (x1-x0)*res;
}

int main(){
    srand(time(NULL));
    float* vecs = malloc(sizeof(float)*2*GRIDX*GRIDY);
    float* dot = malloc(sizeof(float)*4*WIDTH*HEIGHT);
    for(int i =0; i < GRIDX*GRIDY; i++){
        unsigned w = 8 * sizeof(unsigned);
        unsigned s = w / 2; // rotation width
        unsigned a = i, b = i/WIDTH;
        a *= 3284157443; b ^= a << s | a >> w-s;
        b *= 1911520717; a ^= b << s | b >> w-s;
        a *= 2048419325;
        float random = rand(); // in [0, 2*Pi]
        float x = cos(random);
        float y = sin(random);
        vecs[2*i+0] = x;
        vecs[2*i+1] = y;
    }
    char* data = malloc(sizeof(char)*4*WIDTH*HEIGHT);
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            float xnorm = (float)x/WIDTH;
            float ynorm = (float)y/HEIGHT;
            int vecx = xnorm*GRIDX;
            int vecy = ynorm*GRIDY;
            // dot[(WIDTH*y+x)*4+0] = vecs[(GRIDX*vecy+vecx)*2+0]*((float)(xnorm*GRIDX-vecx)) + vecs[(GRIDX*vecy+vecx)*2+1]*((float)(ynorm*GRIDY-vecy));
            // dot[(WIDTH*y+x)*4+1] = vecs[(GRIDX*(vecy+1)+vecx)*2+0]*((float)(xnorm*GRIDX-vecx)) + vecs[(GRIDX*(vecy+1)+vecx)*2+1]*((float)(ynorm*GRIDY-(vecy+1)));
            // dot[(WIDTH*y+x)*4+2] = vecs[(GRIDX*(vecy+1)+(vecx+1))*2+0]*((float)(xnorm*GRIDX-(vecx+1))) + vecs[(GRIDX*(vecy+1)+(vecx+1))*2+1]*((float)(ynorm*GRIDY-(vecy+1)));
            // dot[(WIDTH*y+x)*4+3] = vecs[(GRIDX*vecy+(vecx+1))*2+0]*((float)(xnorm*GRIDX-(vecx+1))) + vecs[(GRIDX*vecy+(vecx+1))*2+1]*((float)(ynorm*GRIDY-vecy));
            float x1 = vecs[(GRIDX*vecy+vecx)*2+0]*((float)(xnorm*GRIDX-vecx)) + vecs[(GRIDX*vecy+vecx)*2+1]*((float)(ynorm*GRIDY-vecy));
            float x2 = vecs[(GRIDX*(vecy+1)+vecx)*2+0]*((float)(xnorm*GRIDX-vecx)) + vecs[(GRIDX*(vecy+1)+vecx)*2+1]*((float)(ynorm*GRIDY-(vecy+1)));
            float x3 = vecs[(GRIDX*(vecy+1)+(vecx+1))*2+0]*((float)(xnorm*GRIDX-(vecx+1))) + vecs[(GRIDX*(vecy+1)+(vecx+1))*2+1]*((float)(ynorm*GRIDY-(vecy+1)));
            float x4 = vecs[(GRIDX*vecy+(vecx+1))*2+0]*((float)(xnorm*GRIDX-(vecx+1))) + vecs[(GRIDX*vecy+(vecx+1))*2+1]*((float)(ynorm*GRIDY-vecy));
            // x1 = (x1+1)/2.0;
            // x2 = (x1+1)/2.0;
            // x3 = (x1+1)/2.0;
            // x4 = (x1+1)/2.0;
            float ix1 = smoothstep(fabsf(x1), fabsf(x2), (float)(ynorm*GRIDY-vecy));
            float ix2 = smoothstep(fabsf(x4), fabsf(x3), (float)(ynorm*GRIDY-vecy));
            float res = smoothstep(fabsf(ix1),fabsf(ix2), (float)(xnorm*GRIDX-vecx));
            data[(WIDTH*y+x)*4+0] = res*255;
            data[(WIDTH*y+x)*4+1] = res*255;
            data[(WIDTH*y+x)*4+2] = res*255;
            data[(WIDTH*y+x)*4+3] = 255;
            // data[(WIDTH*y+x)*4+0] = (xnorm)*255;
            // data[(WIDTH*y+x)*4+1] = (ynorm)*255;
            // data[(WIDTH*y+x)*4+2] = 0;
            // data[(WIDTH*y+x)*4+3] = 255;
        }
    }
    stbi_write_png("test.png", WIDTH, HEIGHT, 4, data, 4*WIDTH);
    free(data);
    free(vecs);
    return 0;
}