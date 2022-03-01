cmake_minimum_required(VERSION 3.20)
project(mindless_open_gl)

set(CMAKE_CXX_STANDARD 14)

add_executable(mindless_open_gl
	WIN32
        code/source/win32_mindless.cpp
	code/source/mindless_entities.cpp
	code/source/mindless_renderer.cpp
	code/source/mindless_trig.cpp	
	code/source/mindless_file_io.cpp
	code/source/mindless_memory.cpp
		)
