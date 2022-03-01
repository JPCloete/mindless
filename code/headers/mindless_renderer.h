#pragma once

#include <windows.h>
#include <gl/GL.h>
#include "opengl/khrplatform.h"
#include "opengl/glext.h"

//forward decleration stuff
struct entity;
struct uEntities;
struct v2;
struct v3;

struct spriteBufferLocations {
    float* SpriteVertexAddress;
    unsigned short* SpriteIndexAddress;
    int VertexSize;
    int IndexSize;
};  

struct renderBufferLocations {
    float* RenderVertexAddress;
    unsigned short* RenderIndexAddress;
};

struct entitySprites {
    spriteBufferLocations Tile;
    spriteBufferLocations Ship;
    spriteBufferLocations BlackHole;  
    spriteBufferLocations Bullet; 
};

void InitOpenGlFunctions();

void InitOpenGlShaders();

void Render(HDC WindowDC);

void InitBuffers_3D();

void glResizeWindow(GLsizei width, GLsizei height);

void UpdateBuffers(uEntities* UEntities, entity* Entities);