#include "core/app_functions.h"

// Every function is declared here
const function_t AppFunctions::function_lightSource = {"lightSource", ActionFunctionIDX::LIGHT_SOURCE};
//const function_t AppFunctions::function_waterCycle = {"waterCycle", ActionFunctionIDX::WATER_CYCLE};
const function_t AppFunctions::function_inputPump = {"inputPump", ActionFunctionIDX::INPUT_PUMP};
const function_t AppFunctions::function_outputPump = {"outputPump", ActionFunctionIDX::OUTPUT_PUMP};
const function_t AppFunctions::function_fanIntake = {"fanIntake", ActionFunctionIDX::INTAKE_FAN};
const function_t AppFunctions::function_fanExhaus = {"fanExhaus", ActionFunctionIDX::EXHAUS_FAN};
const function_t AppFunctions::function_oxygenPump = {"oxygenPump", ActionFunctionIDX::OXYGEN_PUMP};
// const function_t AppFunctions::function_drainWater = {"drainWater", ActionFunctionIDX::DRAIN_WATER};

// Holds total count of functions. CHANGE WHEN ADDING A NEW
const uint8_t AppFunctions::TOTAL_CORE_FUNCTIONS = 6;