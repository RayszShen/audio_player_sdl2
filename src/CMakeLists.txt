cmake_minimum_required(VERSION 3.10)

project(AudioPlayer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_DIR)
file(GLOB_RECURSE SRC_FILE
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILE}        
)

target_link_libraries(${PROJECT_NAME}
    SDL2
)