//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_GEOMETRY_H
#define PYTHONRAMSES_GEOMETRY_H

#include "ramses-python/RamsesObject.h"
#include "ramses-python/Resource.h"
#include "ramses-client-api/GeometryBinding.h"

namespace RamsesPython
{
    class Geometry : public RamsesObject
    {
    public:
        Geometry(ramses::GeometryBinding* geometry);

        void setIndexBuffer(Resource indexResource);
        void setVertexBuffer(std::string name, Resource vertexResource);

        ramses::GeometryBinding* m_geometry;
    };
}

#endif
