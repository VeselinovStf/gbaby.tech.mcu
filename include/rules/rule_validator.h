#ifndef RULE_VALIDATOR_H
#define RULE_VALIDATOR_H

#include "core/app_control.h"
#include "rules/rule_types.h"

class RuleValidator
{
public:
  static uint8_t ApplyRules(control_t *action, control_t *control, control_sensor_data_t *SENSORS)
  {

    if (action->status == ActionStatus::ACTION_ON)
    {
      // Unit is Active
      if (action->rule == RuleStatus::RULE_OFF)
      {
        // No rule is selected so trigerring rhe default behaivior of STATUS ON
        return 1;
      }
      else
      {
        if (RuleValidator::onWhenRule(action, SENSORS))
        {
          // START
          return 1;
        }

        if (RuleValidator::offWhenRule(action, SENSORS))
        {
          // STOP

          // If is time based : Reset action state off/on time
          if (action->onWhenRule == RuleType::TIME_RULE && action->rule == RuleStatus::RULE_ON)
          {
            int addTime = ((action->time.HOUR * 60) * 60) + (action->time.MINUTES * 60) + action->time.SECONDS;

            // Update when Current time > off time + the off time value or so called rest time
            if (addTime >= (action->time.OFF_TIME + action->offWhenValue))
            {
              control->time.ON_TIME =
                  addTime + action->onWhenValue;

              control->time.OFF_TIME =
                  addTime + action->onWhenValue;
            }
          }

         
        }
      }
    }

    return 0;
  }
  static bool onWhenRule(control_t *data, control_sensor_data_t *SENSORS)
  {
    switch (data->onWhenRule)
    {
    case static_cast<uint8_t>(RuleType::TEMPERATURE_RULE):
        /* Temperature */
        return whenCondition(data->onWhenCondition, data->onWhenValue, SENSORS->temperature);
    case static_cast<uint8_t>(RuleType::HUMIDITY_RULE):
        /* Humidity */
        return whenCondition(data->onWhenCondition, data->onWhenValue, SENSORS->humidity);
    case static_cast<uint8_t>(RuleType::TANK_LEVEL_RULE):
        /* Tank Level */
        return whenCondition(data->onWhenCondition, data->onWhenValue, SENSORS->waterTankLevel);
    case static_cast<uint8_t>(RuleType::TIME_RULE):
        /* Hours */
        return onTimeCondition(data->time.HOUR, data->time.MINUTES, data->time.SECONDS, data->time.ON_TIME);
    default:
      /* Nothing to happend */
      return false;
    }

    return false;
  }

  static bool offWhenRule(control_t *data, control_sensor_data_t *SENSORS)
  {
    switch (data->offWhenRule)
    {
    case static_cast<uint8_t>(RuleType::TEMPERATURE_RULE):
        /* Temperature */
        return whenCondition(data->offWhenCondition, data->offWhenValue, SENSORS->temperature);
    case static_cast<uint8_t>(RuleType::HUMIDITY_RULE):
        /* Humidity */
        return whenCondition(data->offWhenCondition, data->offWhenValue, SENSORS->humidity);
    case static_cast<uint8_t>(RuleType::TANK_LEVEL_RULE):
        /* Tank Level */
        return whenCondition(data->offWhenCondition, data->offWhenValue, SENSORS->waterTankLevel);
    case static_cast<uint8_t>(RuleType::TIME_RULE):
        /* Hours */
        return offTimeCondition(data->time.HOUR, data->time.MINUTES, data->time.SECONDS, data->time.OFF_TIME);
    default:
      /* Nothing to happend */
      return false;
    }

    return false;
  }

  static bool isUpdatable(control_t *data, uint8_t pastTimeFromExecution)
  {
    return isOffTimeConditionPassed(data->time.HOUR, data->time.MINUTES, data->time.SECONDS, data->time.OFF_TIME, pastTimeFromExecution);
  }

private:
  static bool whenCondition(uint8_t onWhenCondition, float dataValue, float stateValue)
  {
    switch (onWhenCondition)
    {
    case static_cast<uint8_t>(RuleConditionType::GreatherThen):
      /* > */
      if (stateValue > dataValue)
      {
        return true;
      }
      break;
    case static_cast<uint8_t>(RuleConditionType::LessThen):
      /* < */
      if (stateValue < dataValue)
      {
        return true;
      }
      break;
    case static_cast<uint8_t>(RuleConditionType::EqualCondition):
      /* = */
      if (dataValue == stateValue)
      {
        return true;
      }
      break;
    default:
      /* Nothing to happend */
      return false;
    }

    return false;
  }

  static bool offTimeCondition(int current_h, int current_m, int current_s, int time_left)
  {
    if ((((current_h * 60) * 60) + (current_m * 60) + current_s) >= time_left)
    {
      return true;
    }

    return false;
  }

  static bool onTimeCondition(int current_h, int current_m, int current_s, int time_left)
  {
    if ((((current_h * 60) * 60) + (current_m * 60) + current_s) <= time_left)
    {
      return true;
    }

    return false;
  }

  static bool isOffTimeConditionPassed(int current_h, int current_m, int current_s, int time_left, uint8_t pastTimeFromExecution)
  {
    if (((((current_h * 60) * 60) + (current_m * 60) + current_s) - pastTimeFromExecution) >= time_left)
    {
      return true;
    }

    return false;
  }
};

#endif