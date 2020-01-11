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

void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &img, const TGAColor &c){
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

    bool long_steep = true, short_steep = true;
    if (std::abs(lx0-lx1) > std::abs(ly0 - ly1)){
        long_steep = false;
        std::swap(lx0, ly0);
        std::swap(lx1, ly1);
    }
    lerr = 0;
    ldy = ly1 - ly0;
    ldx = std::abs(lx1 - lx0);

    for (int i = 0; i < 2; i++){
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
            //line(lx, ly, sx, sy, img, c);
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
}
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &img, const TGAColor &c)
{
    
}
