#include "common_inc.h"
#include "configurations.h"
#include <string.h>
#include <stdlib.h>

extern Motor motor;

void OnUartCmd(uint8_t* _data, uint16_t _len)
{
    float cur, pos, vel, time;
    char *token = NULL;
    switch (_data[0])
    {
        case 'c':
            token = strtok((char*) _data, "c");
            cur = atof(token);
            if (motor.controller->modeRunning != Motor::MODE_COMMAND_CURRENT)
                motor.controller->SetCtrlMode(Motor::MODE_COMMAND_CURRENT);
            motor.controller->SetCurrentSetPoint((int32_t) (cur * 1000));
            break;
        case 'v':
            token = strtok((char*) _data, "v");
            vel = atof(token);
            if (motor.controller->modeRunning != Motor::MODE_COMMAND_VELOCITY)
            {
                motor.config.motionParams.ratedVelocity = boardConfig.velocityLimit;
                motor.controller->SetCtrlMode(Motor::MODE_COMMAND_VELOCITY);
            }
            motor.controller->SetVelocitySetPoint(
                (int32_t) (vel * (float) motor.MOTOR_ONE_CIRCLE_SUBDIVIDE_STEPS));
            break;
        case 'p':
            token = strtok((char*) _data, "p");
            pos = atof(token);
            if (motor.controller->modeRunning != Motor::MODE_COMMAND_POSITION)
                motor.controller->requestMode = Motor::MODE_COMMAND_POSITION;

            motor.controller->SetPositionSetPoint(
                (int32_t) (pos * (float) motor.MOTOR_ONE_CIRCLE_SUBDIVIDE_STEPS));
            break;
        default:
            printf("Only support [c] [v] [p] commands!\r\n");
            break;
    }
}

