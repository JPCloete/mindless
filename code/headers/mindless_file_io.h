#pragma once

struct file 
{
    void* Pointer;
    int Size;
};

struct shaderBuffer 
{
    void* Pointer;
    unsigned int Size;
};

file ReadCharFile(const char fileName[]);
