#pragma once

enum arenaType
{
    File,
    Sound,
    Render,
    Entity,
    Grid,
    Empty
};

struct arenaMetadata
{
    void* MemLoc;
    void* MemEdge;
    long Size;
    long SizeRemaining;
};

struct subArenaMetadata
{   
    long Size;

    unsigned int MemGap;
    arenaType ArenaType;

    //TODO:Do I really want to do it like this?
    char* FileArena;
};

struct adjArena 
{
    short FragmentGap;
};  

void* AllocHeadArena(arenaMetadata &ArenaMetadata);