#include "loader.h"

Loader::Loader()
{
}

Loader::~Loader()
{
}

void Loader::loadOBJ(std::string path) {
    this->faces.clear();
    this->vertices.clear();
    this->normals.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            Vector3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            this->vertices.push_back(vertex);
        }
        else if (prefix == "vn") {
            Vector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            this->normals.push_back(normal);
        }
        else if (prefix == "f") {
            std::vector<int> face;
            std::string vertex;
            while (iss >> vertex) {
                std::istringstream viss(vertex);
                int index;
                viss >> index;
                face.push_back(index - 1);  // Convert to 0-based index
            }
            this->faces.push_back(face);
        }
    }

    // std::cout << "Loaded " << vertices.size() << " vertices, " << normals.size() << " normals, " << faces.size() << " faces." << std::endl;

    file.close();

}
