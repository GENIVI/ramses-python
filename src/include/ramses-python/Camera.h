//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef PYTHONRAMSES_CAMERA_H
#define PYTHONRAMSES_CAMERA_H

#include "ramses-python/Node.h"
#include "ramses-client-api/PerspectiveCamera.h"
#include "ramses-client-api/OrthographicCamera.h"

namespace RamsesPython
{
    class Camera : public Node
    {
    public:
        Camera(ramses::LocalCamera* camera);

        void setFrustum(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearPlane, float farPlane);

        void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);

        ramses::LocalCamera* m_camera;
    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(ramses::PerspectiveCamera* camera);

        void setFrustumFromFoV(float fov, float aspectRatio, float nearPlane, float farPlane);

        ramses::PerspectiveCamera* m_camera;
    };

    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(ramses::OrthographicCamera* camera);

        ramses::OrthographicCamera* m_camera;
    };

}

#endif
