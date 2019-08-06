#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>

#pragma pack(push, 1)
struct TGAHeader{
	char id_length;	
	char color_map_type;
	char image_type;
	short color_map_origin;
	short color_map_length;
	char color_map_depth;
	short x_origin;
	short y_origin;
	unsigned short width;
	unsigned short height;
	unsigned char bits_per_pixel;
	char image_descriptor;
};
#pragma pack(pop)


struct TGAColor{
	union {
		struct {unsigned char b, g, r, a;};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	TGAColor():val(0), bytespp(1){}
	TGAColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a):b(_b), g(_g), r(_r), a(_a), bytespp(4){}
	TGAColor(int v, int bpp): val(v), bytespp(bpp){}
	TGAColor(unsigned char *b, int bpp):val(0), bytespp(bpp){
		for (int i = 0;i<bpp; ++i){
			raw[i] = b[i];
		}
	}
	//TGAColor(TGAColor &c):val(c.val), bytespp(c.bytespp){}
	TGAColor operator=(TGAColor &c){
		if(this != &c){
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};

class TGAImage{
private:
	unsigned char* id;
	int width;
	int height;
	int bytespp;
	unsigned char* data;
	
	bool load_rle_data(std::ifstream &in);
	bool unload_rle_data(std::ofstream &out);
public:
	enum Format{
		GRAYSCALE = 1,
		RGB = 3,
		RGBA = 4
	};
	
	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(int w, int h, int bpp, unsigned int c);
	TGAImage(int w, int h, const TGAColor &c);
	TGAImage(const TGAImage &img);	
	bool read_tga_file(const char* filename);
	bool write_tga_file(const char* filename, bool rle = true);
	bool flip_horizont();
	bool flip_vertical();
	bool set(int x, int y, TGAColor &c);
	bool set(int x, int y, const TGAColor &c);
	TGAColor get(int x, int y);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char *buffer();
	~TGAImage();
	bool scale(int w, int h);
	void clear();
	TGAImage & operator =(const TGAImage &img);

};
#endif // __IMAGE_H__
