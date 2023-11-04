#ifndef APP_CONTROL_H
#define APP_CONTROL_H

#include <Arduino.h>
#include <SD.h>
#include "utility/log/lab_logger.h"
#include "core/app_functions.h"
#include <ESP32Time.h>
typedef struct
{
    float temperature = 0;
    float humidity = 0;
    float waterTankLevel = 0;
} control_sensor_data_t;

typedef struct
{
    int HOUR = 0;
    int MINUTES = 0;
    int SECONDS = 0;
    float ON_TIME = 0;
    float OFF_TIME = 0;
} app_time_t;

typedef struct
{
    // Is turnd on or off
    uint8_t status = 0;
    // Is going to use conditions
    uint8_t rule = 0;
    // Unit name
    String unit;
    // One rule: 1: Temperature, 2: Humidity, 3:Tank Level, 4:Hours
    uint8_t onWhenRule = 0;
    // One rule condition: 1: > 2: < 3: ==
    uint8_t onWhenCondition = 0;
    // One rule condition value
    float onWhenValue = 0;
    // Off rule: 1: Temperature, 2: Humidity, 3:Tank Level, 4:Hours
    uint8_t offWhenRule = 0;
    // Off rule condition: 1: > 2: < 3: ==
    uint8_t offWhenCondition = 0;
    // Off rule condition value
    float offWhenValue = 0;
    app_time_t time;
} control_t;

typedef struct
{
    // NEW Functions MUST follow the same order like ActionFunctionIDX
    const function_t FUNCTIONS[TOTAL_CORE_FUNCTIONS] =
        {
            AppFunctions::function_lightSource,
            AppFunctions::function_inputPump,
            AppFunctions::function_outputPump,
            AppFunctions::function_fanIntake,
            AppFunctions::function_fanExhaus,
            AppFunctions::function_oxygenPump,
            };
    control_t CONTROLS[TOTAL_CORE_FUNCTIONS];
    control_sensor_data_t SENSORS;
} app_control_t;

enum ActionStatus{
    ACTION_ON = 1,
    ACTION_OFF = 0
};

enum RuleStatus{
    RULE_ON = 1,
    RULE_OFF = 0
};

class AppControlMapper
{
public:
    AppControlMapper(LabLogger *logger)
    {
        this->logger = logger;
    }

    void MapFromStorage(fs::FS &fs, const char *datafile, app_control_t *app_control)
    {
        // Open file for writing
        if (!fs.exists(datafile))
        {
            this->logger->log("[AppControl]:[ERR]: Can't find datafile");
            return;
        }
        
        File file = fs.open(datafile);
        if (!file)
        {
            this->logger->log("[AppControl]:[ERR]: Failed to create file");
            return;
        }

        String jsonString = file.readString();

        // Create a JSON document
        DynamicJsonDocument jsonDoc(3072); // Adjust buffer size as needed

        this->logger->log("[AppControl]:[INF]: Deserializing Control json");

        // Deserialize the JSON data from the string
        DeserializationError error = deserializeJson(jsonDoc, jsonString);

        if (error)
        {
            this->logger->log("[AppControl]:[ERR]: Failed to deserialize JSON");
            return;
        }

        // Access and use the JSON data
        JsonObject sensors = jsonDoc["sensors"];
        app_control->SENSORS.temperature = sensors["temperature"].as<float>(); // TODO: #18 Find bug in temperature/humidity
        app_control->SENSORS.humidity = sensors["humidity"].as<float>();
        app_control->SENSORS.waterTankLevel = sensors["waterTankLevel"].as<float>();

        JsonArray devices = jsonDoc["devices"];

        for (uint8_t i = 0; i < AppFunctions::TOTAL_CORE_FUNCTIONS; i++)
        {
            app_control->CONTROLS[i].status = devices[i]["status"];
            app_control->CONTROLS[i].unit = devices[i]["unit"].as<String>();
            app_control->CONTROLS[i].rule = devices[i]["rule"];
            app_control->CONTROLS[i].onWhenRule = devices[i]["onWhen"]["rule"];
            app_control->CONTROLS[i].onWhenCondition = devices[i]["onWhen"]["condition"];
            app_control->CONTROLS[i].onWhenValue = devices[i]["onWhen"]["value"];
            app_control->CONTROLS[i].offWhenRule = devices[i]["offWhen"]["rule"];
            app_control->CONTROLS[i].offWhenCondition = devices[i]["offWhen"]["condition"];
            app_control->CONTROLS[i].offWhenValue = devices[i]["offWhen"]["value"];

            app_control->CONTROLS[i].time.HOUR = devices[i]["time"]["HOUR"];
            app_control->CONTROLS[i].time.MINUTES = devices[i]["time"]["MINUTES"];
            app_control->CONTROLS[i].time.SECONDS = devices[i]["time"]["SECONDS"];
            app_control->CONTROLS[i].time.ON_TIME = devices[i]["time"]["ON_TIME"];
            app_control->CONTROLS[i].time.OFF_TIME = devices[i]["time"]["OFF_TIME"];
        }

        file.close();
    }

    void RestorToDefaultStorage(fs::FS &fs, const char *datafile, app_control_t *app_control)
    {
        // Open file for writing
        if (!fs.exists(datafile))
        {
            this->logger->log("[ASM]:[ERR]: Can't find contol file");
            return;
        }

        File file = fs.open(datafile, FILE_WRITE);

        if (!file)
        {
            this->logger->log("[ASM]:[ERR]: Failed to create control file");
            return;
        }

        DynamicJsonDocument jsonDoc(4000); // Adjust the buffer size as needed

        JsonObject sensors = jsonDoc.createNestedObject("sensors");
        sensors["temperature"] = app_control->SENSORS.temperature;
        sensors["humidity"] = app_control->SENSORS.humidity;
        sensors["waterTankLevel"] = app_control->SENSORS.waterTankLevel;

        // // Create a JSON array for devices
        JsonArray devices = jsonDoc.createNestedArray("devices");

        // // Create and add objects for each device
        for (int i = 0; i < AppFunctions::TOTAL_CORE_FUNCTIONS; i++)
        {
            JsonObject device = devices.createNestedObject();
            device["status"] = 0;
            device["unit"] = app_control->CONTROLS[i].unit;
            device["rule"] = 0;

            JsonObject onWhen = device.createNestedObject("onWhen");
            onWhen["rule"] = 0;
            onWhen["condition"] = 0;
            onWhen["value"] = 0;

            JsonObject offWhen = device.createNestedObject("offWhen");
            offWhen["rule"] =0;
            offWhen["condition"] = 0;
            offWhen["value"] = 0;

            JsonObject time = device.createNestedObject("time");
            time["HOUR"] = 0;
            time["MINUTES"] = 0;
            time["SECONDS"] = 0;
            time["ON_TIME"] = 0;
            time["OFF_TIME"] = 0;

            // Restore current control states
            app_control->CONTROLS[i].status = 0;
            app_control->CONTROLS[i].rule = 0;
            app_control->CONTROLS[i].unit = app_control->CONTROLS[i].unit;
            app_control->CONTROLS[i].offWhenRule = 0;
            app_control->CONTROLS[i].offWhenCondition = 0;
            app_control->CONTROLS[i].offWhenValue = 0;
            app_control->CONTROLS[i].onWhenRule = 0;
            app_control->CONTROLS[i].onWhenCondition = 0;
            app_control->CONTROLS[i].onWhenValue = 0;
            app_control->CONTROLS[i].time.HOUR = 0;
            app_control->CONTROLS[i].time.SECONDS = 0;
            app_control->CONTROLS[i].time.MINUTES =0;
            app_control->CONTROLS[i].time.ON_TIME =0;
            app_control->CONTROLS[i].time.OFF_TIME = 0;
        }

        // Serialize JSON to file
        if (serializeJson(jsonDoc, file) == 0)
        {
            this->logger->log("[ASM]:[ERR]: Failed to write contol file");
        }

        // Close the file
        file.close();
    }

    void MapToStorage(fs::FS &fs, const char *datafile, app_control_t *app_control)
    {
        // Open file for writing
        if (!fs.exists(datafile))
        {
            this->logger->log("[ASM]:[ERR]: Can't find contol file");
            return;
        }

        File file = fs.open(datafile, FILE_WRITE);

        if (!file)
        {
            this->logger->log("[ASM]:[ERR]: Failed to create control file");
            return;
        }

        DynamicJsonDocument jsonDoc(4000); // Adjust the buffer size as needed

        JsonObject sensors = jsonDoc.createNestedObject("sensors");
        sensors["temperature"] = app_control->SENSORS.temperature;
        sensors["humidity"] = app_control->SENSORS.humidity;
        sensors["waterTankLevel"] = app_control->SENSORS.waterTankLevel;

        // // Create a JSON array for devices
        JsonArray devices = jsonDoc.createNestedArray("devices");

        // // Create and add objects for each device
        for (int i = 0; i < AppFunctions::TOTAL_CORE_FUNCTIONS; i++)
        {
            JsonObject device = devices.createNestedObject();
            device["status"] = app_control->CONTROLS[i].status;
            device["unit"] = app_control->CONTROLS[i].unit;
            device["rule"] = app_control->CONTROLS[i].rule;

            JsonObject onWhen = device.createNestedObject("onWhen");
            onWhen["rule"] = app_control->CONTROLS[i].onWhenRule;
            onWhen["condition"] = app_control->CONTROLS[i].onWhenCondition;
            onWhen["value"] = app_control->CONTROLS[i].onWhenValue;

            JsonObject offWhen = device.createNestedObject("offWhen");
            offWhen["rule"] = app_control->CONTROLS[i].offWhenRule;
            offWhen["condition"] = app_control->CONTROLS[i].offWhenCondition;
            offWhen["value"] = app_control->CONTROLS[i].offWhenValue;

            JsonObject time = device.createNestedObject("time");
            time["HOUR"] = app_control->CONTROLS[i].time.HOUR;
            time["MINUTES"] = app_control->CONTROLS[i].time.MINUTES;
            time["SECONDS"] = app_control->CONTROLS[i].time.SECONDS;
            time["ON_TIME"] = app_control->CONTROLS[i].time.ON_TIME;
            time["OFF_TIME"] = app_control->CONTROLS[i].time.OFF_TIME;
        }

        // Serialize JSON to file
        if (serializeJson(jsonDoc, file) == 0)
        {
            this->logger->log("[ASM]:[ERR]: Failed to write contol file");
        }

        // Close the file
        file.close();
    }

    String MapToString(app_control_t *app_control)
    {
        DynamicJsonDocument jsonDoc(3072); // Adjust the buffer size as needed

        JsonObject sensors = jsonDoc.createNestedObject("sensors");
        sensors["temperature"] = app_control->SENSORS.temperature;
        sensors["humidity"] = app_control->SENSORS.humidity;
        sensors["waterTankLevel"] = app_control->SENSORS.waterTankLevel;

        // Create a JSON array for devices
        JsonArray devices = jsonDoc.createNestedArray("devices");

        // Create and add objects for each device
        for (int i = 0; i < AppFunctions::TOTAL_CORE_FUNCTIONS; i++)
        {
            JsonObject device = devices.createNestedObject();
            device["status"] = app_control->CONTROLS[i].status;
            device["unit"] = app_control->CONTROLS[i].unit;
            device["rule"] = app_control->CONTROLS[i].rule;

            JsonObject onWhen = device.createNestedObject("onWhen");
            onWhen["rule"] = app_control->CONTROLS[i].onWhenRule;
            onWhen["condition"] = app_control->CONTROLS[i].onWhenCondition;
            onWhen["value"] = app_control->CONTROLS[i].onWhenValue;

            JsonObject offWhen = device.createNestedObject("offWhen");
            offWhen["rule"] = app_control->CONTROLS[i].offWhenRule;
            offWhen["condition"] = app_control->CONTROLS[i].offWhenCondition;
            offWhen["value"] = app_control->CONTROLS[i].offWhenValue;
        }

        String jsonString;

        // Serialize JSON to file
        if (serializeJson(jsonDoc, jsonString) == 0)
        {
            this->logger->log("[ASM]:[ERR]: Failed to serialize to string");
        }

        // TODO: Add UI to Backend
        // debugger(jsonString);

        return jsonString;
    }

    void MapToFrom(control_t *action_to, control_t *action_from)
    {
        action_to->status = action_from->status;
        action_to->rule = action_from->rule;
        action_to->unit = action_from->unit;
        action_to->offWhenCondition = action_from->offWhenCondition;
        action_to->offWhenRule = action_from->offWhenRule;
        action_to->offWhenValue = action_from->offWhenValue;
        action_to->onWhenCondition = action_from->onWhenCondition;
        action_to->onWhenRule = action_from->onWhenRule;
        action_to->onWhenValue = action_from->onWhenValue;
        action_to->time.ON_TIME = action_from->time.ON_TIME;
        action_to->time.OFF_TIME = action_from->time.OFF_TIME;
    }

private:
    LabLogger *logger;
};
#endif