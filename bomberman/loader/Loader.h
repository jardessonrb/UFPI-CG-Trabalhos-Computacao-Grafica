#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Loader {

public:
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<std::vector<int>> faces;
    Loader();
    ~Loader();
    void loadOBJ(std::string path);
};

#endif // LOADER_H