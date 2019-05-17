#  -------------------------------------------------------------------------
#  Copyright (C) 2018 BMW AG
#  -------------------------------------------------------------------------
#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.
#  -------------------------------------------------------------------------

import RamsesPython

ramses = RamsesPython.Ramses("test")
scene = ramses.createScene("test scene")

camera = scene.createPerspectiveCamera("camera")
camera.setViewport(0, 0, 200, 200)
camera.setFrustumFromFoV(20.0, 1.0, 1.0, 1000.0)

rendergroup = scene.createRenderGroup("rendergroup")
renderpass = scene.createRenderPass("renderpass")

renderpass.addRenderGroup(rendergroup, 0)
renderpass.setCamera(camera)

rootNode = scene.createNode("root")
childNode = scene.createNode("child")
rootNode.addChild(childNode)

childNode.setTranslation(2.5, 0.5, 0.0)

vertShader = """
#version 300 es

in vec3 a_position;
in vec2 a_texcoords;
out vec2 v_texcoords;

void main()
{
    v_texcoords = a_texcoords;
    // z = -1.0, so that the geometry will not be clipped by the near plane of the camera
    gl_Position = vec4(a_position.xy, -1.0, 1.0);
}
"""
fragShader = """
#version 300 es

uniform sampler2D u_texture;
precision mediump float;
uniform float u_opacity;

in vec2 v_texcoords;
out vec4 FragColor;

void main(void)
{
    ivec2 textureSize = textureSize(u_texture, 0);
    vec2 texelFloat = vec2(v_texcoords.x * float(textureSize.x), v_texcoords.y * float(textureSize.y));
    vec4 texel = texelFetch(u_texture, ivec2(texelFloat), 0);
    FragColor = u_opacity * texel;
}

"""

effect = scene.createEffect(vertShader, fragShader)
geometry = scene.createGeometry(effect)
texture = scene.createTexture("texture.png")
textureSampler = scene.createTextureSampler(texture)

appearance = scene.createAppearance(effect)
appearance.setUniformFloat("u_opacity", [1.0])
appearance.setTexture("u_texture", textureSampler)

indices = scene.createIndexArray([0, 1, 2, 2, 1, 3])
vertices = scene.createVertexArray(3, [-1., 0., 0., 1., 0., 0., 0., 1., 0., 1., 1., 0.])
texcoords = scene.createVertexArray(2, [0., 0., 1., 0., 0., 1., 1., 1.])
geometry.setIndexBuffer(indices)
geometry.setVertexBuffer("a_position", vertices)
geometry.setVertexBuffer("a_texcoords", texcoords)

mesh = scene.createMesh("triangle")
mesh.setAppearance(appearance)
mesh.setGeometry(geometry)

rendergroup.addMesh(mesh, 0)

sceneFile = "scene.ramses"
sceneResources = "scene.ramres"
scene.saveToFiles(sceneFile, sceneResources, True)

validationReport = scene.getValidationReport()

if len(validationReport) != 0:
    print(validationReport)
else:
    print("Scene exported successfully to files {} {}".format(sceneFile, sceneResources))

