//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/Window.h"
#include "ramses-renderer-api/DisplayConfig.h"
#include "ramses-renderer-api/RamsesRenderer.h"
// TODO should not use ramses internals
#include "Utils/Image.h"

#include <vector>
#include <thread>
#include <cstdlib>

namespace RamsesPython
{
    Window::Window(ramses_display_manager::DisplayManager* displayManager, ramses::RamsesRenderer* renderer, uint32_t width, uint32_t height, int32_t posX, int32_t posY)
        : m_displayManager(displayManager)
        , m_renderer(renderer)
        , m_displayId(ramses::InvalidDisplayId)
        , m_width(width)
        , m_height(height)
    {
        ramses::DisplayConfig config;
        config.setWindowRectangle(posX, posY, width, height);
        m_displayId = m_displayManager->createDisplay(config);
        while (!m_displayManager->isDisplayCreated(m_displayId))
            m_displayManager->dispatchAndFlush();
    }

    void Window::showScene(Scene& scene)
    {
        //std::thread t ([&] {this->_showScene_internal(scene);});
        //t.join();
        _showScene_internal(scene);
    }

    void Window::_showScene_internal(Scene& scene)
    {

        scene.m_scene->flush();
        scene.m_scene->publish();

        auto sceneId = scene.m_scene->getSceneId();
        m_displayManager->setSceneMapping(sceneId, m_displayId);
        m_displayManager->setSceneState(sceneId, ramses_display_manager::SceneState::Rendered);
        while (m_displayManager->getLastReportedSceneState(sceneId) != ramses_display_manager::SceneState::Rendered)
            m_displayManager->dispatchAndFlush();

        m_shownScenes.insert(sceneId);
    }

    class ScreenshotSaver : public ramses::RendererEventHandlerEmpty
    {
    public:
        ScreenshotSaver(uint32_t width, uint32_t height)
        {
            pixels.resize(4 * width * height);
        }

        virtual void framebufferPixelsRead(const uint8_t* pixelData, const uint32_t pixelDataSize, ramses::displayId_t displayId, ramses::ERendererEventResult result) override final
        {
            assert(result == ramses::ERendererEventResult_OK && pixelDataSize == pixels.size());

            std::memcpy(pixels.data(), pixelData, pixelDataSize);
            screenshotCaptured = true;
        }

        bool screenshotCaptured = false;
        std::vector<uint8_t> pixels;
    };

    void Window::takeScreenshot(std::string filename)
    {
        m_renderer->readPixels(m_displayId, 0, 0, m_width, m_height);
        m_renderer->flush();

        auto saveScreenshot = []() {};
        ScreenshotSaver screenshotSaver(m_width, m_height);
        while (!screenshotSaver.screenshotCaptured)
        {
            m_renderer->dispatchEvents(screenshotSaver);
        }

        ramses_internal::Image image(m_width, m_height, screenshotSaver.pixels.cbegin(), screenshotSaver.pixels.cend(), true);
        image.saveToFilePNG(filename.c_str());
    }

    void Window::close()
    {
        // Unsubscribe all scenes
        for (auto sceneId : m_shownScenes)
            m_displayManager->setSceneState(sceneId, ramses_display_manager::SceneState::Unavailable);

        // Wait until all scenes were successfully unsubscribed before destroying the display
        bool anySceneAvailable = true;
        while (anySceneAvailable)
        {
            anySceneAvailable = false;
            for (auto sceneId : m_shownScenes)
            {
                if (m_displayManager->getLastReportedSceneState(sceneId) != ramses_display_manager::SceneState::Unavailable)
                {
                    anySceneAvailable = true;
                    break;
                }
            }

            m_displayManager->dispatchAndFlush();
        }
        //// TODO this is a pretty bad hack, remove once DisplayManager can synchronously wait until all scenes were unmapped
        //for (int i = 0; i < 20; ++i)
        //{
        //    m_displayManager->dispatchAndFlush();
        //    std::this_thread::sleep_for(10ms);
        //}

        // Destroy display
        m_displayManager->destroyDisplay(m_displayId);
        while (m_displayManager->isDisplayCreated(m_displayId))
            m_displayManager->dispatchAndFlush();
    }
}
