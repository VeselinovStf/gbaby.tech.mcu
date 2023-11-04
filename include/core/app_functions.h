#ifndef APP_FUNCTIONS_STATE_H
#define APP_FUNCTIONS_STATE_H

#include <Arduino.h>

// TODO: #79 Remove this variable
const uint8_t TOTAL_CORE_FUNCTIONS = 6;
const uint8_t TOTAL_LIMITS = 3;
const uint8_t TOTAL_NOTIFICATIONS = 1;

enum ActionFunctionIDX
{
    LIGHT_SOURCE = 0,
    //WATER_CYCLE = 1,
    INPUT_PUMP = 1,
    OUTPUT_PUMP = 2,
    INTAKE_FAN = 3,
    EXHAUS_FAN = 4,
    OXYGEN_PUMP = 5,
    //DRAIN_WATER = 7
};

typedef struct
{
    String Name;
    ActionFunctionIDX Id;
} function_t;

class AppFunctions
{
public:
    static const function_t function_lightSource;
    //static const function_t function_waterCycle;
    static const function_t function_inputPump;
    static const function_t function_outputPump;
    static const function_t function_fanIntake;
    static const function_t function_fanExhaus;
    static const function_t function_oxygenPump;
    //static const function_t function_drainWater;
    static const uint8_t TOTAL_CORE_FUNCTIONS;
};
#endif