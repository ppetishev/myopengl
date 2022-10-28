#include "tgaimage.h"
#include "model.h"
#include <iostream>
#include "shapes.h"
/*const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor gray = TGAColor(128, 128, 128, 255);
const TGAColor light_gray = TGAColor(220, 220, 220, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255,   255);*/
//void line(int x0, int y0, int x1, int y1, TGAImage &img, const TGAColor &c);
//void line(Vec2i t0, Vec2i t1, TGAImage &img, const TGAColor &c);
//void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &img, const TGAColor &c);

int main(int argc, char** argv){
    /*
    int w=800, h=800;
    
    TGAImage image = TGAImage(w, h, white);
    Model m("obj/head.obj");
    
    //for (int i = 0;i < m.nverts(); i++){
    //    Vec3f v = m.vert(i);
    //    image.set(v.x*250+250, v.y*250+250, white);
    //}
    
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
    */
    TGAImage triang = TGAImage(800, 800, white);    
    TGAImage triang2 = TGAImage(800, 800, white);
    TGAImage triang3 = TGAImage(800, 800, white);

    //test line
    /*
    line(50,100, 250,750,triang,light_gray);
    line(50,100, 750,500,triang,light_gray);
    line(250, 750, 750, 500,triang,light_gray);*/
    triangle(50, 100, 250, 750, 750, 500, triang, red);

    /*
    line(100,50,700,600,triang2,light_gray);
    line(700,600,350,500,triang2,light_gray);
    line(100,50, 350,500,triang2,light_gray);*/
    //triangle(100,50,700,600,350,500, triang2, red);//
    triangle(Vec2i(50, 500),Vec2i(750,600),Vec2i(400,300), triang2, red);

    
    //line(100,50,100,700,triang3,light_gray);
    //line(100,700,350,500,triang3,light_gray);
    line(100,50, 350,500,triang3,blue);
    triangle(100,50,100,700,350,500, triang3, red);

    triang.write_tga_file("data\\triangle.tga");
    triang2.write_tga_file("data\\triangle2.tga");
    triang3.write_tga_file("data\\triangle3.tga");
    return 0;
}



