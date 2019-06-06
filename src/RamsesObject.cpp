
//  -------------------------------------------------------------------------
//  Copyright (C) 2019 Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------


#include "ramses-client-api/RamsesObject.h"
#include "ramses-client-api/RamsesObjectTypes.h"

#include "ramses-python/RamsesObject.h"

namespace RamsesPython
{
    RamsesObject::RamsesObject(ramses::RamsesObject* ramsesObject) : m_object(ramsesObject)
    {
    };

    std::string RamsesObject::getName()
    {
        const char* name = m_object->getName();
        return std::string{name};
    }

    bool RamsesObject::isOfType(ramses::ERamsesObjectType type) const
    {
        return m_object->isOfType(type);
    }

    ramses::ERamsesObjectType RamsesObject::getType() const
    {
        return m_object->getType();
    }
}
