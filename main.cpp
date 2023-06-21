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
    
    int w=1200, h=1200;
    
    TGAImage image = TGAImage(w, h, white);
    Model m("obj/head.obj");
    
    //for (int i = 0;i < m.nverts(); i++){
    //    Vec3f v = m.vert(i);
    //    image.set(v.x*250+250, v.y*250+250, white);
    //}
    Vec3f light_dir(0,0,-1);
    for (int i = 0; i < m.nfaces(); i++){
       std::vector<int> face = m.face(i);
       Vec2i screen_coords[3];
       Vec3f world_coords[3];
       for (int j = 0; j < 3; j++){
           Vec3f v = m.vert(face[j]);
           screen_coords[j] = Vec2i((v.x+1.)*w/2., (v.y+1.)*h/2);
           world_coords[j]=v;
        }       
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        
        float intensity = n.normalize()*light_dir;
        if(intensity >0)
        {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                image,TGAColor(intensity*255, intensity*255, intensity*255,255));
        }
        
    }
    image.flip_vertical();
    //image.scale(800, 800);
    image.write_tga_file("data/output.tga");
    /*
    TGAImage triang3 = TGAImage(800, 800, white);
    grind(10,triang3, gray);
    triangle(100,50,120,60,110,80, triang3, red);
    triang3.write_tga_file("data\\triangle3.tga");
    */
    return 0;
}



