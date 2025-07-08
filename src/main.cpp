#pragma once

#include <iostream>
#include "application.hpp"
#include "globals.hpp"
#include "ObjectLoader.hpp"

# include <windows.h>



int main(void)
{
    char buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);
    std::cout << buffer << std::endl;
    ObjectRenderer::Application application(ObjectRenderer::g_width, ObjectRenderer::g_height);
    application.run();
}
