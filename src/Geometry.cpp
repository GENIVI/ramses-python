//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include <string>
#include <exception>

#include "ramses-python/Geometry.h"
#include "ramses-client-api/Effect.h"
#include "ramses-client-api/GeometryBinding.h"
#include "ramses-client-api/AttributeInput.h"
#include "ramses-utils.h"

namespace RamsesPython
{
    Geometry::Geometry(ramses::GeometryBinding* geometry)
        : RamsesObject(geometry)
        , m_geometry(geometry)
    {
    }

    void Geometry::setIndexBuffer(Resource indexResource)
    {
        if (indexResource.m_resource->getType() == ramses::ERamsesObjectType_UInt16Array)
            m_geometry->setIndices(*ramses::RamsesUtils::TryConvert<ramses::UInt16Array>(*indexResource.m_resource));
        else if (indexResource.m_resource->getType() == ramses::ERamsesObjectType_UInt32Array)
            m_geometry->setIndices(*ramses::RamsesUtils::TryConvert<ramses::UInt32Array>(*indexResource.m_resource));
        else
        {
            std::string msg {"Expected resource to be either ramses::ERamsesObjectType_UInt16Array or ramses::ERamsesObjectType_UInt32Array"};
            throw std::runtime_error{msg};
        }

    }

    void Geometry::setVertexBuffer(std::string name, Resource vertexResource)
    {
        ramses::AttributeInput attribute;
        ramses::status_t attributeFound = m_geometry->getEffect().findAttributeInput(name.c_str(), attribute);

        if (attributeFound != ramses::StatusOK)
        {
            std::string msg {"Attribute not found!"};
            throw std::runtime_error{msg};
        }

        if (attribute.getDataType() == ramses::EEffectInputDataType_Float)
            m_geometry->setInputBuffer(attribute, *ramses::RamsesUtils::TryConvert<ramses::FloatArray>(*vertexResource.m_resource));
        else if (attribute.getDataType() == ramses::EEffectInputDataType_Vector2F)
            m_geometry->setInputBuffer(attribute, *ramses::RamsesUtils::TryConvert<ramses::Vector2fArray>(*vertexResource.m_resource));
        else if (attribute.getDataType() == ramses::EEffectInputDataType_Vector3F)
            m_geometry->setInputBuffer(attribute, *ramses::RamsesUtils::TryConvert<ramses::Vector3fArray>(*vertexResource.m_resource));
        else if (attribute.getDataType() == ramses::EEffectInputDataType_Vector4F)
            m_geometry->setInputBuffer(attribute, *ramses::RamsesUtils::TryConvert<ramses::Vector4fArray>(*vertexResource.m_resource));
        else
        {
            std::string msg {"Expected attribute to be either FloatArray or Vector2fArray or Vector3fArray or Vector4fArray"};
            throw std::runtime_error{msg};
        }
    }
}
