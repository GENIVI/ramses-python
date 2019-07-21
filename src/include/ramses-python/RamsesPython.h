//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_RAMSESPYTHON_H
#define PYTHONRAMSES_RAMSESPYTHON_H

#include "ramses-client-api/RamsesClient.h"
#include "ramses-renderer-api/RamsesRenderer.h"
#include "DisplayManager/DisplayManager.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>

#include "ramses-python/Scene.h"
#include "ramses-python/Window.h"

namespace RamsesPython
{

    class Ramses
    {
    public:
        Ramses(std::string name);
        ~Ramses();
        Scene createScene(std::string sceneName);
        Window openWindow(uint32_t width, uint32_t height, int32_t posX = 20, int32_t posY = 20);

    private:
        static ramses::RamsesFrameworkConfig& GetStaticConfig();

        // TODO is there a way to make class copy-able, and not use shared pointers?
        ramses::RamsesFramework* m_framework = nullptr;
        ramses::RamsesClient* m_client = nullptr;
        ramses::RamsesRenderer* m_renderer = nullptr;
        ramses_display_manager::DisplayManager* m_displayManager = nullptr;

        std::unordered_map<ramses::sceneId_t, ramses::Scene*> m_scenes;
        std::vector<Window*> m_open_windows;
    };
}

#endif
