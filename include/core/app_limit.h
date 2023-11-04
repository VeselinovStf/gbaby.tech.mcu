#ifndef APP_LIMIT_H
#define APP_LIMIT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "utility/log/lab_logger.h"
#include "core/app_feature.h"
#include "core/app_notification.h"
typedef struct
{
    float min;
    float max;
    String name;
} limit_t;

typedef struct
{
    limit_t limits[TOTAL_LIMITS];
} app_limit_t;

class AppLimitValidator
{
public:
    static bool validate(limit_t *limit, float value)
    {
        if (value >= limit->max || value <= limit->min)
        {
            return false;
        }
 
        return true;
    }

    static bool validate(app_limit_t *limits, app_control_t *app_control)
    {
        for (uint8_t i = 0; i < TOTAL_LIMITS; i++)
        {
            if (!AppLimitValidator::validate(&limits->limits[i], app_control->SENSORS.temperature) && !AppLimitValidator::validate(&limits->limits[i], app_control->SENSORS.humidity) && !AppLimitValidator::validate(&limits->limits[i], app_control->SENSORS.waterTankLevel))
            {
                return false;
            }
        }

        return true;
    }

    static bool validate(app_limit_t *limits, app_control_t *app_control, app_notification_t *app_notifications)
    {
        for (uint8_t i = 0; i < TOTAL_LIMITS; i++)
        {
            if (!AppLimitValidator::validate(&limits->limits[i], app_control->SENSORS.temperature) && !AppLimitValidator::validate(&limits->limits[i], app_control->SENSORS.humidity) && !AppLimitValidator::validate(&limits->limits[i], app_control->SENSORS.waterTankLevel))
            {
                return false;
            }
        }

        return true;
    }
};

class AppLimitMapper
{
public:
    AppLimitMapper(LabLogger *logger, app_limit_t *init)
    {
        this->logger = logger;

        for (uint8_t i = 0; i < TOTAL_LIMITS; i++)
        {
            init->limits[i].name = this->default_limits[i].name;
        }
    }

    void MapToStorage(fs::FS &fs, const char *datafile, limit_t *app_limit)
    {
        // Open file for writing
        File file = SD.open(datafile, FILE_WRITE);
        if (!file)
        {
            this->logger->log("[ALM]:[ERR]: Failed to create file");
            return;
        }

        // Allocate a temporary JsonDocument
        StaticJsonDocument<2024> doc;

        doc["min"] = app_limit->min;
        doc["max"] = app_limit->max;
        doc["name"] = app_limit->name;

        // Serialize JSON to file
        if (serializeJson(doc, file) == 0)
        {
            this->logger->log("[ALM]:[ERR]: Failed to write to file");
        }

        // Close the file
        file.close();
    }

    void MapFromStorage(fs::FS &fs, const char *datafile, limit_t *app_limit)
    {
        // Open file for writing
        if (!SD.exists(datafile))
        {
            this->logger->log("[AppLimitMapper]:[ERR]: Can't find contol file");
            return;
        }

        File original = SD.open(datafile, FILE_READ);
        String jsonString = original.readString();
        DynamicJsonDocument jsonDoc(2024); // Adjust buffer size as needed

        this->logger->log("[AppLimitMapper]:[INF]: Deserializing Control json");

        // Deserialize the JSON data from the string
        DeserializationError error = deserializeJson(jsonDoc, jsonString);

        if (error)
        {
            this->logger->log("[AppLimitMapper]:[ERR]: Failed to deserialize JSON");
            return;
        }

        app_limit->min = jsonDoc["min"];
        app_limit->max = jsonDoc["max"];
        app_limit->name = jsonDoc["name"].as<String>();

        // Close the file
        original.close();
    }

    void MapAllFromStorage(fs::FS &fs, app_limit_t *app_limit)
    {
        for (uint8_t i = 0; i < TOTAL_LIMITS; i++)
        {
            String datafile = "/data/" + app_limit->limits[i].name + "-environment-limits.json";
            this->MapFromStorage(fs, datafile.c_str(), &app_limit->limits[i]);
        }
    }

    String MapToString(limit_t *app_limit)
    {
        DynamicJsonDocument jsonDoc(3072); // Adjust the buffer size as needed

        JsonObject limits = jsonDoc.createNestedObject("limits");

        limits["min"] = app_limit->min;
        limits["max"] = app_limit->max;
        limits["name"] = app_limit->name;

        String jsonString;

        // Serialize JSON to file
        if (serializeJson(jsonDoc, jsonString) == 0)
        {
            this->logger->log("[AppLimitMapper]:[ERR]: Failed to serialize to string");
        }

        // TODO: Add UI to Backend
        // debugger(jsonString);

        return jsonString;
    }

private:
    LabLogger *logger;
    limit_t default_limits[TOTAL_LIMITS] = {
        {1, 0, "t"},
        {2, 0, "h"}, // TODO: #95 Add Notification property -> send Notifications on triggering
        {3, 0, "wl"} // TODO: #93 Implement "c" - clean - a days based limit for cleaning !!
    };
};
#endif