//  -------------------------------------------------------------------------
//  Copyright (C) 2019 Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_SCENEGRAPHITERATOR_H
#define PYTHONRAMSES_SCENEGRAPHITERATOR_H

#include "ramses-python/Node.h"
#include "ramses-client-api/SceneGraphIterator.h"
#include "ramses-client-api/RamsesObjectTypes.h"

namespace RamsesPython
{

    class SceneGraphIterator
    {
        public:
            SceneGraphIterator(RamsesPython::Node& startNode,
                               ramses::ETreeTraversalStyle traversalStyle,
                               ramses::ERamsesObjectType objectType);

            RamsesPython::Node getNext();

        private:
            ramses::SceneGraphIterator m_iter;
    };

}

#endif
