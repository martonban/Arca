cmake_minimum_required(VERSION 3.10)
project(Arca)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMNAKE_CXX_EXTENSIONS OFF)

file(GLOB ARCA_SOURCES src/*.cpp)
file(GLOB_RECURSE ARCA_HEADERS include/*.h include/*.hpp)


add_library(Arca STATIC
    ${ARCA_SOURCES}
    ${ARCA_HEADERS}
)

set_target_properties(Arca PROPERTIES
    PUBLIC_HEADER "${ARCA_HEADERS}"
)

target_include_directories(Arca
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_custom_command(TARGET Arca POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:Arca>/include"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/include"
            "$<TARGET_FILE_DIR:Arca>/include"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${json_SOURCE_DIR}/single_include/nlohmann"
        "$<TARGET_FILE_DIR:Arca>/include/nlohmann"
)

target_link_libraries(Arca PUBLIC nlohmann_json::nlohmann_json)