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


    
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &img, const TGAColor &c){
    //triangle(t0.x, t0.y, t1.x, t1.y, t2.x, t2.y, img, c);
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    // sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!)
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec2i A =               t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            img.set(j, t0.y+i, c); // attention, due to int casts t0.y+i != A.y
        }
    }
}
void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &img, const TGAColor &c){
    grind(50, img, light_gray);
    for (int y = -1; y<=1; y++){
        for (int x = -1; x<=1;x++){
            img.set(x0+x,y0+y, blue);
            img.set(x1+x,y1+y, blue);
            img.set(x2+x,y2+y, blue);
        }
    }

    if (y0 > y1)
    {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }
    if (y0 > y2)
    {
        std::swap(y0, y2);
        std::swap(x0, x2);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }

    bool long_steep = true, short_steep = true;
    if (std::abs(x0-x2) > std::abs(y0 - y2)){
        long_steep = false;
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    int lcx = x0, scx = x0, cy = y0, y_limit = y1;
    int lx0 = x0, ly0 = y0, lx1 = x2, ly1 = y2;
    int sx0 = x0, sy0 = y0, sx1 = x1, sy1 = y1;
    int lerr, ldy, ldx, lx, ly;
    int serr, sdy, sdx, sx, sy;
    int ldir, sdir;
    if(lx0 < lx1)
        ldir = 1;
    else ldir = -1;
    if (lx0 == lx1)
        ldir = 0;

  
    
    if (std::abs(lx0-lx1) > std::abs(ly0 - ly1)){
        long_steep = false;
        std::swap(lx0, ly0);
        std::swap(lx1, ly1);
    }
    lerr = 0;
    ldy = ly1 - ly0;
    ldx = std::abs(lx1 - lx0);

    for (int i = 0; i < 2; i++){
        if (i == 1){
            sy1 = y2;
            sx1= x2;
            sy0 = y1;
            sy1 = y2;
        }
        if(std::abs(sx0-sx1) > std::abs(sy0-sy1)){
            short_steep = false;
            std::swap(sx0, sy0);
            std::swap(sx1, sy1);
        }
        serr = 0;
        
        
        sdy = sy1 - sy0;
        sdx = std::abs(sx1 - sx0);
        if (sx0 < sx1)
            sdir = 1;
        else 
            sdir = -1;
        if (sx0 == sx1)
            sdir = 0;            
            
        for (;cy <=y_limit; cy++)
        {
            if (long_steep){
                lx = lcx;
                ly = cy;
                sx = cy;
                sy = scx;
            }
            else{
                lx = cy;
                ly = lcx;
            }
            if (short_steep){
                sx = scx;
                sy = cy;                
            }
            else{
                sx = cy;
                sy = scx;
            }
            line(lx, ly, sx, sy, img, c);
            img.set(lx,ly,c);
            img.set(sx,sy,c);
            lerr+=ldx;
            if(lerr * 2 > ldy){
                lerr -= ldy;
                lcx +=ldir;
            }            
            
            serr += sdx;
            if(serr * 2 > sdy){
                serr -= sdy;
                scx +=sdir;
            }
            
        }
        sx0 = x1, sy0 = y1, sx1 = x2, sy1 = y2;
        y_limit = y2;
    }
    //triangle(Vec2i(x0,y0), Vec2i(x1,y1), Vec2i(x2,y2), img, c);
}

#endif //__SHAPES_H__