#include "model.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>


Model::Model(const char* filename){
    std::ifstream istr(filename);
    if (!istr.is_open()){
        std::cerr << "can't read file " << filename << std::endl;
        return;
    }
    std::string line;
    std::vector<int> face;
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
            for (int i = 0; i<3; i++) iss >>v.raw[i];
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