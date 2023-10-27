#ifndef __MODEL__
#define __MODEL__

#include "geometry.h"
#include <vector>

class Model{
private:
    std::vector<Vec3f> verts;
    std::vector<std::vector<int>> faces;
    float max_vert;
public:
    Model(const char* filename);
    ~Model();
    int nverts();
    int nfaces();
    float get_max_vert();
    Vec3f vert(int i);
    std::vector<int> face(int i);
};

#endif //__MODEL__