#include "tgaimage.h"
#include <iostream>
#include <cstring>

TGAImage::TGAImage():width(0), height(0), bytespp(0), data(NULL){}

TGAImage::TGAImage(int w, int h, int bpp):width(w), height(h), bytespp(bpp), data(NULL){
	unsigned long nbytes = w * h * bpp;
	data = new unsigned char[nbytes];
	memset(data,0,nbytes);
}

TGAImage::TGAImage(int w, int h, int bpp, unsigned int c):width(w), height(h), 
		bytespp(bpp), data(NULL){
	unsigned long nbytes = w * h * bpp;
	data = new unsigned char[nbytes];
	unsigned char buf[bpp];
	for (int i = 0; i < bpp; ++i){
		buf[i] = c % 256;
		c /=256;
	}

	for (unsigned long i = 0; i < nbytes; i+=bpp){
		for (int j = 0; j < bpp; ++j){
			data[i+j] = buf[j];
		}
	}
}

TGAImage::TGAImage(int w, int h, const TGAColor &c):width(w), height(h), 
		bytespp(c.bytespp), data(NULL){
	unsigned long nbytes = w * h * c.bytespp;
	data = new unsigned char[nbytes];
	for (unsigned long i = 0; i < nbytes; i+=c.bytespp){
		for (int j = 0; j < c.bytespp; ++j){
			data[i+j] = c.raw[j];
		}
	}
}

TGAImage::TGAImage(const TGAImage &img){
	width = img.width;
	height = img.height;
	bytespp = img.bytespp;
	unsigned long nbytes = width * height * bytespp;
	data = new unsigned char[nbytes];
	memcpy(data, img.data, nbytes);
}

bool TGAImage::read_tga_file(const char* filename){
	if (data) delete [] data;
	data = NULL;
	std::ifstream istr;
	istr.open(filename);
	if(!istr.is_open()){
		std::cerr << "can't open file " << filename << std::endl;
		istr.close();
		return false;
	}

	TGAHeader header;
	istr.read((char *)&header, sizeof(header));
	if(!istr.good()){
		istr.close();
		std::cerr << "can't read header in " << filename << std::endl;		
		return false;
	}
	
	width = header.width;
	height = header.height;
	bytespp = header.bits_per_pixel>>3;
	
	std::cout << width << ", " << height << ", " << bytespp << ", " << (int)header.image_type << std::endl;
	if (width<=0 || height<=0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA)){
		istr.close();
		std::cerr << "bad bpp/width/height in " << filename << std::endl;
		return false;
	}
	if (header.id_length < 0){
		istr.close();
		std::cerr << "bad identificator lenght in " << filename << std::endl;
		return false;
	}
	if (header.id_length > 0){
		istr.read((char *)id, header.id_length);
		if(!istr.good()){
			istr.close();
			std::cerr << "can't read identificator in " << filename << std::endl;		
			return false;
		}
	}

	id = new unsigned char[header.id_length];
	unsigned long nbytes = width * height * bytespp;
	data = new unsigned char[nbytes];

	if(header.image_type == 3 || header.image_type == 2)
	{
	istr.read((char*)data, nbytes);
		if(!istr.good()){
			istr.close();
			std::cerr << "can't read data in " << filename << std::endl;		
			return false;
		}
	}else 
	if(header.image_type == 10 || header.image_type == 11){
		if(!load_rle_data(istr)){
			istr.close();
			std::cerr << "can't read data in " << filename << std::endl;		
			return false;
		}		
	}else {
		istr.close();
			std::cerr << "unknown file format " << (int) header.image_type 
				<< " of " << filename << std::endl;		
			return false;
	}
	if (!(header.image_descriptor & 0x20))
	{
		flip_vertical();
	}
	if (header.image_descriptor & 0x10)
	{
		flip_horizont();
	}
	istr.close();
	return true;
}

bool TGAImage::load_rle_data(std::ifstream &istr){
	unsigned char chunk_header;
	TGAColor pixel;
	const unsigned long pixel_count = width * height;
	unsigned long pixel_counter = 0;
	unsigned long index_counter = 0;
	do{
		chunk_header = istr.get();
		if (!istr.good())
		{
			std::cerr << "error in load rle\n";
			return false;
		}
		if (chunk_header < 128){
			chunk_header++;
			for (int i = 0; i < chunk_header; i++)
			{
				istr.read((char *)pixel.raw, bytespp);
				if (!istr.good()){
					std::cerr << "error in load rle\n";
					return false;
				}
				for (int i = 0; i < bytespp; i++){
					data[index_counter++] = pixel.raw[i];
					//index_counter++;
				}
				pixel_counter++;
				if(pixel_counter > pixel_count){
					std::cerr << "Too many pixels read\n";
				}
			}
		}else{
			chunk_header -= 127;
			istr.read((char *)pixel.raw, bytespp);
			for (int i = 0; i < chunk_header; i++){
				for (int j = 0; j<bytespp; j++){
					data[index_counter++] = pixel.raw[j];
				}
				pixel_counter++;
				if(pixel_counter > pixel_count){
					std::cerr << "Too many pixels read\n";
				}
			}
		}
	} while (pixel_counter < pixel_count);
	return true;
}

bool TGAImage::unload_rle_data(std::ofstream &ostr){
	const unsigned char max_chunk_length = 128;
    unsigned long npixels = width*height;
    unsigned long curpix = 0;
    while (curpix<npixels) {
        unsigned long chunkstart = curpix*bytespp;
        unsigned long curbyte = curpix*bytespp;
        unsigned char run_length = 1;
        bool raw = true;
        while (curpix+run_length<npixels && run_length<max_chunk_length) {
            bool succ_eq = true;
            for (int t=0; succ_eq && t<bytespp; t++) {
                succ_eq = (data[curbyte+t]==data[curbyte+t+bytespp]);
            }
            curbyte += bytespp;
            if (1==run_length) {
                raw = !succ_eq;
            }
            if (raw && succ_eq) {
                run_length--;
                break;
            }
            if (!raw && !succ_eq) {
                break;
            }
            run_length++;
        }
        curpix += run_length;
        ostr.put(raw?run_length-1:run_length+127);
        if (!ostr.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
        ostr.write((char *)(data+chunkstart), (raw?run_length*bytespp:bytespp));
        if (!ostr.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
    }
    return true;
}

bool TGAImage::write_tga_file(const char* filename, bool rle){
	unsigned char developer_area_ref[4] = {0, 0, 0, 0};
    unsigned char extension_area_ref[4] = {0, 0, 0, 0};
    unsigned char footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    std::ofstream out;
	std::ofstream ofstr;
	ofstr.open(filename, std::ios::binary);
	if (!ofstr.is_open()){
		std::cerr << "cannot open/create file to write" << "\n";
		ofstr.close();
		return false;
	}

	TGAHeader header;
	memset((void *)&header,0, sizeof(header));
	header.width = width;
	header.height = height;
	header.bits_per_pixel = bytespp << 3;
	header.image_type = (bytespp == GRAYSCALE ? (rle?11:3):(rle?10:2));
	header.image_descriptor = 0x20;

	ofstr.write((char *)&header, sizeof(header));
	if (!ofstr.good()){
		std::cerr << "can't write header\n";
		ofstr.close();
		return false;
	}
	if (rle){
		unload_rle_data(ofstr);
		if (!ofstr.good()){
			std::cerr << "can't write rle data\n";
			ofstr.close();
			return false;
		}
	}
	else{
		ofstr.write((char*)data, width * height * bytespp);
		if (!ofstr.good()){
			std::cerr << "can't write data\n";
			ofstr.close();
			return false;
		}
	}
	
	ofstr.write((char *)developer_area_ref, sizeof(developer_area_ref));
    if (!ofstr.good()) {
        std::cerr << "can't dump the tga file\n";
        ofstr.close();
        return false;
    }
    ofstr.write((char *)extension_area_ref, sizeof(extension_area_ref));
    if (!ofstr.good()) {
        std::cerr << "can't dump the tga file\n";
        ofstr.close();
        return false;
    }
    ofstr.write((char *)footer, sizeof(footer));
    if (!ofstr.good()) {
        std::cerr << "can't dump the tga file\n";
        ofstr.close();
        return false;
    }
    ofstr.close();	
	return true;
}

bool TGAImage::flip_horizont(){
	if(!data) return false;
	int half = width >> 1;
	for (int i = 0; i < half; i++){
		for(int j = 0;j < height; j++){
			TGAColor c1 = get(i,j);
			TGAColor c2 = get(width-i-1,j);
			set(i,j,c2);
			set(width-i-1,j,c1);
		}
	}
	return true;
}

bool TGAImage::flip_vertical(){
	if(!data) return false;
	int half = height >> 1;
	for (int i = 0; i < width; i++){
		for(int j = 0;j < half; j++){
			TGAColor c1 = get(i,j);
			TGAColor c2 = get(i,height - 1 -j);
			set(i,j,c2);
			set(i,height - 1 -j,c1);
		}
	}
	return true;
}

unsigned char *TGAImage::buffer() {
    return data;
}

bool TGAImage::set(int x, int y, TGAColor &c) {
    if (!data || x<0 || y<0 || x>=width || y>=height) {
        return false;
    }
    memcpy(data+(x+y*width)*bytespp, c.raw, bytespp);
    return true;
}

bool TGAImage::set(int x, int y, const TGAColor &c) {
    if (!data || x<0 || y<0 || x>=width || y>=height) {
        return false;
    }
    memcpy(data+(x+y*width)*bytespp, c.raw, bytespp);
    return true;
}
int TGAImage::get_height(){
	return height;
}
int TGAImage::get_width(){
	return width;
}
int TGAImage::get_bytespp(){
	return bytespp;
}

TGAColor TGAImage::get(int x, int y){
	if(!data || x < 0 || y < 0 || x >= width || y >= height){
		return TGAColor();
	}
	return TGAColor(data + (x + y*width)*bytespp, bytespp);
}
bool TGAImage::scale(int w, int h){
	if (w<=0 || h<=0 || !data) return false;
    unsigned char *tdata = new unsigned char[w*h*bytespp];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w*bytespp;
	unsigned long olinebytes = width*bytespp;
	for (int j=0; j<height; j++) {
		int errx = width-w;
		int nx   = -bytespp;
		int ox   = -bytespp;
		for (int i=0; i<width; i++) {
			ox += bytespp;
			errx += w;
			while (errx>=(int)width) {
				errx -= width;
				nx += bytespp;
				memcpy(tdata+nscanline+nx, data+oscanline+ox, bytespp);
			}
		}
		erry += h;
		oscanline += olinebytes;
		while (erry>=(int)height) {
			if (erry>=(int)height<<1) // it means we jump over a scanline
				memcpy(tdata+nscanline+nlinebytes, tdata+nscanline, nlinebytes);
			erry -= height;
			nscanline += nlinebytes;
		}
	}
    delete [] data;
    data = tdata;
    width = w;
    height = h;
    return true;
}
TGAImage::~TGAImage(){
    if (data) delete [] data;
}
TGAImage & TGAImage::operator =(const TGAImage &img) {
    if (this != &img) {
        if (data) delete [] data;
        width  = img.width;
        height = img.height;
        bytespp = img.bytespp;
        unsigned long nbytes = width*height*bytespp;
        data = new unsigned char[nbytes];
        memcpy(data, img.data, nbytes);
    }
    return *this;
}
void TGAImage::clear() {
    memset((void *)data, 0, width*height*bytespp);
}