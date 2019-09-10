#include "tgaimage.h"
#include "model.h"
#include <iostream>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor gray   = TGAColor(127, 127, 127,   255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
void line(int x0, int y0, int x1, int y1, TGAImage &img, const TGAColor &c);

int main(int argc, char** argv) {
    int w=800, h=800;
    TGAImage image = TGAImage(w, h, white);
    Model m("obj/head.obj");
    /*/
    for (int i = 0;i < m.nverts(); i++){
        Vec3f v = m.vert(i);
        image.set(v.x*250+250, v.y*250+250, white);
    }
    */
    for (int i = 0; i < m.nfaces(); i++){
       std::vector<int> face = m.face(i);
       for (int j = 0; j < 3; j++){
           Vec3f v0 = m.vert(face[j]);
           Vec3f v1 = m.vert(face[(j+1)%3]);
           int x0 = (v0.x + 1.)*w/2.;
           int x1 = (v1.x + 1.)*w/2.;
           int y0 = (v0.y + 1.)*h/2.;
           int y1 = (v1.y + 1.)*h/2.;
           line(x0, y0, x1, y1, image, black);
       }
    }
    image.flip_vertical();
    //image.scale(800, 800);
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
