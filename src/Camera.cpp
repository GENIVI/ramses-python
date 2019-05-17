//  -------------------------------------------------------------------------
//  Copyright (C) 2019 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "ramses-python/Camera.h"
#include <assert.h>
#include <iostream>

namespace RamsesPython
{
    Camera::Camera(ramses::LocalCamera* camera)
        : m_camera(camera)
    {
    }

    void Camera::setFrustum(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearPlane, float farPlane)
    {
        ramses::status_t success = m_camera->setFrustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
        assert(ramses::StatusOK == success);
    }

    void Camera::setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height)
    {
        ramses::status_t success = m_camera->setViewport(x, y, width, height);
        assert(ramses::StatusOK == success);
    }

    PerspectiveCamera::PerspectiveCamera(ramses::PerspectiveCamera* camera)
        : Camera(camera)
        , m_camera(camera)
    {
    }

    void PerspectiveCamera::setFrustumFromFoV(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        ramses::status_t success = m_camera->setFrustum(fov, aspectRatio, nearPlane, farPlane);
        assert(ramses::StatusOK == success);
    }

    OrthographicCamera::OrthographicCamera(ramses::OrthographicCamera* camera)
        : Camera(camera)
        , m_camera(camera)
    {
    }
}
