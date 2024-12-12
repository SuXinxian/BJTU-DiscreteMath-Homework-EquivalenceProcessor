cmake_minimum_required(VERSION 3.14)
project(DiscreteMathProject)


set(CMAKE_CXX_STANDARD 17)


if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()


add_executable(DiscreteMathProject
        main.cpp
        relation_processor.cpp
        test_generator.cpp
)