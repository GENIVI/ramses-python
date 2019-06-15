//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_SCENETOTEXT_H
#define PYTHONRAMSES_SCENETOTEXT_H

#include <string>
#include <sstream>
#include <unordered_set>

namespace ramses
{
    class Scene;
    class RamsesClient;
    class Node;
    class Appearance;
    class UniformInput;
    class MeshNode;
    class Appearance;
}

namespace RamsesPython
{
    using NodeSet = std::unordered_set<ramses::Node const*>;

    class SceneToText
    {
    public:
        SceneToText(const ramses::Scene& scene, const ramses::RamsesClient& client, bool printTransformations, std::string inspectNode);

        void printToStream(std::ostringstream& stream) const;

    private:
        void printRenderPasses(std::ostringstream& stream) const;
        void printSubtree(std::ostringstream& stream, const ramses::Node& rootNode, uint32_t indentation) const;
        template <typename T>
        void printUniformValues(std::ostringstream& stream, const ramses::Appearance& appearance, const ramses::UniformInput& uniform) const;
        void printGeometryAttributes(std::ostringstream& stream, const ramses::MeshNode& mesh, uint32_t indentation) const;
        void printUniforms(std::ostringstream& stream, const ramses::MeshNode& mesh, uint32_t indentation) const;
        void printTransformations(std::ostringstream& stream, const ramses::Node& node, uint32_t indentation) const;
        NodeSet collectRootNodes() const;

        const ramses::Scene& m_scene;
        const ramses::RamsesClient& m_client;
        bool m_printTransformations;
        std::string m_nodeToInspect;
    };
}

#endif
