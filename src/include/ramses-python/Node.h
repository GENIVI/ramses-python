//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_NODE_H
#define PYTHONRAMSES_NODE_H

#include "ramses-client-api/Node.h"
#include "ramses-python/RamsesObject.h"

namespace RamsesPython
{
    class Node : public RamsesObject
    {
    public:
        Node(ramses::Node* node);

        void addChild(Node node);
        void setParent(Node node);
        void setRotation(float x, float y, float z);
        void setTranslation(float x, float y, float z);
        void setScaling(float x, float y, float z);
        void setVisibility(bool visible);

        ramses::Node* m_node;
    };
}

#endif
