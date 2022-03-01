#pragma once
#include "mindless_renderer.h"

struct v4 {
    float X; 
    float Y;
    float Z;
    float W;
};

struct mat4 {
    v4 X;
    v4 Y;
    v4 Z;
    v4 W;
};

mat4 EditMat4Entries(mat4* iMat4, unsigned int rowEntry, unsigned int columnEntry, float iValue);

mat4 MultiplyMat4_Mat4(mat4* iMat4Ptr1, mat4* iMat4Ptr2);

mat4 TransposeMat4(mat4* iMat4Ptr);
