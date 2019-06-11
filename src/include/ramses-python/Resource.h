//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_RESOURCE_H
#define PYTHONRAMSES_RESOURCE_H

#include "ramses-python/RamsesObject.h"
#include "ramses-client-api/Resource.h"

namespace RamsesPython
{
    class Resource : public RamsesObject
    {
    public:
        Resource(const ramses::Resource* resource)
            : RamsesObject(resource)
            , m_resource(resource)
        {
        }

        const ramses::Resource* m_resource;
    };
}

#endif
