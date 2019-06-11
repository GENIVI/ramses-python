
//  -------------------------------------------------------------------------
//  Copyright (C) 2019 Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_RAMSESOBJECT_H
#define PYTHONRAMSES_RAMSESOBJECT_H

#include <string>

#include "ramses-client-api/RamsesObject.h"
#include "ramses-client-api/RamsesObjectTypes.h"

namespace RamsesPython
{
    class RamsesObject
    {
        public:
            RamsesObject(const ramses::RamsesObject* ramsesObject);
            std::string getName();
            bool isOfType(ramses::ERamsesObjectType type) const;
            ramses::ERamsesObjectType getType() const;
        private:
            const ramses::RamsesObject* m_object;

    };
}
#endif
