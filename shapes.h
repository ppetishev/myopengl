#ifndef __SHAPES_H__
#define __SHAPES_H__
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor gray = TGAColor(128, 128, 128, 255);
const TGAColor light_gray = TGAColor(220, 220, 220, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255,   255);

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
void line(Vec2i t0, Vec2i t1, TGAImage &img, TGAColor &c){
    line(t0.x, t0.y, t1.x, t1.y, img, c);
}

void grind(int cell_size, TGAImage &img, const TGAColor &c){
    for (int i =0; i<img.get_height(); i+=cell_size){
        line(0, i, img.get_width()-1, i, img, c);
    }
    for (int i =0; i<img.get_width(); i+=cell_size){
        line(i, 0, i, img.get_height()-1, img, c);
    }
}


    
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &img, const TGAColor &c){ // triangle in 2-dimensional space
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    int first_segment_height = t1.y - t0.y;
    int second_segment_height = t2.y - t1.y;
    Vec2f delta_alpha = Vec2f(t2.x-t0.x, t2.y-t0.y)*((float)1/total_height);
    Vec2f delta_first_beta = Vec2f(t1.x-t0.x,t1.y-t0.y)*((float)1/first_segment_height);
    Vec2f delta_second_beta = Vec2f(t2.x-t1.x, t2.y-t1.y)*((float)1/second_segment_height);
    Vec2i A = t0;
    Vec2i B;
    Vec2f A_acc = Vec2f(t0.x, t0.y);
    Vec2f B_first_acc = Vec2f(t0.x, t0.y);
    Vec2f B_second_acc = Vec2f(t1.x, t1.y);
    for (int i=0; i<total_height; i++) {
        bool second_half = i>=first_segment_height || t1.y==t0.y;
        A_acc = A_acc + delta_alpha;
        A = Vec2i((int)A_acc.x, (int)A_acc.y);
        if(second_half)
        {
            B_second_acc = B_second_acc + delta_second_beta;
            B = Vec2i((int)B_second_acc.x, (int)B_second_acc.y);
        }
        else

        {
            B_first_acc = B_first_acc + delta_first_beta;
            B = Vec2i((int)B_first_acc.x, (int)B_first_acc.y);
        }
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            img.set(j, t0.y+i, c); // attention, due to int casts t0.y+i != A.y
        }
    }
}
void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &img, const TGAColor &c){
    triangle(Vec2i(x0,y0), Vec2i(x1,y1), Vec2i(x2,y2), img, c);
}

void triangle(Vec3i t0, Vec3i t1, Vec3i t2, TGAImage &img, const TGAColor &c, int *zbuffer) // triangle in 3-dimensional space
{
    if (t0.y == t1.y && t0.y == t2.y) return;
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y == t0.y;
        int segment_height = (second_half ? t2.y-t1.y : t1.y-t0.y);
        float alpha = (float)i/total_height;
        float beta = (float)(i-(second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec3i A = t0 + Vec3f(t2-t0) * alpha;
        Vec3i B = second_half ? t1 + (Vec3f(t2-t1) * beta) : t0 + (Vec3f(t1-t0) * beta);
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j<=B.x; j++)
        {
            float phi = A.x == B.x ? 1. : (float)(j - A.x)/(float)(B.x - A.x);
            Vec3i P = (Vec3f)A + Vec3f(B-A)*phi;
            int idx = P.x + P.y * img.get_width();
            //std::cout << idx<< std::endl;
            if(zbuffer[idx] < P.z)
            {
                zbuffer[idx] = P.z;
                img.set(P.x, P.y, c);
            }
        }
    }
}
#endif //__SHAPES_H__