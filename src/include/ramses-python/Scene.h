//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG, Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_SCENE_H
#define PYTHONRAMSES_SCENE_H

#include <vector>
#include <string>

#include "ramses-client-api/Scene.h"
#include "ramses-client-api/RamsesClient.h"
#include "ramses-client-api/Resource.h"
#include "ramses-client-api/RamsesObject.h"

#include "ramses-python/Resource.h"
#include "ramses-python/TextureSampler.h"
#include "ramses-python/Appearance.h"
#include "ramses-python/Geometry.h"
#include "ramses-python/Node.h"
#include "ramses-python/Mesh.h"
#include "ramses-python/RenderGroup.h"
#include "ramses-python/RenderPass.h"
#include "ramses-python/Camera.h"
#include "ramses-python/RamsesObject.h"

namespace boost
{
    namespace python
    {
        class list;
    }
}

namespace RamsesPython
{
    class Scene
    {
    public:
        Scene(ramses::Scene* scene, ramses::RamsesClient* client);

        Resource createEffect(std::string vertexShader, std::string fragmentShader);

        // TODO also allow creation from memory
        Resource createTexture(std::string file);

        // TODO implement sampling states
        TextureSampler createTextureSampler(Resource texture);

        // TODO also allow creation from memory
        Resource createVertexArray(uint32_t components, const std::vector<float>& vertexData);

        // TODO also allow creation from memory
        Resource createIndexArray(const std::vector<uint32_t>& indexData);

        Appearance createAppearance(Resource effect);
        Geometry createGeometry(Resource effect);
        Node createNode(std::string name);
        Mesh createMesh(std::string name);

        RenderGroup createRenderGroup(std::string name);

        RenderPass createRenderPass(std::string name);

        PerspectiveCamera createPerspectiveCamera(std::string name);

        OrthographicCamera createOrthographicCamera(std::string name);

        void saveToFiles(std::string sceneFile, std::string resourcesFile, bool compress);
        std::string getValidationReport() const;

        RamsesObject findObjectByName(const char* name);

    private:
        ramses::Scene* m_scene;
        ramses::RamsesClient* m_client;
        std::vector<const ramses::Resource*> m_resources;
    };
}

#endif
