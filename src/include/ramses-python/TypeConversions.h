//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG, Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_TYPECONVERSIONS_H
#define PYTHONRAMSES_TYPECONVERSIONS_H

#include "ramses-client-api/MeshNode.h"
#include "ramses-client-api/PerspectiveCamera.h"
#include "ramses-client-api/OrthographicCamera.h"
#include "ramses-utils.h"

#include "ramses-python/Node.h"
#include "ramses-python/Mesh.h"
#include "ramses-python/Camera.h"

#include "assert.h"

namespace RamsesPython
{
    // Python is not type-sensitive, but C++ is -> need special utils for type conversion
    class TypeConversions
    {
    public:
        static Mesh ToMesh(RamsesObject object)
        {
            assert(object.m_object != nullptr);
            ramses::MeshNode* meshNode = ramses::RamsesUtils::TryConvert<ramses::MeshNode>(*const_cast<ramses::RamsesObject*>(object.m_object));
            // TODO rework error handling if needed
            assert(meshNode != nullptr);
            return Mesh(meshNode);
        }

        static PerspectiveCamera ToPerspectiveCamera(RamsesObject object)
        {
            assert(object.m_object != nullptr);
            ramses::PerspectiveCamera* camera = ramses::RamsesUtils::TryConvert<ramses::PerspectiveCamera>(*const_cast<ramses::RamsesObject*>(object.m_object));
            // TODO rework error handling if needed
            assert(camera != nullptr);
            return PerspectiveCamera(camera);
        }

        static OrthographicCamera ToOrthographicCamera(RamsesObject object)
        {
            assert(object.m_object != nullptr);
            ramses::OrthographicCamera* camera = ramses::RamsesUtils::TryConvert<ramses::OrthographicCamera>(*const_cast<ramses::RamsesObject*>(object.m_object));
            // TODO rework error handling if needed
            assert(camera != nullptr);
            return OrthographicCamera(camera);
        }

        static Node ToNode(RamsesObject object)
        {
            assert(object.m_object != nullptr);
            ramses::Node* node = ramses::RamsesUtils::TryConvert<ramses::Node>(*const_cast<ramses::RamsesObject*>(object.m_object));
            // TODO rework error handling if needed
            assert(node != nullptr);
            return Node(node);
        }

    };
}

#endif
