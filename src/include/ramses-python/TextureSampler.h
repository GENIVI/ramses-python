//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_TEXTURESAMPLER_H
#define PYTHONRAMSES_TEXTURESAMPLER_H

#include "ramses-python/RamsesObject.h"
#include "ramses-client-api/TextureSampler.h"

namespace RamsesPython
{
    class TextureSampler : public RamsesObject
    {
    public:
        TextureSampler(ramses::TextureSampler* textureSampler)
            : RamsesObject(textureSampler)
            , m_textureSampler(textureSampler)
        {
        }

        // TODO

        ramses::TextureSampler* m_textureSampler;
    };
}

#endif
