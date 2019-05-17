//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/RamsesPython.h"
#include "ramses-python/Resource.h"

// Needed for stl <-> python conversions - don't remove!
#include "pybind11/stl.h"

namespace RamsesPython
{
    Ramses::Ramses(std::string name)
        : m_framework(new ramses::RamsesFramework(GetStaticConfig()))
        , m_client(new ramses::RamsesClient(name.c_str(), *m_framework))
    {
    };

    Scene Ramses::createScene(std::string sceneName)
    {
        static ramses::sceneId_t sceneIdCounter = 0;

        ramses::sceneId_t sceneId = sceneIdCounter++;
        m_scenes[sceneId] = m_client->createScene(sceneId, ramses::SceneConfig(), sceneName.c_str());

        return Scene(m_scenes[sceneId], m_client.get());
    }

    ramses::RamsesFrameworkConfig& Ramses::GetStaticConfig()
    {
        static int argc = 1;
        static char const* const argv[] = { "RamsesPython" };
        static ramses::RamsesFrameworkConfig config(argc, argv);
        config.setPeriodicLogsEnabled(false);
        return config;
    }
}

using namespace RamsesPython;
using namespace pybind11;

PYBIND11_MODULE(RamsesPython, m)
{
    // TODO extend documentation
    m.doc() = R"pbdoc(
        Python bindings for the RAMSES C++ API
        -----------------------
        .. currentmodule:: RamsesPython
    )pbdoc";

    class_<Ramses>(m, "Ramses")
        .def(init< std::string >())
        .def("createScene", &Ramses::createScene);

    class_<Node>(m, "Node")
        .def(init<ramses::Node*>())
        .def("addChild", &Node::addChild)
        .def("setParent", &Node::setParent)
        .def("setRotation", &Node::setRotation)
        .def("setTranslation", &Node::setTranslation)
        .def("setScaling", &Node::setScaling)
        .def("setVisibility", &Node::addChild)
        .def("addChild", &Node::addChild)
    ;

    class_<Mesh, Node>(m, "Mesh")
        .def(init<ramses::MeshNode*>())
        .def("setAppearance", &Mesh::setAppearance)
        .def("setGeometry", &Mesh::setGeometry)
    ;

    class_<RenderGroup>(m, "RenderGroup")
        .def(init<ramses::RenderGroup*>())
        .def("addMesh", &RenderGroup::addMesh)
        .def("removeMesh", &RenderGroup::removeMesh)
        .def("addRenderGroup", &RenderGroup::addRenderGroup)
        .def("removeRenderGroup", &RenderGroup::removeRenderGroup)
    ;

    class_<RenderPass>(m, "RenderPass")
        .def(init<ramses::RenderPass*>())
        .def("addRenderGroup", &RenderPass::addRenderGroup)
        .def("removeRenderGroup", &RenderPass::removeRenderGroup)
        .def("setCamera", &RenderPass::setCamera)
    ;

    class_<RamsesPython::Resource>(m, "Resource")
        .def(init<ramses::Resource*>())
    ;

    class_<TextureSampler>(m, "TextureSampler")
        .def(init<ramses::TextureSampler*>())
    ;

    class_<Appearance>(m, "Appearance")
        .def(init<ramses::Appearance*>())
        .def("setUniformFloat", &Appearance::setUniformFloat)
        .def("setUniformInt", &Appearance::setUniformInt)
        .def("setTexture", &Appearance::setTexture)
    ;

    class_<Geometry>(m, "Geometry")
        .def(init<ramses::GeometryBinding*>())
        .def("setIndexBuffer", &Geometry::setIndexBuffer)
        .def("setVertexBuffer", &Geometry::setVertexBuffer)
    ;

    class_<Camera>(m, "Camera")
        .def(init<ramses::LocalCamera*>())
        .def("setFrustum", &PerspectiveCamera::setFrustum)
        .def("setViewport", &PerspectiveCamera::setViewport)
    ;

    class_<PerspectiveCamera, Camera>(m, "PerspectiveCamera")
        .def(init<ramses::PerspectiveCamera*>())
        .def("setFrustumFromFoV", &PerspectiveCamera::setFrustumFromFoV)
    ;

    class_<OrthographicCamera, Camera>(m, "OrthographicCamera")
        .def(init<ramses::OrthographicCamera*>())
    ;

    class_<Scene>(m, "Scene")
        .def(init<ramses::Scene*, ramses::RamsesClient*>())
        .def("createEffect", &Scene::createEffect)
        .def("createIndexArray", &Scene::createIndexArray)
        .def("createVertexArray", &Scene::createVertexArray)
        .def("createTexture", &Scene::createTexture)
        .def("createTextureSampler", &Scene::createTextureSampler)
        .def("createNode", &Scene::createNode)
        .def("createMesh", &Scene::createMesh)
        .def("createRenderGroup", &Scene::createRenderGroup)
        .def("createRenderPass", &Scene::createRenderPass)
        .def("createPerspectiveCamera", &Scene::createPerspectiveCamera)
        .def("createOrthographicCamera", &Scene::createOrthographicCamera)
        .def("createAppearance", &Scene::createAppearance)
        .def("createGeometry", &Scene::createGeometry)
        .def("saveToFiles", &Scene::saveToFiles)
        .def("getValidationReport", &Scene::getValidationReport)
    ;
}

