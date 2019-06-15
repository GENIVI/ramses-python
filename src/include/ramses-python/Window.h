//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_WINDOW_H
#define PYTHONRAMSES_WINDOW_H

#include "DisplayManager/DisplayManager.h"
#include "ramses-python/Scene.h"

#include <unordered_set>

namespace RamsesPython
{
    class Window
    {
    public:
        Window(std::shared_ptr<ramses_display_manager::DisplayManager> displayManager, std::shared_ptr<ramses::RamsesRenderer> renderer, uint32_t width, uint32_t height, int32_t posX, int32_t posY);

        void showScene(Scene scene);
        void takeScreenshot(std::string file);
        void close();

    private:
        std::shared_ptr<ramses_display_manager::DisplayManager> m_displayManager;
        // TODO Only needed for screenshots - implement feature in display manager, remove renderer reference here
        std::shared_ptr<ramses::RamsesRenderer> m_renderer;
        ramses::displayId_t m_displayId;
        std::unordered_set<ramses::sceneId_t> m_shownScenes;
        const uint32_t m_width;
        const uint32_t m_height;
    };
}

#endif
