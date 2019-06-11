//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_MESH_H
#define PYTHONRAMSES_MESH_H

#include "ramses-client-api/MeshNode.h"
#include "ramses-python/Node.h"
#include "ramses-python/Appearance.h"
#include "ramses-python/Geometry.h"

namespace RamsesPython
{
    class Mesh : public Node
    {
    public:
        Mesh(ramses::MeshNode* mesh)
            : Node(mesh)
            , m_mesh(mesh)
        {
        }

        void setAppearance(Appearance appearance)
        {
            m_mesh->setAppearance(*appearance.m_appearance);
        }

        Appearance getAppearance() const
        {
            return Appearance(m_mesh->getAppearance());
        }

        void setGeometry(Geometry geometry)
        {
            m_mesh->setGeometryBinding(*geometry.m_geometry);
        }

        Geometry getGeometry() const
        {
            return Geometry(m_mesh->getGeometryBinding());
        }

        ramses::MeshNode* m_mesh;
    };

}

#endif
