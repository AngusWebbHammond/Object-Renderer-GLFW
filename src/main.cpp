#pragma once

#include <iostream>

#include "Core/application.hpp"
#include "Core/globals.hpp"

#include "Entity/entity.hpp"
#include "Scene/scene.hpp"

int main(void)
{
    ObjectRenderer::Application application(ObjectRenderer::g_width, ObjectRenderer::g_height);
    application.run();
}
