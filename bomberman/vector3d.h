#pragma once
#include <cmath>

struct vector3d {
    float x, y, z;

    vector3d() : x(0.0f), y(0.0f), z(0.0f) {}
    vector3d(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    vector3d operator + (const vector3d& vec) const {
        return vector3d(x + vec.x, y + vec.y, z + vec.z);
    }

    vector3d operator * (const float vec) const {
        return vector3d(x * vec, y * vec, z * vec);
    }

    float magnitude() {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    void normalizar() {
        float magn = magnitude();
        x = x / magn;
        y = y / magn;
        z = z / magn;
    }
};