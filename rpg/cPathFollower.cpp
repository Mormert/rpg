/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cPathFollower.h"

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btVector3.h>

#include <jle3DGraph.h>
#include <jlePhysics.h>

void
cPathFollower::start()
{
}

void
cPathFollower::update(float dt)
{
    moveTowardsPosition(dt);

    gEngine->renderGraph().sendLine(_moveToPosition + glm::vec3{0.f, 0.5f, 0.f},
                                _moveToPosition - glm::vec3{0.f, 0.5f, 0.f});
}

void
cPathFollower::serverUpdate(float dt)
{
    moveTowardsPosition(dt);
}

glm::vec3
cPathFollower::getCurrentPosition() const
{
    return glm::mix(_moveFromPosition, _moveToPosition, _interpolationAlpha);
}

glm::vec3
cPathFollower::getMoveToPosition() const
{
    return _moveToPosition;
}

void
cPathFollower::moveTowardsPosition(float dt)
{
    const auto interpLength = glm::length(_moveFromPosition - _moveToPosition);

    _interpolationAlpha += 1.f / interpLength * dt * 10.f;
    _interpolationAlpha = glm::clamp(_interpolationAlpha, 0.f, 1.f);

    const auto interpPos = getCurrentPosition();

    getTransform().setWorldPosition(interpPos);

    btVector3 rayFrom = {interpPos.x, interpPos.y + 10.f, interpPos.z};
    btVector3 rayTo = {interpPos.x, interpPos.y - 10.f, interpPos.z};

    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

    scene()->getPhysics().dynamicsWorld().rayTest(rayFrom, rayTo, rayCallback);

    if (rayCallback.hasHit()) {
        glm::vec3 hitPos{
            rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z()};
        hitPos.y -= 1.5f;
        getTransform().setWorldPosition(hitPos);
    }
}

void
rpgPathFollowEventServerToClient::execute()
{
    if (auto object = getSceneClient().getObjectFromNetId(objectNetId)) {
        if (auto pathFollower = object->getComponent<cPathFollower>()) {
            pathFollower->_moveFromPosition = moveFromPosition;
            pathFollower->_moveToPosition = moveToPosition;
            pathFollower->_interpolationAlpha = 0.f;
        }
    }
}