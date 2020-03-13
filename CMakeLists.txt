cmake_minimum_required(VERSION 3.10)
project(PBMM CXX CUDA)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --ptxas-options=-v -Xptxas -O3 -v")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/cpp ${CMAKE_SOURCE_DIR}/PBMM-cpu)

add_executable(valiantCPU 
        PBMM-cpu/valiant/main.cpp 
        cpp/Grammar.cpp 
        cpp/Grammar.h
        cpp/Graph.cpp 
        cpp/Graph.h  
        cpp/Bound.cpp 
        cpp/Bound.h 
        cpp/Layer.cpp 
        cpp/Layer.h 
        PBMM-cpu/ParsingTables.cpp 
        PBMM-cpu/ParsingTables.h 
        PBMM-cpu/valiant/ValiantParsing.cpp 
        PBMM-cpu/valiant/ValiantParsing.h)
target_link_libraries(valiantCPU m4ri)

include_directories(${CMAKE_SOURCE_DIR}/cpp ${CMAKE_SOURCE_DIR}/PBMM-cpu)

add_executable(modificationCPU 
        PBMM-cpu/modification/main.cpp 
        cpp/Grammar.cpp 
        cpp/Grammar.h 
        cpp/Graph.cpp 
        cpp/Graph.h 
        cpp/Bound.cpp 
        cpp/Bound.h 
        cpp/Layer.cpp 
        cpp/Layer.h 
        PBMM-cpu/ParsingTables.cpp 
        PBMM-cpu/ParsingTables.h 
        PBMM-cpu/modification/ModifiedParsing.cpp 
        PBMM-cpu/modification/ModifiedParsing.h)
target_link_libraries(modificationCPU m4ri)

include_directories(${CMAKE_SOURCE_DIR}/cpp ${CMAKE_SOURCE_DIR}/PBMM-cpu)

add_executable(modificationCPU-substrings 
        PBMM-cpu/modification/main-substrings.cpp 
        cpp/Grammar.cpp 
        cpp/Grammar.h 
        cpp/Graph.cpp 
        cpp/Graph.h 
        cpp/Bound.cpp 
        cpp/Bound.h 
        cpp/Layer.cpp 
        cpp/Layer.h 
        PBMM-cpu/ParsingTables.cpp 
        PBMM-cpu/ParsingTables.h 
        PBMM-cpu/modification/ModifiedParsing.cpp 
        PBMM-cpu/modification/ModifiedParsing.h)
target_link_libraries(modificationCPU-substrings m4ri)

include_directories(${CMAKE_SOURCE_DIR}/cpp ${CMAKE_SOURCE_DIR}/PBMM-gpu)

add_executable(valiantGPU
        PBMM-gpu/valiant/main.cpp
        cpp/Grammar.cpp 
        cpp/Grammar.h 
        cpp/Graph.cpp 
        cpp/Graph.h 
        cpp/Layer.cpp 
        cpp/Layer.h 
        cpp/Bound.cpp 
        cpp/Bound.h 
        PBMM-gpu/ParsingTable.h
        PBMM-gpu/ParsingTable.cpp
        PBMM-gpu/GpuMatrix.h
        PBMM-gpu/GpuMatrix.cpp
        PBMM-gpu/Multiplication.cu
        PBMM-gpu/Multiplication.h
        PBMM-gpu/Parameters.h
        )

include_directories(${CMAKE_SOURCE_DIR}/cpp ${CMAKE_SOURCE_DIR}/PBMM-gpu)

add_executable(modificationGPU
        PBMM-gpu/modification/main.cpp
        cpp/Grammar.cpp 
        cpp/Grammar.h 
        cpp/Graph.cpp 
        cpp/Graph.h 
        cpp/Layer.cpp 
        cpp/Layer.h 
        cpp/Bound.cpp 
        cpp/Bound.h 
        PBMM-gpu/ParsingTable.h
        PBMM-gpu/ParsingTable.cpp        
        PBMM-gpu/GpuMatrix.h
        PBMM-gpu/GpuMatrix.cpp
        PBMM-gpu/Multiplication.cu
        PBMM-gpu/Multiplication.h
        PBMM-gpu/Parameters.h
        )

include_directories(${CMAKE_SOURCE_DIR}/cpp ${CMAKE_SOURCE_DIR}/PBMM-gpu)

add_executable(modificationGPU-substrings
        PBMM-gpu/modification/main-substrings.cpp
        cpp/Grammar.cpp 
        cpp/Grammar.h 
        cpp/Graph.cpp 
        cpp/Graph.h 
        cpp/Layer.cpp 
        cpp/Layer.h 
        cpp/Bound.cpp 
        cpp/Bound.h 
        PBMM-gpu/ParsingTable.h
        PBMM-gpu/ParsingTable.cpp        
        PBMM-gpu/GpuMatrix.h
        PBMM-gpu/GpuMatrix.cpp
        PBMM-gpu/Multiplication.cu
        PBMM-gpu/Multiplication.h
        PBMM-gpu/Parameters.h
        )

