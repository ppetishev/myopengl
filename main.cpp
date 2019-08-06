#include "tgaimage.h"
#include <iostream>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
void line(int x0, int y0, int x1, int y1, TGAImage &img, const TGAColor &c);

int main(int argc, char** argv) {
    TGAImage image = TGAImage(500, 500, white);
    //image.set(52, 41, red);
    
    line(10, 10, 50, 450, image, red);
    line(10, 460, 500, 20, image, red);
    line(10, 120, 520, 120, image, red);

    image.write_tga_file("data/output.tga");
    return 0;
}
void line(int x0, int y0, int x1, int y1, TGAImage &img, const TGAColor &c){
    bool steep = false;
    if (std::abs(x1-x0)<std::abs(y1-y0)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++){
        if (steep){
            img.set(y, x, c);
        }
        else{
            img.set(x, y, c);
        }
        error2 += derror2;
        if(error2 > dx){
            y += (y0 > y1?-1:1);
            error2 -= dx * 2;
        }
    }
}
