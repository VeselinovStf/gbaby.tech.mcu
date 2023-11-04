#ifndef APP_FEATURE_STATE_H
#define APP_FEATURE_STATE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "utility/log/lab_logger.h"
#include "core/app_functions.h"
#include "core/app_control.h"

typedef struct
{
    uint8_t total = 0;
} feature_config_t;
typedef struct
{
    feature_config_t CONFIG;
    String NAME;
    uint8_t ID;
} action_feature_t;

class AppFeatureMapper
{
public:
    AppFeatureMapper(LabLogger *logger)
    {
        this->logger = logger;
    }

    void UpdateToStorage(const char *datafile, app_control_t *app_control, action_feature_t *feature_config, control_t *feature_state)
    {
        // Open file for writing
        if (!SD.exists(datafile))
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Can't find contol file");
            return;
        }

        File original = SD.open(datafile, FILE_READ);
        String jsonString = original.readString();
        DynamicJsonDocument originalFeaturesDoc(15000); // Adjust buffer size as needed

        this->logger->log("[FEATURE_MAPPER]:[INF]: Deserializing Control json");

        // Deserialize the JSON data from the string
        DeserializationError error = deserializeJson(originalFeaturesDoc, jsonString);
        if (error)
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Failed to deserialize JSON");
            return;
        }

        original.close();

        File file = SD.open(datafile, FILE_WRITE);

        if (!file)
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Failed to create control file");
            return;
        }

        this->logger->log("[FEATURE_MAPPER]:[INF]: Mapping Original");

        DynamicJsonDocument jsonDoc(15000); // Adjust the buffer size as needed

        uint8_t t = originalFeaturesDoc["config"]["totalAdded"].as<uint8_t>();
        uint8_t totalAdded = t + 1;

        JsonObject config = jsonDoc.createNestedObject("config");
        config["total"] = totalAdded;

        // // Create a JSON array for devices
        JsonArray features = jsonDoc.createNestedArray("features");

        // MAP ORIGINAL Feature Configuration
        for (int i = 0; i < t; i++)
        {
            JsonObject device = features.createNestedObject();
            device["id"] = originalFeaturesDoc["features"][i]["id"];
            device["name"] = originalFeaturesDoc["features"][i]["name"];

            device["status"] = originalFeaturesDoc["features"][i]["status"];
            device["unit"] = originalFeaturesDoc["features"][i]["unit"];
            device["rule"] = originalFeaturesDoc["features"][i]["rule"];

            JsonObject onWhen = device.createNestedObject("onWhen");
            onWhen["rule"] = originalFeaturesDoc["features"][i]["onWhenRule"];
            onWhen["condition"] = originalFeaturesDoc["features"][i]["onWhenCondition"];
            onWhen["value"] = originalFeaturesDoc["features"][i]["onWhenValue"];

            JsonObject offWhen = device.createNestedObject("offWhen");
            offWhen["rule"] = originalFeaturesDoc["features"][i]["offWhenRule"];
            offWhen["condition"] = originalFeaturesDoc["features"][i]["offWhenCondition"];
            offWhen["value"] = originalFeaturesDoc["features"][i]["offWhenValue"];

            JsonObject time = device.createNestedObject("time");
            time["HOUR"] = originalFeaturesDoc["features"][i]["time.HOUR"];
            time["MINUTES"] = originalFeaturesDoc["features"][i]["time.MINUTES"];
            time["SECONDS"] = originalFeaturesDoc["features"][i]["time.SECONDS"];
            time["ON_TIME"] = originalFeaturesDoc["features"][i]["time.ON_TIME"];
            time["OFF_TIME"] = originalFeaturesDoc["features"][i]["time.OFF_TIME"];

            JsonArray devices = device.createNestedArray("devices");

            for (int j = 0; j < AppFunctions::TOTAL_CORE_FUNCTIONS; j++)
            {
                JsonObject d = devices.createNestedObject();

                d["status"] = originalFeaturesDoc["features"][i][j]["status"];
                d["unit"] = originalFeaturesDoc["features"][i]["unit"];
                d["rule"] = originalFeaturesDoc["features"][i]["rule"];

                JsonObject onWhen = d.createNestedObject("onWhen");
                onWhen["rule"] = originalFeaturesDoc["features"][i]["onWhenRule"];
                onWhen["condition"] = originalFeaturesDoc["features"][i]["onWhenCondition"];
                onWhen["value"] = originalFeaturesDoc["features"][i]["onWhenValue"];

                JsonObject offWhen = d.createNestedObject("offWhen");
                offWhen["rule"] = originalFeaturesDoc["features"][i]["offWhenRule"];
                offWhen["condition"] = originalFeaturesDoc["features"][i]["offWhenCondition"];
                offWhen["value"] = originalFeaturesDoc["features"][i]["offWhenValue"];

                JsonObject time = d.createNestedObject("time");
                time["HOUR"] = originalFeaturesDoc["features"][i]["time.HOUR"];
                time["MINUTES"] = originalFeaturesDoc["features"][i]["time.MINUTES"];
                time["SECONDS"] = originalFeaturesDoc["features"][i]["time.SECONDS"];
                time["ON_TIME"] = originalFeaturesDoc["features"][i]["time.ON_TIME"];
                time["OFF_TIME"] = originalFeaturesDoc["features"][i]["time.OFF_TIME"];
            }
        }

        this->logger->log("[FEATURE_MAPPER]:[INF]: Mapping new Feature");

        // MAP NEW Feature Setup Controll
        JsonObject nf = features.createNestedObject();
        nf["id"] = totalAdded;
        nf["name"] = feature_config->NAME;

        nf["status"] = feature_state->status;
        nf["unit"] = feature_state->unit;
        nf["rule"] = feature_state->rule;

        JsonObject onWhen = nf.createNestedObject("onWhen");
        onWhen["rule"] = feature_state->onWhenRule;
        onWhen["condition"] = feature_state->onWhenCondition;
        onWhen["value"] = feature_state->onWhenValue;

        JsonObject offWhen = nf.createNestedObject("offWhen");
        offWhen["rule"] = feature_state->offWhenRule;
        offWhen["condition"] = feature_state->offWhenCondition;
        offWhen["value"] = feature_state->offWhenValue;

        JsonObject time = nf.createNestedObject("time");
        time["HOUR"] = feature_state->time.HOUR;
        time["MINUTES"] = feature_state->time.MINUTES;
        time["SECONDS"] = feature_state->time.SECONDS;
        time["ON_TIME"] = feature_state->time.ON_TIME;
        time["OFF_TIME"] = feature_state->time.OFF_TIME;

        JsonArray devices = nf.createNestedArray("devices");
        for (int j = 0; j < AppFunctions::TOTAL_CORE_FUNCTIONS; j++)
        {
            JsonObject d = devices.createNestedObject();

            d["status"] = app_control->CONTROLS[j].status;
            d["unit"] = app_control->CONTROLS[j].unit;
            d["rule"] = app_control->CONTROLS[j].rule;

            JsonObject onWhen = d.createNestedObject("onWhen");
            onWhen["rule"] = app_control->CONTROLS[j].onWhenRule;
            onWhen["condition"] = app_control->CONTROLS[j].onWhenCondition;
            onWhen["value"] = app_control->CONTROLS[j].onWhenValue;

            JsonObject offWhen = d.createNestedObject("offWhen");
            offWhen["rule"] = app_control->CONTROLS[j].offWhenRule;
            offWhen["condition"] = app_control->CONTROLS[j].offWhenCondition;
            offWhen["value"] = app_control->CONTROLS[j].offWhenValue;

            JsonObject time = d.createNestedObject("time");
            time["HOUR"] = app_control->CONTROLS[j].time.HOUR;
            time["MINUTES"] = app_control->CONTROLS[j].time.MINUTES;
            time["SECONDS"] = app_control->CONTROLS[j].time.SECONDS;
            time["ON_TIME"] = app_control->CONTROLS[j].time.ON_TIME;
            time["OFF_TIME"] = app_control->CONTROLS[j].time.OFF_TIME;
        }

        // Serialize JSON to file
        if (serializeJson(jsonDoc, file) == 0)
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Failed to write contol file");
        }

        this->logger->log("[FEATURE_MAPPER]:[INF]: New Feature Added!");

        // Close the file
        file.close();
    }

    void MapFeature(fs::FS &fs, long id, const char *datafile, app_control_t *app_control)
    {
        // Open file for writing
        if (!SD.exists(datafile))
        {
            this->logger->log("[MAP_FEATURE_MAPPER]:[ERR]: Can't find contol file");
            return;
        }

        File original = SD.open(datafile, FILE_READ);
        String jsonString = original.readString();
        DynamicJsonDocument originalFeaturesDoc(15000); // Adjust buffer size as needed

        this->logger->log("[MAP_FEATURE_MAPPER]:[INF]: Deserializing Control json");

        // Deserialize the JSON data from the string
        DeserializationError error = deserializeJson(originalFeaturesDoc, jsonString);
        if (error)
        {
            this->logger->log("[MAP_FEATURE_MAPPER]:[ERR]: Failed to deserialize JSON");
            return;
        }

        uint8_t totalAdded = originalFeaturesDoc["config"]["total"].as<uint8_t>();
        for (uint8_t i = 0; i < totalAdded; i++)
        {
            if (originalFeaturesDoc["features"][i]["id"].as<uint8_t>() == id)
            {
                for (int j = 0; j < AppFunctions::TOTAL_CORE_FUNCTIONS; j++)
                {
                    app_control->CONTROLS[j].status = originalFeaturesDoc["features"][i]["devices"][j]["status"];
                    app_control->CONTROLS[j].rule = originalFeaturesDoc["features"][i]["devices"][j]["rule"];
                    app_control->CONTROLS[j].unit = originalFeaturesDoc["features"][i]["devices"][j]["unit"].as<String>();
                    app_control->CONTROLS[j].offWhenRule = originalFeaturesDoc["features"][i]["devices"][j]["offWhen"]["rule"];
                    app_control->CONTROLS[j].offWhenCondition = originalFeaturesDoc["features"][i]["devices"][j]["offWhen"]["condition"];
                    app_control->CONTROLS[j].offWhenValue = originalFeaturesDoc["features"][i]["devices"][j]["offWhen"]["value"];
                    app_control->CONTROLS[j].onWhenRule = originalFeaturesDoc["features"][i]["devices"][j]["onWhen"]["rule"];
                    app_control->CONTROLS[j].onWhenCondition = originalFeaturesDoc["features"][i]["devices"][j]["onWhen"]["condition"];
                    app_control->CONTROLS[j].onWhenValue = originalFeaturesDoc["features"][i]["devices"][j]["onWhen"]["value"];
                    app_control->CONTROLS[j].time.HOUR = originalFeaturesDoc["features"][i]["devices"][j]["time"]["HOUR"];
                    app_control->CONTROLS[j].time.SECONDS = originalFeaturesDoc["features"][i]["devices"][j]["time"]["SECONDS"];
                    app_control->CONTROLS[j].time.MINUTES = originalFeaturesDoc["features"][i]["devices"][j]["time"]["MINUTES"];
                    app_control->CONTROLS[j].time.ON_TIME = originalFeaturesDoc["features"][i]["devices"][j]["time"]["ON_TIME"];
                    app_control->CONTROLS[j].time.OFF_TIME = originalFeaturesDoc["features"][i]["devices"][j]["time"]["OFF_TIME"];
                }

                break;
            }
        }

        original.close();
    }

    void MapToStorage(fs::FS &fs, long id, const char *datafile, control_t *feature_state, action_feature_t *feature_config)
    {
        // Open file for writing
        if (!SD.exists(datafile))
        {
            this->logger->log("[MAP_FEATURE_MAPPER]:[ERR]: Can't find contol file");
            return;
        }

        // TODO: #84 Protection Handlers: Main object must hold property for app stability
        //       if this property is to low, on app start, all resources and states must be refreshed

        File original = SD.open(datafile, FILE_READ);
        String jsonString = original.readString();
        DynamicJsonDocument originalFeaturesDoc(15000); // Adjust buffer size as needed

        this->logger->log("[FEATURE_MAPPER]:[INF]: Deserializing Control json");

        // Deserialize the JSON data from the string
        DeserializationError error = deserializeJson(originalFeaturesDoc, jsonString);
        if (error)
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Failed to deserialize JSON");
            return;
        }

        original.close();

        File file = SD.open(datafile, FILE_WRITE);

        if (!file)
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Failed to create control file");
            return;
        }

        this->logger->log("[FEATURE_MAPPER]:[INF]: Mapping Original");

        DynamicJsonDocument jsonDoc(15000);

        JsonObject config = jsonDoc.createNestedObject("config");

        feature_config->CONFIG.total = originalFeaturesDoc["config"]["total"].as<uint8_t>();

        config["total"] = feature_config->CONFIG.total;

        // // Create a JSON array for devices
        JsonArray features = jsonDoc.createNestedArray("features");

        // MAP ORIGINAL Feature Configuration
        for (int i = 0; i < feature_config->CONFIG.total; i++)
        {
            JsonObject nf = features.createNestedObject();

            if (originalFeaturesDoc["features"][i]["id"].as<uint8_t>() == id)
            {
                nf["id"] = i;
                nf["name"] = feature_config->NAME;

                nf["status"] = feature_state->status;
                nf["unit"] = feature_state->unit;
                nf["rule"] = feature_state->rule;

                JsonObject onWhen = nf.createNestedObject("onWhen");
                onWhen["rule"] = feature_state->onWhenRule;
                onWhen["condition"] = feature_state->onWhenCondition;
                onWhen["value"] = feature_state->onWhenValue;

                JsonObject offWhen = nf.createNestedObject("offWhen");
                offWhen["rule"] = feature_state->offWhenRule;
                offWhen["condition"] = feature_state->offWhenCondition;
                offWhen["value"] = feature_state->offWhenValue;

                JsonObject time = nf.createNestedObject("time");
                time["HOUR"] = feature_state->time.HOUR;
                time["MINUTES"] = feature_state->time.MINUTES;
                time["SECONDS"] = feature_state->time.SECONDS;
                time["ON_TIME"] = feature_state->time.ON_TIME;
                time["OFF_TIME"] = feature_state->time.OFF_TIME;
            }
            else
            {

                nf["id"] = originalFeaturesDoc["features"][i]["id"];
                nf["name"] = originalFeaturesDoc["features"][i]["name"];
                nf["status"] = originalFeaturesDoc["features"][i]["status"];
                nf["unit"] = originalFeaturesDoc["features"][i]["unit"];
                nf["rule"] = originalFeaturesDoc["features"][i]["rule"];

                JsonObject onWhen = nf.createNestedObject("onWhen");
                onWhen["rule"] = originalFeaturesDoc["features"][i]["onWhenRule"];
                onWhen["condition"] = originalFeaturesDoc["features"][i]["onWhenCondition"];
                onWhen["value"] = originalFeaturesDoc["features"][i]["onWhenValue"];

                JsonObject offWhen = nf.createNestedObject("offWhen");
                offWhen["rule"] = originalFeaturesDoc["features"][i]["offWhenRule"];
                offWhen["condition"] = originalFeaturesDoc["features"][i]["offWhenCondition"];
                offWhen["value"] = originalFeaturesDoc["features"][i]["offWhenValue"];

                JsonObject time = nf.createNestedObject("time");
                time["HOUR"] = originalFeaturesDoc["features"][i]["time.HOUR"];
                time["MINUTES"] = originalFeaturesDoc["features"][i]["time.MINUTES"];
                time["SECONDS"] = originalFeaturesDoc["features"][i]["time.SECONDS"];
                time["ON_TIME"] = originalFeaturesDoc["features"][i]["time.ON_TIME"];
                time["OFF_TIME"] = originalFeaturesDoc["features"][i]["time.OFF_TIME"];
            }

            JsonArray devices = nf.createNestedArray("devices");

            for (int j = 0; j < AppFunctions::TOTAL_CORE_FUNCTIONS; j++)
            {
                JsonObject d = devices.createNestedObject();

                d["status"] = originalFeaturesDoc["features"][i]["devices"][j]["status"];
                d["unit"] = originalFeaturesDoc["features"][i]["devices"][j]["unit"];
                d["rule"] = originalFeaturesDoc["features"][i]["devices"][j]["rule"];

                JsonObject onWhen = d.createNestedObject("onWhen");
                onWhen["rule"] = originalFeaturesDoc["features"][i]["devices"][j]["onWhenRule"];
                onWhen["condition"] = originalFeaturesDoc["features"][i]["devices"][j]["onWhenCondition"];
                onWhen["value"] = originalFeaturesDoc["features"][i]["devices"][j]["onWhenValue"];

                JsonObject offWhen = d.createNestedObject("offWhen");
                offWhen["rule"] = originalFeaturesDoc["features"][i]["devices"][j]["offWhenRule"];
                offWhen["condition"] = originalFeaturesDoc["features"][i]["devices"][j]["offWhenCondition"];
                offWhen["value"] = originalFeaturesDoc["features"][i]["devices"][j]["offWhenValue"];

                JsonObject time = d.createNestedObject("time");
                time["HOUR"] = originalFeaturesDoc["features"][i]["devices"][j]["time.HOUR"];
                time["MINUTES"] = originalFeaturesDoc["features"][i]["devices"][j]["time.MINUTES"];
                time["SECONDS"] = originalFeaturesDoc["features"][i]["devices"][j]["time.SECONDS"];
                time["ON_TIME"] = originalFeaturesDoc["features"][i]["devices"][j]["time.ON_TIME"];
                time["OFF_TIME"] = originalFeaturesDoc["features"][i]["devices"][j]["time.OFF_TIME"];
            }
        }

        // Serialize JSON to file
        if (serializeJson(jsonDoc, file) == 0)
        {
            this->logger->log("[FEATURE_MAPPER]:[ERR]: Failed to write contol file");
        }

        file.close();
    }

private:
    LabLogger *logger;
};
#endif