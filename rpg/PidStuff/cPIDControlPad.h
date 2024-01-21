/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include "PIDController.h"
#include "jleComponent.h"

class cPIDControlPad : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cPIDControlPad)
public:
    template <class Archive>
    void
    serialize(Archive &archive)
    {
        try{
            archive(cereal::make_nvp("pid", pidX), CEREAL_NVP(target));
            pidY.kp = pidX.kp;
            pidY.ki = pidX.ki;
            pidY.kd = pidX.kd;
        }catch(std::exception&e){
        }
    }

    void editorInspectorImGuiRender() override;

    void update(float dt) override;

    void autoTune(float dt);

protected:
    PIDController pidX{};
    PIDController pidY{};
    glm::vec2 target;

    enum class PIDTuneState
    {
        Production,
        WaitForOscillation
    } pidTuneState;


    struct PidTuneInternals
    {
        float time{};
    } pidTuneInternals;

};

CEREAL_REGISTER_TYPE(cPIDControlPad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cPIDControlPad)
