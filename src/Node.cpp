//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include <vector>

#include "ramses-python/Node.h"
#include <assert.h>

namespace RamsesPython
{
    Node::Node(ramses::Node* node)
        : RamsesObject(node)
        , m_node(node)
    {
    }

    void Node::addChild(Node node)
    {
        m_node->addChild(*node.m_node);
    }

    void Node::setParent(Node node)
    {
        m_node->setParent(*node.m_node);
    }

    void Node::setRotation(float x, float y, float z)
    {
        m_node->setRotation(x, y, z);
    }

    void Node::setTranslation(float x, float y, float z)
    {
        m_node->setTranslation(x, y, z);
    }

    void Node::setScaling(float x, float y, float z)
    {
        m_node->setScaling(x, y, z);
    }

    void Node::setVisibility(bool visible)
    {
        m_node->setVisibility(visible);
    }

    Node Node::getParent()
    {
        return Node{m_node->getParent()};
    }

    std::vector<float> Node::getRotation()
    {
        float x, y, z;
        ramses::status_t success = m_node->getRotation(x, y, z);
        assert(ramses::StatusOK == success);

        return std::vector<float> {x, y, z};
    }

    std::vector<float> Node::getTranslation()
    {
        float x, y, z;
        ramses::status_t success = m_node->getTranslation(x, y, z);
        assert(ramses::StatusOK == success);

        return std::vector<float> {x, y, z};
    }

    std::vector<float> Node::getScaling()
    {
        float x, y, z;
        ramses::status_t success = m_node->getScaling(x, y, z);
        assert(ramses::StatusOK == success);

        return std::vector<float> {x, y, z};
    }

    bool Node::getVisibility()
    {
        return m_node->getVisibility();
    }


}
