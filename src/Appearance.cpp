//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/Appearance.h"
#include "ramses-client.h"
#include <assert.h>

namespace RamsesPython
{
    Appearance::Appearance(ramses::Appearance* appearance)
        : m_appearance(appearance)
    {
    }

    void Appearance::setUniformFloat(std::string name, const std::vector<float>& uniformValues)
    {
        ramses::UniformInput uniform;
        ramses::status_t uniformFound = m_appearance->getEffect().findUniformInput(name.c_str(), uniform);
        if (uniformFound == ramses::StatusOK)
        {
            switch (uniform.getDataType())
            {
            case ramses::EEffectInputDataType_Float:
                assert(uniformValues.size() == uniform.getElementCount());
                m_appearance->setInputValueFloat(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Vector2F:
                assert(uniformValues.size() == uniform.getElementCount() * 2);
                m_appearance->setInputValueVector2f(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Vector3F:
                assert(uniformValues.size() == uniform.getElementCount() * 3);
                m_appearance->setInputValueVector3f(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Vector4F:
                assert(uniformValues.size() == uniform.getElementCount() * 4);
                m_appearance->setInputValueVector4f(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Matrix22F:
                assert(uniformValues.size() == uniform.getElementCount() * 4);
                m_appearance->setInputValueMatrix22f(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Matrix33F:
                assert(uniformValues.size() == uniform.getElementCount() * 9);
                m_appearance->setInputValueMatrix33f(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Matrix44F:
                assert(uniformValues.size() == uniform.getElementCount() * 16);
                m_appearance->setInputValueMatrix44f(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            default:
                assert(false && "uniform not found");
            }
        }
        // TODO error handling
    }

    void Appearance::setUniformInt(std::string name, const std::vector<int32_t>& uniformValues)
    {
        ramses::UniformInput uniform;
        ramses::status_t uniformFound = m_appearance->getEffect().findUniformInput(name.c_str(), uniform);
        if (uniformFound == ramses::StatusOK)
        {
            switch (uniform.getDataType())
            {
            case ramses::EEffectInputDataType_Int32:
                assert(uniformValues.size() == uniform.getElementCount());
                m_appearance->setInputValueInt32(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Vector2I:
                assert(uniformValues.size() == uniform.getElementCount() * 2);
                m_appearance->setInputValueVector2i(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Vector3I:
                assert(uniformValues.size() == uniform.getElementCount() * 3);
                m_appearance->setInputValueVector3i(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            case ramses::EEffectInputDataType_Vector4I:
                assert(uniformValues.size() == uniform.getElementCount() * 4);
                m_appearance->setInputValueVector4i(uniform, uniform.getElementCount(), (uniformValues).data());
                break;
            default:
                assert(false && "uniform not found");
            }
            // TODO error handling
        }
    }

    void Appearance::setTexture(std::string name, TextureSampler textureSampler)
    {
        ramses::UniformInput uniform;
        ramses::status_t uniformFound = m_appearance->getEffect().findUniformInput(name.c_str(), uniform);
        if (uniformFound == ramses::StatusOK && uniform.getDataType() == ramses::EEffectInputDataType_TextureSampler)
        {
            m_appearance->setInputTexture(uniform, *textureSampler.m_textureSampler);
            // TODO error handling
        }
    }
}
