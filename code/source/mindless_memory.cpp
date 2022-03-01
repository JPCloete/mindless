#define NETBUDGET 2147483648; //2GB RAM
#include "../headers/mindless_memory.h"
#include <malloc.h>
#include <windows.h>

void AllocHeadArena(arenaMetadata &ArenaMetadata)
{
    void* headArena = malloc(NETBUDGET);
    
    ArenaMetadata->MemLoc = headArena;
    ArenaMetadata->Size = NETBUDGET;
    ArenaMetadata->SizeRemaining = SizeRemaining;
}

Arena* AllocSubArena(arenaMetadata &ArenaMetadata, size_t size, arenaType arenaType) {
    
    subArenaMetadata SubArenaMetaData;

    ArenaMetadata->SizeRemaining -= (size + sizeof(SubArenaMetaData) + 1);
    if(ArenaMetadata->SizeRemaining < 0)
    {
        //CRASH? IDK LULE
    }

    switch(arenaType)
    {
        case aFile: {
            subArenaMetadata* fileBufferPointer = (SubArenaMetadata*)ArenaMetadata->MemEdge;
            
        }
        break;

        case Sound: {
            
        }
        break;

        case Render: {
            
        }
        break;

        // case EntityBuffer: {

        // }

        default: {
            
        }
        break;
    }

    ArenaMetadata->MemEdge += (size + sizeof(SubArenaMetaData) + 1);
}

// void FreeHeadArena(void* arena)
// {

// }

// void FreeSubArena(void* arena)
// {

// }