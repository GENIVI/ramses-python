//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG, Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------


#include <string>
#include <exception>

#include "ramses-python/Scene.h"
#include "ramses-python/Resource.h"
#include "ramses-python/SceneToText.h"

#include "ramses-client.h"
#include "ramses-utils.h"
#include "assert.h"

namespace RamsesPython
{
    Scene::Scene(ramses::Scene* scene, ramses::RamsesClient* client)
        : RamsesObject(scene)
        , m_scene(scene)
        , m_client(client)
    {
    }

    Resource Scene::createEffect(std::string vertexShader, std::string fragmentShader)
    {
        ramses::EffectDescription effectDesc;

        // TODO Should come from outside
        effectDesc.setUniformSemantic("u_ViewMatrix", ramses::EEffectUniformSemantic_ViewMatrix);
        effectDesc.setUniformSemantic("u_ModelMatrix", ramses::EEffectUniformSemantic_ModelMatrix);
        effectDesc.setUniformSemantic("u_ProjectionMatrix", ramses::EEffectUniformSemantic_ProjectionMatrix);
        effectDesc.setUniformSemantic("u_MVPMatrix", ramses::EEffectUniformSemantic_ModelViewProjectionMatrix);
        effectDesc.setUniformSemantic("u_CameraWorldPosition", ramses::EEffectUniformSemantic_CameraWorldPosition);

        effectDesc.setVertexShader(vertexShader.c_str());
        effectDesc.setFragmentShader(fragmentShader.c_str());

        auto* effectResource = m_client->createEffect(effectDesc);
        m_resources.push_back(effectResource);
        return Resource(effectResource);
    }

    // TODO also allow creation from memory
    Resource Scene::createTexture(std::string file)
    {
        ramses::Texture2D* texture = ramses::RamsesUtils::CreateTextureResourceFromPng(file.c_str(), *m_client);
        assert(nullptr != texture);
        m_resources.push_back(texture);
        return Resource(texture);
    }

    // TODO implement sampling states
    TextureSampler Scene::createTextureSampler(Resource texture)
    {
        // TODO error handling
        assert(texture.m_resource->getType() == ramses::ERamsesObjectType_Texture2D);

        ramses::TextureSampler* textureSampler = m_scene->createTextureSampler(
            ramses::ETextureAddressMode_Repeat,
            ramses::ETextureAddressMode_Repeat,
            ramses::ETextureSamplingMethod_Linear_MipMapLinear,
            ramses::ETextureSamplingMethod_Linear,
            *ramses::RamsesUtils::TryConvert<ramses::Texture2D>(*texture.m_resource));

        return TextureSampler(textureSampler);
    }

    // TODO also allow creation from memory
    Resource Scene::createVertexArray(uint32_t components, const std::vector<float>& vertexData)
    {
        assert(0 != components && vertexData.size() % components == 0);

        const ramses::Resource* vertexResource = nullptr;
        switch (components)
        {
        case 1:
            vertexResource = m_client->createConstFloatArray(static_cast<uint32_t>(vertexData.size()), (vertexData).data());
            break;
        case 2:
            vertexResource = m_client->createConstVector2fArray(static_cast<uint32_t>(vertexData.size()) / components, (vertexData).data());
            break;
        case 3:
            vertexResource = m_client->createConstVector3fArray(static_cast<uint32_t>(vertexData.size()) / components, (vertexData).data());
            break;
        case 4:
            vertexResource = m_client->createConstVector4fArray(static_cast<uint32_t>(vertexData.size()) / components, (vertexData).data());
            break;
        default:
            std::string msg {"Unsupported vertex array format"};
            throw std::runtime_error {msg};
        }

        m_resources.push_back(vertexResource);
        return Resource(vertexResource);
    }

    // TODO also allow creation from memory
    Resource Scene::createIndexArray(const std::vector<uint32_t>& indexData)
    {
        assert(indexData.size() != 0);
        auto* ramsesResource = m_client->createConstUInt32Array(static_cast<uint32_t>(indexData.size()), (indexData).data());
        m_resources.push_back(ramsesResource);
        return Resource(ramsesResource);
    }

    Appearance Scene::createAppearance(Resource effect)
    {
        assert(effect.m_resource->getType() == ramses::ERamsesObjectType_Effect);
        return Appearance(m_scene->createAppearance(*ramses::RamsesUtils::TryConvert<ramses::Effect>(*effect.m_resource)));
    }

    Geometry Scene::createGeometry(Resource effect)
    {
        assert(effect.m_resource->getType() == ramses::ERamsesObjectType_Effect);
        return Geometry(m_scene->createGeometryBinding(*ramses::RamsesUtils::TryConvert<ramses::Effect>(*effect.m_resource)));
    }

    Node Scene::createNode(std::string name)
    {
        return Node(m_scene->createNode(name.c_str()));
    }

    Mesh Scene::createMesh(std::string name)
    {
        return Mesh(m_scene->createMeshNode(name.c_str()));
    }

    RenderGroup Scene::createRenderGroup(std::string name)
    {
        return RenderGroup(m_scene->createRenderGroup(name.c_str()));
    }

    RenderPass Scene::createRenderPass(std::string name)
    {
        ramses::RenderPass* renderPass = m_scene->createRenderPass(name.c_str());
        renderPass->setClearFlags(ramses::EClearFlags_None);
        return RenderPass(renderPass);
    }

    PerspectiveCamera Scene::createPerspectiveCamera(std::string name)
    {
        return PerspectiveCamera(m_scene->createPerspectiveCamera(name.c_str()));
    }

    OrthographicCamera Scene::createOrthographicCamera(std::string name)
    {
        return OrthographicCamera(m_scene->createOrthographicCamera(name.c_str()));
    }

    void Scene::saveToFiles(std::string sceneFile, std::string resourcesFile, bool compress)
    {
        // TODO fix ramses serialization API
        ramses::ResourceFileDescriptionSet resourceFileSet;
        ramses::ResourceFileDescription resourceFile(resourcesFile.c_str());

        for(const auto& resource : m_resources)
            resourceFile.add(resource);

        resourceFileSet.add(resourceFile);

        m_scene->flush();
        m_client->saveSceneToFile(*m_scene, sceneFile.c_str(), resourceFileSet, compress);
    }

    std::string Scene::getValidationReport() const
    {
        ramses::status_t validationStatus = m_scene->validate();

        if(ramses::StatusOK != validationStatus)
        {
            return std::string("Scene is not in a valid state! Report:\n") + m_scene->getValidationReport(ramses::EValidationSeverity_Info);
        }

        return "";
    }

    std::string Scene::toTextWithFilter(std::string filter)
    {
        SceneToText stt(*m_scene, *m_client, true, filter);
        std::ostringstream outputStream;
        stt.printToStream(outputStream);
        return outputStream.str();
    }

    std::string Scene::toText()
    {
        return toTextWithFilter("");
    }

    RamsesPython::RamsesObject Scene::findObjectByName(const char* name)
    {
        return RamsesPython::RamsesObject {m_scene->findObjectByName(name)};
    }
}
