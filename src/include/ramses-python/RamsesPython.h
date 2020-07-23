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

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

#include "ramses-python/Scene.h"

namespace RamsesPython
{
    class Ramses
    {
    public:
        Ramses(std::string name);

        Scene createScene(std::string sceneName);

    private:
        static ramses::RamsesFrameworkConfig& GetStaticConfig();

        // TODO is there a way to make class copy-able, and not use shared pointers?
        std::shared_ptr<ramses::RamsesFramework> m_framework;
        ramses::RamsesClient* m_client;

        std::unordered_map<ramses::sceneId_t, ramses::Scene*> m_scenes;
    };
}

#endif
