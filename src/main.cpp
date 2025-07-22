#pragma once

#include <iostream>
#include "application.hpp"
#include "globals.hpp"
#include "ObjectLoader.hpp"

# include <windows.h>



int main(void)
{
    ObjectRenderer::Application application(ObjectRenderer::g_width, ObjectRenderer::g_height);
    application.run();
}
