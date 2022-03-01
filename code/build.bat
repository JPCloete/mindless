@echo off
mkdir ..\build
pushd ..\build
cl -Zi ..\code\source\win32_mindless.cpp ..\code\source\mindless_entities.cpp ..\code\source\mindless_renderer.cpp ..\code\source\mindless_file_io.cpp ..\code\source\mindless_matrix.cpp ..\code\source\mindless_trig.cpp ..\code\source\mindless_memory.cpp 
popd