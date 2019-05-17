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

namespace RamsesPython
{

    class Node
    {
    public:
        Node(ramses::Node* node)
            : m_node(node)
        {
        }

        void addChild(Node node)
        {
            m_node->addChild(*node.m_node);
        }

        void setParent(Node node)
        {
            m_node->setParent(*node.m_node);
        }

        void setRotation(float x, float y, float z)
        {
            m_node->setRotation(x, y, z);
        }

        void setTranslation(float x, float y, float z)
        {
            m_node->setTranslation(x, y, z);
        }

        void setScaling(float x, float y, float z)
        {
            m_node->setScaling(x, y, z);
        }

        void setVisibility(bool visible)
        {
            m_node->setVisibility(visible);
        }

        ramses::Node* m_node;
    };
}

#endif
