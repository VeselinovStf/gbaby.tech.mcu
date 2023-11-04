#ifndef APP_NOTIFICATION_H
#define APP_NOTIFICATION_H

#include <Arduino.h>
#include "core/app_functions.h"
#include <FS.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "utility/log/lab_logger.h"
#include "notifications/notifier.h"

enum Notification
{
    /*
        Telegram notification flag
    */
    TELEGRAM_NOTIFICATION = 0,
};

enum NotificationState
{
    OFF_NOTIFICATION = 0,
    ON_NOTIFICATION = 1,
};

typedef struct
{
    String API_KEY;
    String ID;
    uint8_t status;
    String name;
    int delay;
} notification_t;

typedef struct
{
    notification_t notifications[TOTAL_NOTIFICATIONS] = {
        {"","",0,"telegram"}
    };
} app_notification_t;

class AppNotificationMapper
{
public:
    AppNotificationMapper(LabLogger *logger)
    {
        this->logger = logger;
    };

    void MapFromStorage(fs::FS &fs, String type, notification_t *notification)
    {
        String datafile = "/data/" + type + "-notifications.json";

        // Open file for writing
        if (!SD.exists(datafile))
        {
            this->logger->log("[AppNotificationMapper]:[ERR]: Can't find contol file");
            return;
        }

        File original = SD.open(datafile, FILE_READ);
        String jsonString = original.readString();
        DynamicJsonDocument jsonDoc(2024); // Adjust buffer size as needed

        this->logger->log("[AppNotificationMapper]:[INF]: Deserializing Control json");

        // Deserialize the JSON data from the string
        DeserializationError error = deserializeJson(jsonDoc, jsonString);

        if (error)
        {
            this->logger->log("[AppNotificationMapper]:[ERR]: Failed to deserialize JSON");
            return;
        }

        notification->API_KEY = jsonDoc["config"]["api_key"].as<String>();
        notification->ID = jsonDoc["config"]["id"].as<String>();
        notification->status = jsonDoc["config"]["status"];
        notification->name = jsonDoc["config"]["name"].as<String>();
        notification->delay = jsonDoc["config"]["delay"].as<int>();

        // Close the file
        original.close();
    }

    void MapToStorage(fs::FS &fs, String type, notification_t *notification)
    {
        String datafile = "/" + type + "-notifications.json";

        // Open file for writing
        File file = SD.open(datafile, FILE_WRITE);
        if (!file)
        {
            this->logger->log("[AppNotificationMapper]:[ERR]: Failed to create file");
            return;
        }

        // Allocate a temporary JsonDocument
        StaticJsonDocument<2024> doc;

        doc["config"]["api_key"] = notification->API_KEY;
        doc["config"]["id"] = notification->ID;
        doc["config"]["status"] = notification->status;
        doc["config"]["delay"] = notification->delay;

        // Serialize JSON to file
        if (serializeJson(doc, file) == 0)
        {
            this->logger->log("[AppNotificationMapper]:[ERR]: Failed to write to file");
        }

        // Close the file
        file.close();
    }

    String MapToString(fs::FS &fs, const char *datafile, notification_t *notification)
    {
        DynamicJsonDocument jsonDoc(3072); // Adjust the buffer size as needed

        JsonObject limits = jsonDoc.createNestedObject("config");

        limits["api_key"] = notification->API_KEY;
        limits["id"] = notification->ID;
        limits["status"] = notification->status;
        limits["name"] = notification->name;
        limits["delay"] = notification->delay;

        String jsonString;

        // Serialize JSON to file
        if (serializeJson(jsonDoc, jsonString) == 0)
        {
            this->logger->log("[AppNotification]:[ERR]: Failed to serialize to string");
        }

        // TODO: Add UI to Backend
        // debugger(jsonString);

        return jsonString;
    }

private:
    LabLogger *logger;
};

#endif