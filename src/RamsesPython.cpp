//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG, Daniel Werner Lima Souza de Almeida
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/RamsesPython.h"
#include "ramses-python/Resource.h"
#include "ramses-python/SceneGraphIterator.h"
#include "ramses-python/TypeConversions.h"
#include "ramses-client-api/SceneGraphIterator.h"
#include "ramses-client-api/ResourceFileDescriptionSet.h"
#include "ramses-client-api/ResourceFileDescription.h"

// Needed for stl <-> python conversions - don't remove!
#include "pybind11/stl.h"

namespace RamsesPython
{
    Ramses::Ramses(std::string name)
        : m_framework(new ramses::RamsesFramework(GetStaticConfig()))
        , m_client(m_framework->createClient(name.c_str()))
    {
    };

    Scene Ramses::createScene(std::string sceneName)
    {
        static uint64_t sceneIdCounter = 1;

        ramses::sceneId_t sceneId(sceneIdCounter++);
        m_scenes[sceneId] = m_client->createScene(sceneId, ramses::SceneConfig(), sceneName.c_str());

        return Scene(m_scenes[sceneId], m_client);
    }

    Scene Ramses::loadSceneFromFile(std::string sceneFile, std::string resourceFile)
    {
        ramses::ResourceFileDescriptionSet resourceFileInformation;
        resourceFileInformation.add(ramses::ResourceFileDescription(resourceFile.c_str()));
        ramses::Scene* scene = m_client->loadSceneFromFile(sceneFile.c_str(), resourceFileInformation);

        if (scene == nullptr)
        {
            throw std::runtime_error("Could not load scene! Check ramses errors above for details!");
        }

        ramses::sceneId_t sceneId = scene->getSceneId();
        m_scenes[sceneId] = scene;

        return Scene(scene, m_client);
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
        .def("createScene", &Ramses::createScene)
        .def("loadSceneFromFile", &Ramses::loadSceneFromFile);

    class_<RamsesObject>(m, "RamsesObject")
        .def("getName", &RamsesObject::getName)
        .def("isOfType", &RamsesObject::isOfType)
        .def("getType", &RamsesObject::getType)
        ;

    class_<Node, RamsesObject>(m, "Node")
        .def(init<ramses::Node*>())
        .def("addChild", &Node::addChild)
        .def("setParent", &Node::setParent)
        .def("setRotation", &Node::setRotation)
        .def("setTranslation", &Node::setTranslation)
        .def("setScaling", &Node::setScaling)
        .def("setVisibility", &Node::addChild)
        .def("getParent", &Node::getParent)
        .def("getRotation", &Node::getRotation)
        .def("getTranslation", &Node::getTranslation)
        .def("getScaling", &Node::getScaling)
        .def("getVisibility", &Node::getVisibility)
        .def("getModelMatrix", &Node::getModelMatrix)
    ;

    class_<Mesh, Node>(m, "Mesh")
        .def(init<ramses::MeshNode*>())
        .def("setAppearance", &Mesh::setAppearance)
        .def("getAppearance", &Mesh::getAppearance)
        .def("setGeometry", &Mesh::setGeometry)
        .def("getGeometry", &Mesh::getGeometry)
    ;

    class_<RenderGroup, RamsesObject>(m, "RenderGroup")
        .def(init<ramses::RenderGroup*>())
        .def("addMesh", &RenderGroup::addMesh)
        .def("removeMesh", &RenderGroup::removeMesh)
        .def("addRenderGroup", &RenderGroup::addRenderGroup)
        .def("removeRenderGroup", &RenderGroup::removeRenderGroup)
    ;

    class_<RenderPass, RamsesObject>(m, "RenderPass")
        .def(init<ramses::RenderPass*>())
        .def("addRenderGroup", &RenderPass::addRenderGroup)
        .def("removeRenderGroup", &RenderPass::removeRenderGroup)
        .def("setCamera", &RenderPass::setCamera)
    ;

    class_<Resource, RamsesObject>(m, "Resource")
        .def(init<ramses::Resource*>())
    ;

    class_<TextureSampler, RamsesObject>(m, "TextureSampler")
        .def(init<ramses::TextureSampler*>())
    ;

    class_<Appearance, RamsesObject>(m, "Appearance")
        .def(init<ramses::Appearance*>())
        .def("setUniformFloat", &Appearance::setUniformFloat)
        .def("setUniformInt", &Appearance::setUniformInt)
        .def("setTexture", &Appearance::setTexture)
    ;

    class_<Geometry, RamsesObject>(m, "Geometry")
        .def(init<ramses::GeometryBinding*>())
        .def("setIndexBuffer", &Geometry::setIndexBuffer)
        .def("setVertexBuffer", &Geometry::setVertexBuffer)
    ;

    class_<Camera, Node>(m, "Camera")
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

    class_<Scene, RamsesObject>(m, "Scene")
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
        .def("findObjectByName", &Scene::findObjectByName)
        .def("toText", &Scene::toText)
        .def("toText", &Scene::toTextWithFilter)
        .def("destroy", &Scene::destroy)
    ;

    enum_<ramses::ERamsesObjectType>(m, "ERamsesObjectType", arithmetic(), "Ramses object type")
        .value("ERamsesObjectType_Invalid",                     ramses::ERamsesObjectType_Invalid                 , "ERamsesObjectType_Invalid")
        .value("ERamsesObjectType_ClientObject",                ramses::ERamsesObjectType_ClientObject            , "ERamsesObjectType_ClientObject")
        .value("ERamsesObjectType_RamsesObject",                ramses::ERamsesObjectType_RamsesObject            , "ERamsesObjectType_RamsesObject")
        .value("ERamsesObjectType_SceneObject",                 ramses::ERamsesObjectType_SceneObject             , "ERamsesObjectType_SceneObject")
        .value("ERamsesObjectType_AnimationObject",             ramses::ERamsesObjectType_AnimationObject         , "ERamsesObjectType_AnimationObject")
        .value("ERamsesObjectType_Client",                      ramses::ERamsesObjectType_Client                  , "ERamsesObjectType_Client")
        .value("ERamsesObjectType_Scene",                       ramses::ERamsesObjectType_Scene                   , "ERamsesObjectType_Scene")
        .value("ERamsesObjectType_AnimationSystem",             ramses::ERamsesObjectType_AnimationSystem         , "ERamsesObjectType_AnimationSystem")
        .value("ERamsesObjectType_AnimationSystemRealTime",     ramses::ERamsesObjectType_AnimationSystemRealTime , "ERamsesObjectType_AnimationSystemRealT")
        .value("ERamsesObjectType_Node",                        ramses::ERamsesObjectType_Node                    , "ERamsesObjectType_Node")
        .value("ERamsesObjectType_MeshNode",                    ramses::ERamsesObjectType_MeshNode                , "ERamsesObjectType_MeshNode")
        .value("ERamsesObjectType_Camera",                      ramses::ERamsesObjectType_Camera                  , "ERamsesObjectType_Camera")
        .value("ERamsesObjectType_RemoteCamera",                ramses::ERamsesObjectType_RemoteCamera            , "ERamsesObjectType_RemoteCamera")
        .value("ERamsesObjectType_LocalCamera",                 ramses::ERamsesObjectType_LocalCamera             , "ERamsesObjectType_LocalCamera")
        .value("ERamsesObjectType_PerspectiveCamera",           ramses::ERamsesObjectType_PerspectiveCamera       , "ERamsesObjectType_PerspectiveCamera")
        .value("ERamsesObjectType_OrthographicCamera",          ramses::ERamsesObjectType_OrthographicCamera      , "ERamsesObjectType_OrthographicCamera")
        .value("ERamsesObjectType_Effect",                      ramses::ERamsesObjectType_Effect                  , "ERamsesObjectType_Effect")
        .value("ERamsesObjectType_AnimatedProperty",            ramses::ERamsesObjectType_AnimatedProperty        , "ERamsesObjectType_AnimatedProperty")
        .value("ERamsesObjectType_Animation",                   ramses::ERamsesObjectType_Animation               , "ERamsesObjectType_Animation")
        .value("ERamsesObjectType_AnimationSequence",           ramses::ERamsesObjectType_AnimationSequence       , "ERamsesObjectType_AnimationSequence")
        .value("ERamsesObjectType_AnimatedSetter",              ramses::ERamsesObjectType_AnimatedSetter          , "ERamsesObjectType_AnimatedSetter")
        .value("ERamsesObjectType_Appearance",                  ramses::ERamsesObjectType_Appearance              , "ERamsesObjectType_Appearance")
        .value("ERamsesObjectType_GeometryBinding",             ramses::ERamsesObjectType_GeometryBinding         , "ERamsesObjectType_GeometryBinding")
        .value("ERamsesObjectType_Spline",                      ramses::ERamsesObjectType_Spline                  , "ERamsesObjectType_Spline")
        .value("ERamsesObjectType_SplineStepBool",              ramses::ERamsesObjectType_SplineStepBool          , "ERamsesObjectType_SplineStepBool")
        .value("ERamsesObjectType_SplineStepFloat",             ramses::ERamsesObjectType_SplineStepFloat         , "ERamsesObjectType_SplineStepFloat")
        .value("ERamsesObjectType_SplineStepInt32",             ramses::ERamsesObjectType_SplineStepInt32         , "ERamsesObjectType_SplineStepInt32")
        .value("ERamsesObjectType_SplineStepVector2f",          ramses::ERamsesObjectType_SplineStepVector2f      , "ERamsesObjectType_SplineStepVector2f")
        .value("ERamsesObjectType_SplineStepVector3f",			ramses::ERamsesObjectType_SplineStepVector3f      , "ERamsesObjectType_SplineStepVector3f")
        .value("ERamsesObjectType_SplineStepVector4f",			ramses::ERamsesObjectType_SplineStepVector4f      , "ERamsesObjectType_SplineStepVector4f")
        .value("ERamsesObjectType_SplineStepVector2i",			ramses::ERamsesObjectType_SplineStepVector2i      , "ERamsesObjectType_SplineStepVector2i")
        .value("ERamsesObjectType_SplineStepVector3i",			ramses::ERamsesObjectType_SplineStepVector3i      , "ERamsesObjectType_SplineStepVector3i")
        .value("ERamsesObjectType_SplineStepVector4i",			ramses::ERamsesObjectType_SplineStepVector4i      , "ERamsesObjectType_SplineStepVector4i")
        .value("ERamsesObjectType_SplineLinearFloat",			ramses::ERamsesObjectType_SplineLinearFloat       , "ERamsesObjectType_SplineLinearFloat")
        .value("ERamsesObjectType_SplineLinearInt32",			ramses::ERamsesObjectType_SplineLinearInt32       , "ERamsesObjectType_SplineLinearInt32")
        .value("ERamsesObjectType_SplineLinearVector2f",		ramses::ERamsesObjectType_SplineLinearVector2f    , "ERamsesObjectType_SplineLinearVector2f")
        .value("ERamsesObjectType_SplineLinearVector3f",		ramses::ERamsesObjectType_SplineLinearVector3f    , "ERamsesObjectType_SplineLinearVector3f")
        .value("ERamsesObjectType_SplineLinearVector4f",		ramses::ERamsesObjectType_SplineLinearVector4f    , "ERamsesObjectType_SplineLinearVector4f")
        .value("ERamsesObjectType_SplineLinearVector2i",		ramses::ERamsesObjectType_SplineLinearVector2i    , "ERamsesObjectType_SplineLinearVector2i")
        .value("ERamsesObjectType_SplineLinearVector3i",		ramses::ERamsesObjectType_SplineLinearVector3i    , "ERamsesObjectType_SplineLinearVector3i")
        .value("ERamsesObjectType_SplineLinearVector4i",		ramses::ERamsesObjectType_SplineLinearVector4i    , "ERamsesObjectType_SplineLinearVector4i")
        .value("ERamsesObjectType_SplineBezierFloat",			ramses::ERamsesObjectType_SplineBezierFloat       , "ERamsesObjectType_SplineBezierFloat")
        .value("ERamsesObjectType_SplineBezierInt32",			ramses::ERamsesObjectType_SplineBezierInt32       , "ERamsesObjectType_SplineBezierInt32")
        .value("ERamsesObjectType_SplineBezierVector2f",		ramses::ERamsesObjectType_SplineBezierVector2f    , "ERamsesObjectType_SplineBezierVector2f")
        .value("ERamsesObjectType_SplineBezierVector3f",		ramses::ERamsesObjectType_SplineBezierVector3f    , "ERamsesObjectType_SplineBezierVector3f")
        .value("ERamsesObjectType_SplineBezierVector4f",		ramses::ERamsesObjectType_SplineBezierVector4f    , "ERamsesObjectType_SplineBezierVector4f")
        .value("ERamsesObjectType_SplineBezierVector2i",		ramses::ERamsesObjectType_SplineBezierVector2i    , "ERamsesObjectType_SplineBezierVector2i")
        .value("ERamsesObjectType_SplineBezierVector3i",		ramses::ERamsesObjectType_SplineBezierVector3i    , "ERamsesObjectType_SplineBezierVector3i")
        .value("ERamsesObjectType_SplineBezierVector4i",		ramses::ERamsesObjectType_SplineBezierVector4i    , "ERamsesObjectType_SplineBezierVector4i")
        .value("ERamsesObjectType_Resource",					ramses::ERamsesObjectType_Resource                , "ERamsesObjectType_Resource")
        .value("ERamsesObjectType_Texture2D",					ramses::ERamsesObjectType_Texture2D               , "ERamsesObjectType_Texture2D")
        .value("ERamsesObjectType_Texture3D",					ramses::ERamsesObjectType_Texture3D               , "ERamsesObjectType_Texture3D")
        .value("ERamsesObjectType_TextureCube",					ramses::ERamsesObjectType_TextureCube             , "ERamsesObjectType_TextureCube")
        .value("ERamsesObjectType_UInt16Array",					ramses::ERamsesObjectType_UInt16Array             , "ERamsesObjectType_UInt16Array")
        .value("ERamsesObjectType_UInt32Array",					ramses::ERamsesObjectType_UInt32Array             , "ERamsesObjectType_UInt32Array")
        .value("ERamsesObjectType_FloatArray",                  ramses::ERamsesObjectType_FloatArray              , "ERamsesObjectType_FloatArray")
        .value("ERamsesObjectType_Vector2fArray",				ramses::ERamsesObjectType_Vector2fArray           , "ERamsesObjectType_Vector2fArray")
        .value("ERamsesObjectType_Vector2iArray",				ramses::ERamsesObjectType_Vector2iArray           , "ERamsesObjectType_Vector2iArray")
        .value("ERamsesObjectType_Vector3fArray",				ramses::ERamsesObjectType_Vector3fArray           , "ERamsesObjectType_Vector3fArray")
        .value("ERamsesObjectType_Vector3iArray",				ramses::ERamsesObjectType_Vector3iArray           , "ERamsesObjectType_Vector3iArray")
        .value("ERamsesObjectType_Vector4fArray",				ramses::ERamsesObjectType_Vector4fArray           , "ERamsesObjectType_Vector4fArray")
        .value("ERamsesObjectType_Vector4iArray",				ramses::ERamsesObjectType_Vector4iArray           , "ERamsesObjectType_Vector4iArray")
        .value("ERamsesObjectType_RenderGroup",                 ramses::ERamsesObjectType_RenderGroup             , "ERamsesObjectType_RenderGroup")
        .value("ERamsesObjectType_RenderPass",                  ramses::ERamsesObjectType_RenderPass              , "ERamsesObjectType_RenderPass")
        .value("ERamsesObjectType_BlitPass",                    ramses::ERamsesObjectType_BlitPass                , "ERamsesObjectType_BlitPass")
        .value("ERamsesObjectType_TextureSampler",              ramses::ERamsesObjectType_TextureSampler          , "ERamsesObjectType_TextureSampler")
        .value("ERamsesObjectType_RenderBuffer",				ramses::ERamsesObjectType_RenderBuffer            , "ERamsesObjectType_RenderBuffer")
        .value("ERamsesObjectType_RenderTarget",				ramses::ERamsesObjectType_RenderTarget            , "ERamsesObjectType_RenderTarget")
        .value("ERamsesObjectType_IndexDataBuffer",             ramses::ERamsesObjectType_IndexDataBuffer         , "ERamsesObjectType_IndexDataBuffer")
        .value("ERamsesObjectType_VertexDataBuffer",            ramses::ERamsesObjectType_VertexDataBuffer        , "ERamsesObjectType_VertexDataBuffer")
        .value("ERamsesObjectType_Texture2DBuffer",				ramses::ERamsesObjectType_Texture2DBuffer         , "ERamsesObjectType_Texture2DBuffer")
        .value("ERamsesObjectType_DataObject",                  ramses::ERamsesObjectType_DataObject              , "ERamsesObjectType_DataObject")
        .value("ERamsesObjectType_DataFloat",                   ramses::ERamsesObjectType_DataFloat               , "ERamsesObjectType_DataFloat")
        .value("ERamsesObjectType_DataVector2f",				ramses::ERamsesObjectType_DataVector2f            , "ERamsesObjectType_DataVector2f")
        .value("ERamsesObjectType_DataVector3f",				ramses::ERamsesObjectType_DataVector3f            , "ERamsesObjectType_DataVector3f")
        .value("ERamsesObjectType_DataVector4f",				ramses::ERamsesObjectType_DataVector4f            , "ERamsesObjectType_DataVector4f")
        .value("ERamsesObjectType_DataMatrix22f",				ramses::ERamsesObjectType_DataMatrix22f           , "ERamsesObjectType_DataMatrix22f")
        .value("ERamsesObjectType_DataMatrix33f",				ramses::ERamsesObjectType_DataMatrix33f           , "ERamsesObjectType_DataMatrix33f")
        .value("ERamsesObjectType_DataMatrix44f",				ramses::ERamsesObjectType_DataMatrix44f           , "ERamsesObjectType_DataMatrix44f")
        .value("ERamsesObjectType_DataInt32",					ramses::ERamsesObjectType_DataInt32               , "ERamsesObjectType_DataInt32")
        .value("ERamsesObjectType_DataVector2i",				ramses::ERamsesObjectType_DataVector2i            , "ERamsesObjectType_DataVector2i")
        .value("ERamsesObjectType_DataVector3i",				ramses::ERamsesObjectType_DataVector3i            , "ERamsesObjectType_DataVector3i")
        .value("ERamsesObjectType_DataVector4i",				ramses::ERamsesObjectType_DataVector4i            , "ERamsesObjectType_DataVector4i")
        .value("ERamsesObjectType_StreamTexture",               ramses::ERamsesObjectType_StreamTexture           , "ERamsesObjectType_StreamTexture")
		.export_values();

    class_<SceneGraphIterator>(m, "SceneGraphIterator")
        .def(init<RamsesPython::Node&, ramses::ETreeTraversalStyle, ramses::ERamsesObjectType>())
        .def("getNext", &SceneGraphIterator::getNext)
        ;

    enum_<ramses::ETreeTraversalStyle>(m, "ETreeTraversalStyle")
        .value("ETreeTraversalStyle_DepthFirst", ramses::ETreeTraversalStyle_DepthFirst)
        .value("ETreeTraversalStyle_BreadthFirst", ramses::ETreeTraversalStyle_BreadthFirst)
        ;

    // TODO find out if there is a more pythonic way to deal with types and casts
    m.def("toMesh", &RamsesPython::TypeConversions::ToMesh);
    m.def("toCamera", &RamsesPython::TypeConversions::ToCamera);
    m.def("toPerspectiveCamera", &RamsesPython::TypeConversions::ToPerspectiveCamera);
    m.def("toOrthographicCamera", &RamsesPython::TypeConversions::ToOrthographicCamera);
    m.def("toNode", &RamsesPython::TypeConversions::ToNode);
}
