/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include <jleComponent.h>

class cCamera;

class cRpgPlayerCamera : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cRpgPlayerCamera)
public:
    void update(float dt) override;

    void setTarget(std::weak_ptr<jleObject> target);

    void start() override;

private:
    void processInput(float dt);

    void processTerrainRaycast();

    void rotateCameraTowardsTarget();

    [[nodiscard]] const glm::vec3 getTargetPosition() const;

    std::weak_ptr<jleObject> _target;
    glm::vec3 _targetOffset{};

    std::weak_ptr<cCamera> _cameraComponent;

    float _radialDistance{20.f};
    float _pitch{};
    float _yaw{};
};

CEREAL_REGISTER_TYPE(cRpgPlayerCamera)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cRpgPlayerCamera)