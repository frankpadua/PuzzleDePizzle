cmake_minimum_required(VERSION 3.15)
project(PuzzleDePizzle)

set(CMAKE_CXX_STANDARD 17)

# Add source files from src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Raylib linking
if(WIN32)
    # Use vcpkg
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    endif()
    find_package(raylib CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
elseif(UNIX)
    # Linux uses system raylib
    find_package(raylib REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif()

# Optional: output to /build/bin
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

