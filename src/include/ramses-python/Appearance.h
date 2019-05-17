//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_APPEARANCE_H
#define PYTHONRAMSES_APPEARANCE_H

#include <string>
#include <vector>

#include "ramses-client-api/Appearance.h"

#include "ramses-python/TextureSampler.h"

namespace RamsesPython
{
    class Appearance
    {
    public:
        Appearance(ramses::Appearance* appearance);

        void setUniformFloat(std::string name, const std::vector<float>& uniformValues);
        void setUniformInt(std::string name, const std::vector<int32_t>& uniformValues);
        void setTexture(std::string name, TextureSampler textureSampler);

        ramses::Appearance* m_appearance;
    };

}

#endif
