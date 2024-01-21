/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cPIDControlPad.h"
#include <cMesh.h>
#include <cRigidbody.h>

#include "jleInput.h"
#include "jleKeyboardInput.h"

void
cPIDControlPad::update(float dt)
{


    if(pidTuneState == PIDTuneState::Production)
    {
        auto xyz = getTransform().getWorldPosition();

        auto outputX = pidX.calculate(target.x, xyz.x);
        auto outputY = pidY.calculate(target.y, xyz.z);

        auto rigidbody = object()->getComponent<cRigidbody>();
        if (rigidbody) {
            rigidbody->getBody().applyForce(btVector3{outputX, 0.f, outputY}, btVector3{0.f, 0.f, 0.f});
        }
    }

    if(pidTuneState != PIDTuneState::Production)
    {
        autoTune(dt);
    }


    glm::mat4 trans{1.f};
    trans = glm::translate(trans, glm::vec3{target.x, 0.f, target.y});
    trans = glm::scale(trans, glm::vec3{0.2f});

    auto mesh = object()->getComponent<cMesh>()->getMesh();
    std::shared_ptr<jleMaterial> mat = object()->getComponent<cMesh>()->getMaterial();
    gEngine->renderGraph().sendMesh(mesh, mat, trans, object()->instanceID(), false);
}

void
cPIDControlPad::editorInspectorImGuiRender()
{
    if (ImGui::Button("Reset Pos")) {
        static constexpr auto defaultMat = glm::mat4{1.f};
        getTransform().setLocalMatrix(defaultMat);
        auto rigidbody = object()->getComponent<cRigidbody>();
        if (rigidbody) {
            rigidbody->getBody().setLinearVelocity(btVector3{0.f, 0.f, 0.f});
            rigidbody->getBody().setAngularFactor(btVector3{0.f, 0.f, 0.f});

            btTransform transform;
            transform.setFromOpenGLMatrix((btScalar *)&defaultMat);
            rigidbody->getBody().setWorldTransform(transform);

            pidX.reset();
            pidY.reset();
        }
    }

    if (ImGui::Button("Auto Tune")) {
        pidTuneState = PIDTuneState::WaitForOscillation;
        pidTuneInternals = {};
        pidX.ki = 0.f;
        pidX.kd = 0.f;
    }
}

void
cPIDControlPad::autoTune(float dt)
{
    switch (pidTuneState) {
    case PIDTuneState::WaitForOscillation:

        pidTuneInternals.time += dt;
        if(pidTuneInternals.time > 10.f)
        {

        }

        break;
    case PIDTuneState::Production:
        break;
    }
}
