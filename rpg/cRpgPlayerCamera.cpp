/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cRpgPlayerCamera.h"

#include <cCamera.h>
#include <jleInput.h>

#include <glm/gtx/quaternion.hpp>
#include <utility>

cRpgPlayerCamera::
cRpgPlayerCamera(jleObject *owner, jleScene *scene)
{
}

void
cRpgPlayerCamera::update(float dt)
{
    processInput(dt);
    rotateCameraTowardsTarget();
}

void
cRpgPlayerCamera::setTarget(std::weak_ptr<jleObject> target)
{
    _target = std::move(target);
}

void
cRpgPlayerCamera::start()
{
    const auto cameraChild = object()->spawnChildObject<jleObject>();
    _cameraComponent = cameraChild->addComponent<cCamera>();
}

void
cRpgPlayerCamera::processInput(float dt)
{
    if (gEngine->input().mouse->mouseClick(jleButton::BUTTON_2)) {
        _yaw += gEngine->input().mouse->xDelta() * dt * 55.f;

        _yaw = glm::mod(_yaw, 360.0f);
        if (_yaw >= 180.0f) {
            _yaw -= 360.0f;
        }

        _pitch -= gEngine->input().mouse->yDelta() * dt * 55.f;
        _pitch = glm::clamp(_pitch, -89.0f, -8.0f);
    } else if (gEngine->input().mouse->mouseClick(jleButton::BUTTON_3)) {

        glm::vec3 forward_xz;
        forward_xz.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
        forward_xz.y = 0;
        forward_xz.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));

        constexpr glm::vec3 up = {0.f, 1.f, 0.f};
        const auto right_xz = glm::cross(up, forward_xz);

        _targetOffset += forward_xz * gEngine->input().mouse->yDelta() * dt * 55.f;
        _targetOffset += right_xz * gEngine->input().mouse->xDelta() * dt * 55.f;
    }

    if (gEngine->input().keyboard->keyDown(jleKey::SPACE)) {
        _targetOffset = {};
    }

    if (auto scroll = gEngine->input().mouse->scrollY()) {
        _radialDistance -= scroll;
        _radialDistance = std::clamp(_radialDistance, 1.f, 25.f);
    }

    glm::vec3 forward;
    forward.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
    forward.y = glm::sin(glm::radians(_pitch));
    forward.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
    forward = glm::normalize(forward) * _radialDistance;

    glm::mat4 mat{1.f};
    mat = glm::translate(mat, forward);
    mat = glm::translate(mat, getTargetPosition());

    getTransform().setLocalMatrix(mat);
}

void
cRpgPlayerCamera::rotateCameraTowardsTarget()
{
    const auto target = getTargetPosition();

    if (const auto camera = _cameraComponent.lock()) {
        const auto pointToLookAt = target;
        const auto cameraPosition = getTransform().getLocalPosition();
        constexpr auto upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        const auto forward = glm::normalize(pointToLookAt - cameraPosition);
        const auto rotation = glm::quatLookAt(forward, upVector);
        const auto rotationMatrix = glm::toMat4(rotation);

        camera->object()->getTransform().setLocalMatrix(rotationMatrix);
    }
}

glm::vec3
cRpgPlayerCamera::getTargetPosition() const
{
    if (auto target = _target.lock()) {
        return target->getTransform().getWorldPosition() + _targetOffset;
    }
    return _targetOffset;
}