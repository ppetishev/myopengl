#include "model.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>


Model::Model(const char* filename){
    std::ifstream istr(filename);
    if (!istr.is_open()){
        std::cout << "can't read file " << filename << std::endl;
        return;
    }
    std::string line;
    std::vector<int> face;
    max_vert = std::numeric_limits<float>::min();
    while (!istr.eof()){
        getline(istr, line);
        std::istringstream iss(line);
        char trash, det;
        
        iss >> det;
        switch (det){
            case '#':
                break;
            case 'v':{
                Vec3f v;
                for (int i = 0; i<3; i++) {
                    iss >>v.raw[i]; 
                    if (max_vert < v.raw[i]) 
                    max_vert = v.raw[i];
                }
                verts.push_back(v);
                break;
            }
            case 'f':{
                face.clear();
                int itrash, idx;
                while (iss >> idx >> trash >> itrash >> trash >>itrash)
                    face.push_back(--idx);
                faces.push_back(face);
                break;
            }
        }
    }
     
}

Model::~Model(){
}
float Model::get_max_vert(){
    return max_vert;
}
int Model::nverts(){
    return (int)verts.size();
}
int Model::nfaces(){
    return (int)faces.size();
}
Vec3f Model::vert(int i){
    return verts[i];
}
std::vector<int> Model::face(int i){
    return faces[i];
}