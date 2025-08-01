cmake_minimum_required(VERSION 3.20)

project(renderer)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(AddGLFW)
include(AddGLM)

include_directories(deps/glad/include
    deps/imgui
    deps/imgui/backends)

set(GLAD_GL "deps/glad/src/gl.c")

set(IMGUI_SOURCES
    deps/imgui/imgui.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/backends/imgui_impl_glfw.cpp
    deps/imgui/backends/imgui_impl_opengl3.cpp
)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

include_directories(shaders)

add_executable(main src/main.cpp ${GLAD_GL}
    ${IMGUI_SOURCES}
    src/renderer.hpp 
    src/renderer.cpp 
    src/globals.hpp 
    src/globals.cpp 
    src/application.hpp 
    src/application.cpp 
    src/shader.hpp 
    src/shader.cpp 
    src/objectLoader.hpp 
    src/objectLoader.cpp
    src/mesh.hpp
    src/mesh.cpp
    src/meshHandler.hpp
    src/meshHandler.cpp
    src/entityManagementSystem.hpp
    src/entityManagementSystem.cpp
    src/camera.hpp
    src/camera.cpp)

target_link_libraries(main PRIVATE glfw glm)

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set '3D-RENDERER' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main )
    endif()
endif()
