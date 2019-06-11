//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_RENDERPASS_H
#define PYTHONRAMSES_RENDERPASS_H

#include "ramses-python/RamsesObject.h"
#include "ramses-python/Camera.h"
#include "ramses-client-api/RenderPass.h"

namespace RamsesPython
{
    class RenderPass : public RamsesObject
    {
    public:
        RenderPass(ramses::RenderPass* pass)
            : RamsesObject(pass)
            , m_renderPass(pass)
        {
        }

        void addRenderGroup(RenderGroup group, uint32_t renderOrder)
        {
            m_renderPass->addRenderGroup(*group.m_renderGroup, renderOrder);
        }

        void removeRenderGroup(RenderGroup group)
        {
            m_renderPass->removeRenderGroup(*group.m_renderGroup);
        }

        void setCamera(Camera camera)
        {
            m_renderPass->setCamera(*camera.m_camera);
        }

        ramses::RenderPass* m_renderPass;
    };

}

#endif
