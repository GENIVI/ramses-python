//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/SceneToText.h"

#include "ramses-client.h"
#include "ramses-utils.h"

// TODO should not use ramses internals
#include "EffectInputImpl.h"
#include "GeometryBindingImpl.h"
#include "Scene/ClientScene.h"

namespace RamsesPython
{
    class UniformUtils
    {
    public:
        static ramses::EEffectInputDataType GetUniformBaseType(ramses::EEffectInputDataType fullType)
        {
            switch (fullType)
            {
            case ramses::EEffectInputDataType_Float:
            case ramses::EEffectInputDataType_Vector2F:
            case ramses::EEffectInputDataType_Vector3F:
            case ramses::EEffectInputDataType_Vector4F:
            case ramses::EEffectInputDataType_Matrix22F:
            case ramses::EEffectInputDataType_Matrix33F:
            case ramses::EEffectInputDataType_Matrix44F:
                return ramses::EEffectInputDataType_Float;
            case ramses::EEffectInputDataType_Int32:
            case ramses::EEffectInputDataType_UInt16:
            case ramses::EEffectInputDataType_UInt32:
            case ramses::EEffectInputDataType_Vector2I:
            case ramses::EEffectInputDataType_Vector3I:
            case ramses::EEffectInputDataType_Vector4I:
                return ramses::EEffectInputDataType_Int32;
            case ramses::EEffectInputDataType_TextureSampler:
                return ramses::EEffectInputDataType_TextureSampler;
            default:
                assert(false);
                return ramses::EEffectInputDataType_Invalid;
            }
        }

        static const char* GetUniformShortTypeName(ramses::EEffectInputDataType fullType)
        {
            switch (fullType)
            {
            case ramses::EEffectInputDataType_Float:
                return "float";
            case ramses::EEffectInputDataType_Vector2F:
                return "vec2";
            case ramses::EEffectInputDataType_Vector3F:
                return "vec3";
            case ramses::EEffectInputDataType_Vector4F:
                return "vec4";
            case ramses::EEffectInputDataType_Matrix22F:
                return "mat2";
            case ramses::EEffectInputDataType_Matrix33F:
                return "mat3";
            case ramses::EEffectInputDataType_Matrix44F:
                return "mat4";
            case ramses::EEffectInputDataType_Int32:
                return "int";
            case ramses::EEffectInputDataType_UInt16:
                return "uint16";
            case ramses::EEffectInputDataType_UInt32:
                return "uint32";
            case ramses::EEffectInputDataType_Vector2I:
                return "vec2i";
            case ramses::EEffectInputDataType_Vector3I:
                return "vec3i";
            case ramses::EEffectInputDataType_Vector4I:
                return "vec4i";
            case ramses::EEffectInputDataType_TextureSampler:
                return "sampler";
            default:
                assert(false);
                return "invalid";
            }
        }

        static uint32_t GetUniformBaseSize(const ramses::UniformInput& uniform)
        {
            switch (uniform.getDataType())
            {
            case ramses::EEffectInputDataType_Float:
            case ramses::EEffectInputDataType_Int32:
            case ramses::EEffectInputDataType_UInt16:
            case ramses::EEffectInputDataType_UInt32:
                return 1;
            case ramses::EEffectInputDataType_Vector2F:
            case ramses::EEffectInputDataType_Vector2I:
                return 2;
            case ramses::EEffectInputDataType_Vector3F:
            case ramses::EEffectInputDataType_Vector3I:
                return 3;
            case ramses::EEffectInputDataType_Vector4F:
            case ramses::EEffectInputDataType_Vector4I:
                return 4;
            case ramses::EEffectInputDataType_Matrix22F:
                return 4;
            case ramses::EEffectInputDataType_Matrix33F:
                return 9;
            case ramses::EEffectInputDataType_Matrix44F:
                return 16;
            case ramses::EEffectInputDataType_TextureSampler:
                return 1;
            default:
                assert(false);
                return 0;
            }
        }

        static void GetUniformValues(const ramses::Appearance& appearance, const ramses::UniformInput& uniform, std::vector<float>& values)
        {
            switch (uniform.getDataType())
            {
            case ramses::EEffectInputDataType_Float:
                appearance.getInputValueFloat(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Vector2F:
                appearance.getInputValueVector2f(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Vector3F:
                appearance.getInputValueVector3f(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Vector4F:
                appearance.getInputValueVector4f(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Matrix22F:
                appearance.getInputValueMatrix22f(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Matrix33F:
                appearance.getInputValueMatrix33f(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Matrix44F:
                appearance.getInputValueMatrix44f(uniform, uniform.getElementCount(), values.data());
                break;
            default:
                assert(false);
                break;
            }
        }

        static void GetUniformValues(const ramses::Appearance& appearance, const ramses::UniformInput& uniform, std::vector<int32_t>& values)
        {
            switch (uniform.getDataType())
            {
            case ramses::EEffectInputDataType_Int32:
            case ramses::EEffectInputDataType_UInt16:
            case ramses::EEffectInputDataType_UInt32:
                appearance.getInputValueInt32(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Vector2I:
                appearance.getInputValueVector2i(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Vector3I:
                appearance.getInputValueVector3i(uniform, uniform.getElementCount(), values.data());
                break;
            case ramses::EEffectInputDataType_Vector4I:
                appearance.getInputValueVector4i(uniform, uniform.getElementCount(), values.data());
                break;
            default:
                assert(false);
                break;
            }
        }

        static const char* GetUniformSemanticName(ramses::EEffectUniformSemantic semantic)
        {
            switch (semantic)
            {
            case ramses::EEffectUniformSemantic_ProjectionMatrix:
                return "Projection matrix";
            case ramses::EEffectUniformSemantic_ModelMatrix:
                return "Model matrix";
            case ramses::EEffectUniformSemantic_RendererViewMatrix:
                return "[Deprecated] Renderer-side View matrix";
            case ramses::EEffectUniformSemantic_CameraViewMatrix:
                return "Camera View matrix";
            case ramses::EEffectUniformSemantic_CameraWorldPosition:
                return "Camera world position";
            case ramses::EEffectUniformSemantic_ViewMatrix:
                return "View Matrix";
            case ramses::EEffectUniformSemantic_ModelViewMatrix:
                return "Model-View matrix";
            case ramses::EEffectUniformSemantic_ModelViewMatrix33:
                return "Model-View matrix (3x3)";
            case ramses::EEffectUniformSemantic_ModelViewProjectionMatrix:
                return "Model-View-Projection matrix";
            case ramses::EEffectUniformSemantic_NormalMatrix:
                return "Normal matrix";
            case ramses::EEffectUniformSemantic_RendererScreenResolution:
                return "Renderer screen resolution";
            default:
                assert(false && "Unsupported semantic uniform");
            }

            return "invalid";
        }
    };


    SceneToText::SceneToText(const ramses::Scene& scene, const ramses::RamsesClient& client, bool printTransformations, std::string inspectNode)
        : m_scene(scene)
        , m_client(client)
        , m_printTransformations(printTransformations)
        , m_nodeToInspect(inspectNode)
    {
    }

    void SceneToText::printToStream(std::ostringstream& stream) const
    {
        stream << "Scene '" << m_scene.getName() << "' [id:" << m_scene.getSceneId().getValue() << "]\n";

        NodeSet rootNodes = collectRootNodes();

        for (auto& rootNode : rootNodes)
        {
            printSubtree(stream, *rootNode, 0);
        }

        stream << "\nRenderPass setup:\n";
        printRenderPasses(stream);
    }

    void SceneToText::printRenderPasses(std::ostringstream& stream) const
    {
        ramses::SceneObjectIterator renderPassIterator(m_scene, ramses::ERamsesObjectType_RenderPass);

        const ramses::RamsesObject* passAsObject = renderPassIterator.getNext();
        while (nullptr != passAsObject)
        {
            ramses::RenderPass const * renderPass = ramses::RamsesUtils::TryConvert<ramses::RenderPass>(*passAsObject);
            assert(nullptr != renderPass);

            stream << "RenderPass " << renderPass->getName() << "\n";

            if (nullptr != renderPass->getCamera() && renderPass->getCamera()->isOfType(ramses::ERamsesObjectType_LocalCamera))
            {
                ramses::LocalCamera const * localCamera = ramses::RamsesUtils::TryConvert<ramses::LocalCamera>(*renderPass->getCamera());

                std::string cameraType =
                    (ramses::ERamsesObjectType_PerspectiveCamera == localCamera->getType()) ?
                    "Perspective Camera:" :
                    "Orthographic Camera:";

                stream << "  " << cameraType << "\n    Planes: [" <<
                    "Near: " << localCamera->getNearPlane() <<
                    "Far: " << localCamera->getFarPlane() <<
                    "Left: " << localCamera->getLeftPlane() <<
                    " Right: " << localCamera->getRightPlane() <<
                    " Bot: " << localCamera->getBottomPlane() <<
                    " Top: " << localCamera->getTopPlane() << "]\n";
                stream << "    Viewport: ["
                    << localCamera->getViewportX() << ", "
                    << localCamera->getViewportY() << ", "
                    << localCamera->getViewportWidth() << ", "
                    << localCamera->getViewportHeight() << "]\n";

                ramses::PerspectiveCamera const * perspCamera = ramses::RamsesUtils::TryConvert<ramses::PerspectiveCamera>(*localCamera);
                if (nullptr != perspCamera)
                {
                    stream << "    FoV: " << perspCamera->getVerticalFieldOfView() << "; Asp. Ratio: " << perspCamera->getAspectRatio();
                }
            }
            stream << "\n";

            passAsObject = renderPassIterator.getNext();
        }
    }

    void SceneToText::printSubtree(std::ostringstream& stream, const ramses::Node& rootNode, uint32_t indentation) const
    {
        stream << std::string(indentation, ' ');

        const std::string rootNodePrefix = (0 == indentation) ? "Root" : "";

        if (ramses::ERamsesObjectType_Node == rootNode.getType())
        {
            stream << rootNodePrefix << "Node '" << rootNode.getName() << "'\n";
        }
        else if (ramses::ERamsesObjectType_MeshNode == rootNode.getType())
        {
            stream << rootNodePrefix << "MeshNode '" << rootNode.getName() << "'\n";
        }
        else if (rootNode.isOfType(ramses::ERamsesObjectType_Camera))
        {
            stream << rootNodePrefix << "Camera '" << rootNode.getName() << "'\n";
        }

        printTransformations(stream, rootNode, indentation);

        if (ramses::ERamsesObjectType_MeshNode == rootNode.getType() && m_nodeToInspect == rootNode.getName())
        {
            const auto& meshNode = *ramses::RamsesUtils::TryConvert<ramses::MeshNode>(rootNode);
            printUniforms(stream, meshNode, indentation);
            printGeometryAttributes(stream, meshNode, indentation);
        }

        for (uint32_t i = 0; i < rootNode.getChildCount(); ++i)
            printSubtree(stream, *rootNode.getChild(i), indentation + 1);
    }

    template <typename T>
    void SceneToText::printUniformValues(std::ostringstream& stream, const ramses::Appearance& appearance, const ramses::UniformInput& uniform) const
    {
        const size_t baseSize = UniformUtils::GetUniformBaseSize(uniform);
        std::vector<T> values(baseSize * uniform.getElementCount());
        UniformUtils::GetUniformValues(appearance, uniform, values);
        for (size_t v = 0; v < values.size(); ++v)
        {
            if (v == 0)
                stream << "[";
            else if (v % baseSize == 0 && v != values.size() - 1)
                stream << "][";
            stream << values[v];
            if (v % baseSize != baseSize - 1)
                stream << ",";
            if (v == values.size() - 1)
                stream << "]\n";
        }
    }

    void SceneToText::printGeometryAttributes(std::ostringstream& stream, const ramses::MeshNode& mesh, uint32_t indentation) const
    {
        if (nullptr != mesh.getGeometryBinding())
        {
            const ramses::GeometryBinding& geometry = *mesh.getGeometryBinding();
            const ramses::Effect& effect = geometry.getEffect();

            for (uint32_t i = 0; i < effect.getAttributeInputCount(); ++i)
            {
                ramses::AttributeInput attribute;
                effect.getAttributeInput(i, attribute);

                stream << std::string(indentation, ' ') << UniformUtils::GetUniformShortTypeName(attribute.getDataType()) << " " << attribute.getName() << ": ";

                // data field index on low level scene is indexed starting after reserved slot for indices
                const ramses_internal::DataFieldHandle dataField(attribute.impl.getInputIndex() + 0 + 1u);
                auto& res = geometry.impl.getIScene().getDataResource(geometry.impl.getAttributeDataInstance(), dataField);
                {
                    ramses::ResourceIterator resIter(m_client);

                    bool resourceFound = false;
                    const ramses::Resource* resource = resIter.getNext();
                    while (nullptr != resource)
                    {
                        if (resource->impl.getLowlevelResourceHash() == res.hash)
                        {
                            resourceFound = true;
                            stream << resource->getName() << "[Type: " << resource->getType() << "]";
                        }

                        resource = resIter.getNext();
                    }

                    if (!resourceFound)
                    {
                        stream << "NOT FOUND";
                    }

                    stream << "\n";
                }
            }
        }
    }

    void SceneToText::printUniforms(std::ostringstream& stream, const ramses::MeshNode& mesh, uint32_t indentation) const
    {
        if (nullptr != mesh.getAppearance())
        {
            const ramses::Appearance& appearance = *mesh.getAppearance();
            const ramses::Effect& effect = appearance.getEffect();

            for (uint32_t i = 0; i < effect.getUniformInputCount(); ++i)
            {
                ramses::UniformInput uniform;
                effect.getUniformInput(i, uniform);

                stream << std::string(indentation, ' ') << UniformUtils::GetUniformShortTypeName(uniform.getDataType()) << " " << uniform.getName() << ": ";

                if (uniform.getSemantics() != ramses::EEffectUniformSemantic_Invalid)
                {
                    stream << UniformUtils::GetUniformSemanticName(uniform.getSemantics()) << "\n";
                }
                else
                {
                    auto baseType = UniformUtils::GetUniformBaseType(uniform.getDataType());
                    if (baseType == ramses::EEffectInputDataType_Float)
                        printUniformValues<float>(stream, appearance, uniform);
                    else if (baseType == ramses::EEffectInputDataType_Int32)
                        printUniformValues<int32_t>(stream, appearance, uniform);
                    else if (baseType == ramses::EEffectInputDataType_TextureSampler)
                    {
                        const ramses::TextureSampler* textureSampler;
                        const_cast<ramses::Appearance&>(appearance).getInputTexture(uniform, textureSampler);
                        stream << "[" << textureSampler->getName() << "]\n";
                    }
                }
            }
        }
    }

    void SceneToText::printTransformations(std::ostringstream& stream, const ramses::Node& node, uint32_t indentation) const
    {
        if (m_printTransformations)
        {
            float sx, sy, sz;
            float tx, ty, tz;
            float rx, ry, rz;
            node.getScaling(sx, sy, sz);
            node.getTranslation(tx, ty, tz);
            node.getRotation(rx, ry, rz);
            if (sx != 1.0f || sy != 1.0f || sz != 1.0f || tx != 0.0f || ty != 0.0f || tz != 0.0f || rx != 0.0f || ry != 0.0f || rz != 0.0f)
            {
                stream << std::string(indentation, ' ');

                if (sx != 1.0f || sy != 1.0f || sz != 1.0f)
                    stream << "S[" << sx << "," << sy << "," << sz << "] ";
                if (tx != 0.0f || ty != 0.0f || tz != 0.0f)
                    stream << "T[" << tx << "," << ty << "," << tz << "] ";
                if (rx != 0.0f || ry != 0.0f || rz != 0.0f)
                    stream << "R[" << rx << "," << ry << "," << rz << "] ";

                stream << "\n";
            }
        }
    }

    NodeSet SceneToText::collectRootNodes() const
    {
        NodeSet rootNodes;
        ramses::SceneObjectIterator nodeIterator(m_scene, ramses::ERamsesObjectType_Node);

        const ramses::RamsesObject* nodeAsObject = nodeIterator.getNext();
        while (nullptr != nodeAsObject)
        {
            ramses::Node const * node = ramses::RamsesUtils::TryConvert<ramses::Node>(*nodeAsObject);
            assert(nullptr != node);
            if (nullptr == node->getParent())
                rootNodes.insert(node);

            nodeAsObject = nodeIterator.getNext();
        }

        return rootNodes;
    }
}

