cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(renderer)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(AddGLFW)
include(AddGLM)
include(AddENTT)
include(AddYAML)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(
    deps/glad/include
    deps/imgui
    deps/imgui/backends
)

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

add_executable(main src/main.cpp ${GLAD_GL} ${IMGUI_SOURCES}
    src/Core/renderer.h
    src/Core/renderer.cpp
    src/Core/globals.h
    src/Core/globals.cpp
    src/Core/application.h
    src/Core/application.cpp
    src/Core/objectLoader.h
    src/Core/objectLoader.cpp
    src/Shader/shader.h
    src/Shader/shader.cpp
    src/Entity/mesh.h
    src/Entity/mesh.cpp
    src/Entity/meshHandler.h
    src/Entity/meshHandler.cpp
    src/Component_Functions/camera.h
    src/Component_Functions/camera.cpp
    src/Component_Functions/transform.h
    src/Component_Functions/transform.cpp
    src/UI/ui.h
    src/UI/ui.cpp
    src/Entity/components.cpp
    src/Entity/components.h
    src/Entity/entity.cpp
    src/Entity/entity.h
    src/Scene/scene.cpp
    src/Scene/scene.h
    src/Scripting/scripting.cpp
    src/Scripting/scripting.h
)

target_link_libraries(main PRIVATE glfw glm EnTT::EnTT yaml-cpp::yaml-cpp ${Python3_LIBRARIES})

target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS})

add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set '3D-RENDERER' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main )
    endif()
endif()
