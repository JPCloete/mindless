#include<windows.h>
#include<winbase.h>
#include<fileapi.h>
#include<malloc.h>

#include "../headers/mindless_file_io.h"

// //returns pointer to char array
// file ReadCharFile(const char* fileName[])
// {   
//     HANDLE fileHandle;
//     char fileBuffer[20];
//     file = CreateFile(fileName,GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

//     DWORD nRead;
//     ReadFile(file, fileBuffer, 40, &nRead, NULL);
//     CloseHandle(fileHandle);
//     return 0;
// }

