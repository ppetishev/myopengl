#ifndef __MODEL__
#define __MODEL__

#include "geometry.h"
#include <vector>

class Model{
private:
    std::vector<Vec3f> verts;
    std::vector<std::vector<int>> faces;
public:
    Model(const char* filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    std::vector<int> face(int i);
};

#endif //__MODEL__