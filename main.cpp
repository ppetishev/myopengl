#include "tgaimage.h"
#include <iostream>

int main(){
    TGAImage i = TGAImage();
    i.read_tga_file("data/flag_b24.tga");
    //i.flip_vertical();
    int w, h;
    w = i.get_width();
    h = i.get_height();
    w = 800;
    h = 800;

    i.scale(w,h);
    i.write_tga_file("data/test_out.tga", true);
    TGAImage ii = TGAImage();
    ii.read_tga_file("data/test_out.tga");
    ii.write_tga_file("data/test_out2_rle.tga", true);
    ii.write_tga_file("data/test_out2_non_rle.tga",false);
    unsigned char c = 200;
    std::cout << (int)(char)c << std::endl;
    std::cout << std::endl;
    return 0;
}
