#pragma once

#include "application.hpp"
#include "globals.hpp"
#include "ObjectLoader.hpp"

int main(void)
{
    ObjectRenderer::Application application(ObjectRenderer::g_width, ObjectRenderer::g_height);
    application.run();
}
