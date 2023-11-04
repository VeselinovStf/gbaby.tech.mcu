#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "utility/log/lab_logger.h"

enum AppConfigRefreshState
{
    NONE = 0,
    SPIFFS_REFRESH = 1
};

/*
    Main Configuration of lifecycle of procedures
*/
enum Lifecycle
{
    /*
       Normal loop
   */
    NORMAL = 0,
    /*
        Flaggs nead of free clock time to process data
    */
    FREE_CLOCK = 1,
    /*
        Restart Device state
    */
    RESTART_TASK = 3,
    /*
        Notification state
    */
    NOTIFICATION_RUN = 4,
    /*
        Notification state
    */
    SD_UPDATE_TASK = 5
};

typedef struct
{
    String INPUT_SSID = "A1_01QW2";
    String INPUT_PASSWORD = "12345678";
    String AP_IP = "12345678";
    String STA_IP = "";
    String STA_SUBNETMASK = "255.255.255.0";
    String AP_SUBNETMASK = "255.255.255.0";
    String STA_GETEWAY = "192.168.100.1";
    String AP_GETEWAY = "192.168.100.1";
    uint8_t IS_CONFIRMED = false;
    uint8_t REFRESH_STATE = AppConfigRefreshState::NONE;
    uint8_t WIFI_STA_ENABLED = true;
    const char *APP_CONFIG_FILE_PATH = "/data/app.json";
    const char *APP_LOG_FILE_PATH = "/data/logs/logs.txt";
    const char *APP_FEATURES_DATA_FILE_PATH = "/data/features.json";
    const char *APP_CONTROL_FILE_PATH = "/data/control.json";
    uint8_t Lifecycle = Lifecycle::NORMAL;
    String UPLOAD_SD_FILE_PATH = "";
} app_config_t;

class AppConfigMapper
{
public:
    AppConfigMapper(LabLogger *logger)
    {
        this->logger = logger;
    }

    void MapFromStorage(fs::FS &fs, const char *datafile, app_config_t *app_config)
    {
        // Open file for reading
        File file = fs.open(datafile, FILE_READ);

        // Allocate a temporary JsonDocument
        StaticJsonDocument<4042> doc;

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(doc, file);

        if (error)
            this->logger->log("[ASSM]:[ERR]: Failed to read file, using default configuration");

        // // Copy values from the JsonDocument to the Config

        app_config->INPUT_SSID = doc["INPUT_SSID"].as<String>();
        app_config->INPUT_PASSWORD = doc["INPUT_PASSWORD"].as<String>();
        app_config->IS_CONFIRMED = doc["IS_CONFIRMED"].as<uint8_t>();
        app_config->REFRESH_STATE = doc["REFRESH_STATE"].as<uint8_t>();
        app_config->WIFI_STA_ENABLED = doc["WIFI_STA_ENABLED"].as<uint8_t>();
        app_config->AP_IP = doc["AP_IP"].as<String>();
        app_config->STA_IP = doc["STA_IP"].as<String>();
        app_config->STA_SUBNETMASK = doc["STA_SUBNETMASK"].as<String>();
        app_config->AP_SUBNETMASK = doc["AP_SUBNETMASK"].as<String>();
        app_config->STA_GETEWAY = doc["STA_GETEWAY"].as<String>();
        app_config->AP_GETEWAY = doc["AP_GETEWAY"].as<String>();

        // app_config->APP_LOG_FILE_PATH = doc["APP_LOG_FILE_PATH"].as<const *>();
        // app_config->APP_CONFIG_FILE_PATH = doc["APP_CONFIG_FILE_PATH"].as<String>();
        // app_config->APP_FEATURES_DATA_FILE_PATH = doc["APP_FEATURES_DATA_FILE_PATH"].as<uint8_t>();

        file.close();
    }

    void MapToStorage(const char *datafile, app_config_t *app_config)
    {
        // Open file for writing
        File file = SD.open(datafile, FILE_WRITE);
        if (!file)
        {
            this->logger->log("[ASSM]:[ERR]: Failed to create file");
            return;
        }

        // Allocate a temporary JsonDocument
        StaticJsonDocument<4042> doc;

        doc["INPUT_SSID"] = app_config->INPUT_SSID;
        doc["INPUT_PASSWORD"] = app_config->INPUT_PASSWORD;
        doc["WIFI_STA_ENABLED"] = app_config->WIFI_STA_ENABLED;
        doc["IS_CONFIRMED"] = app_config->IS_CONFIRMED;
        doc["REFRESH_STATE"] = app_config->REFRESH_STATE;
        doc["UPLOAD_SD_FILE_PATH"] = app_config->UPLOAD_SD_FILE_PATH;
        doc["AP_IP"] = app_config->AP_IP;
        doc["STA_IP"] = app_config->STA_IP;
        doc["AP_SUBNETMASK"] = app_config->AP_SUBNETMASK;
        doc["STA_SUBNETMASK"] = app_config->STA_SUBNETMASK;
        doc["STA_GETEWAY"] = app_config->STA_GETEWAY;
        doc["AP_GETEWAY"] = app_config->AP_GETEWAY;

        // Serialize JSON to file
        if (serializeJson(doc, file) == 0)
        {
            this->logger->log("[ASSM]:[ERR]: Failed to write to file");
        }

        // Close the file
        file.close();
    }

private:
    LabLogger *logger;
};
#endif