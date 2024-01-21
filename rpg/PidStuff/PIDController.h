/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#ifdef JLE_BUILD_EDITOR
#include "editor/jleImGuiCerealArchive.h"
#endif
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

class PIDController
{
public:
    PIDController(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd), prevError(0), integral(0) {}

    PIDController() = default;

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(kp), CEREAL_NVP(ki), CEREAL_NVP(kd), CEREAL_NVP(prevError), CEREAL_NVP(integral));
    }

    float
    calculate(float setpoint, float processVariable)
    {
        // Calculate the error
        float error = setpoint - processVariable;

        // Proportional term
        float P = kp * error;

        // Integral term
        integral += ki * error;

        // Derivative term
        float derivative = kd * (error - prevError);

        // PID output
        float output = P + integral + derivative;

        // Update previous error
        prevError = error;

        return output;
    }

    void reset()
    {
        prevError = {0.f};
        integral = {0.f};
    }

private:
    friend class cPIDControlPad;
    float kp{}; // Proportional gain
    float ki{}; // Integral gain
    float kd{}; // Derivative gain

    float prevError{}; // Previous error
    float integral{};  // Integral term accumulator
};
