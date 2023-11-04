#ifndef APP_HISTORY_H
#define APP_HISTORY_H

#include <Arduino.h>
#include "utility/log/lab_logger.h"
#include <ArduinoJson.h>
#include <ESP32Time.h>

class AppHistory
{
public:
    AppHistory(LabLogger *logger, const char *basepath, ESP32Time *rtc,  AppControlMapper *app_control_mapper)
    {
        this->logger = logger;
        this->basepath = basepath;
        this->rtc = rtc;
        this->app_control_mapper = app_control_mapper;
    };

    void save(fs::FS &fs, String path, app_control_t *app_control)
    {
        this->logger->log("[AppHistory]:[INF]: Adding history");

        this->app_control = app_control;

        int day = this->rtc->getDay();
        int month = this->rtc->getMonth();
        int year = this->rtc->getYear();
        
        String newRecordPath = this->basepath +
                               ((String)day) + "_" +
                               ((String)month) + "_" +
                               ((String)year) + "_" + path + ".json";

        // this->copyFileOn(fs, "/data/control.json", newRecordPath.c_str());
        this->addHistory(fs, newRecordPath.c_str());
    };

private:
    LabLogger *logger;
    const char *basepath;
    ESP32Time *rtc;
    AppControlMapper *app_control_mapper;
    app_control_t *app_control;

    void addHistory(fs::FS fs, const char *destinationPath)
    {

        File file;
        if (!fs.exists(destinationPath))
        {
            file = fs.open(destinationPath, FILE_WRITE);
        }
        else
        {
            file = fs.open(destinationPath, FILE_APPEND);
        }

        String control_data = this->app_control_mapper->MapToString(this->app_control);
        String message =
            ((String)this->rtc->getYear()) + "/" +
            ((String)this->rtc->getMonth()) + "/" +
            ((String)this->rtc->getDay()) + "/" +
            ((String)this->rtc->getHour()) + "/" +
            ((String)this->rtc->getMinute()) + "/" +
            ((String)this->rtc->getSecond()) + "/" + control_data;

        file.println(message);

        file.close();
    }
};

#endif