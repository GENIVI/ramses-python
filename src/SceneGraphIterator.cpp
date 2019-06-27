//  -------------------------------------------------------------------------
//  Copyright (C) 2019 Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/SceneGraphIterator.h"
#include "ramses-client-api/SceneGraphIterator.h"
#include "ramses-client-api/RamsesObjectTypes.h"

namespace RamsesPython
{

    SceneGraphIterator::SceneGraphIterator(RamsesPython::Node& startNode,
                                           ramses::ETreeTraversalStyle traversalStyle,
                                           ramses::ERamsesObjectType objectType)
        :m_iter{*startNode.m_node, traversalStyle, objectType}
    {

    };

    RamsesPython::Node SceneGraphIterator::getNext()
    {
        ramses::Node* next_ptr = m_iter.getNext();

        if(!next_ptr)
            throw std::exception{};

        return RamsesPython::Node {next_ptr};
    }

}
