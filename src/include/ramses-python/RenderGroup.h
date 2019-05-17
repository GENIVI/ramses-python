//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_RENDERGROUP_H
#define PYTHONRAMSES_RENDERGROUP_H

#include "ramses-client-api/RenderGroup.h"

namespace RamsesPython
{
    class RenderGroup
    {
    public:
        RenderGroup(ramses::RenderGroup* group)
            : m_renderGroup(group)
        {
        }

        void addMesh(Mesh mesh, uint32_t renderOrder)
        {
            m_renderGroup->addMeshNode(*mesh.m_mesh, renderOrder);
        }

        void removeMesh(Mesh mesh)
        {
            m_renderGroup->removeMeshNode(*mesh.m_mesh);
        }

        void addRenderGroup(RenderGroup group, uint32_t renderOrder)
        {
            m_renderGroup->addRenderGroup(*group.m_renderGroup, renderOrder);
        }

        void removeRenderGroup(RenderGroup group)
        {
            m_renderGroup->removeRenderGroup(*group.m_renderGroup);
        }

        ramses::RenderGroup* m_renderGroup;
    };

}

#endif
