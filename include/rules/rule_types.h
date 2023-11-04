#ifndef RULE_TYPES_VALIDATOR_H
#define RULE_TYPES_VALIDATOR_H

#include <Arduino.h>

enum RuleType : uint8_t
{
    TEMPERATURE_RULE = 1,
    HUMIDITY_RULE = 2,
    TANK_LEVEL_RULE = 3,
    TIME_RULE = 4
};

enum RuleConditionType : uint8_t
{
    GreatherThen = 1,
    LessThen = 2,
    EqualCondition = 3
};

#endif