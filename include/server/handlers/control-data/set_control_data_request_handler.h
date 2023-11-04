#ifndef SET_CONTROL_DATA_REQUEST_HANDLER_H
#define SET_CONTROL_DATA_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include <FS.h>

#include "server/core/web_app_server.h"
#include "core/app_control.h"
#include "rules/rule_types.h"

#include "ESP32Time.h"

class SetControlDataRequestHandler : public AsyncWebHandler
{
public:
    SetControlDataRequestHandler(fs::FS &fs, app_config_t *app_config, app_control_t *app_control, AppControlMapper *app_control_mapper, LabLogger *logger, ESP32Time *rtc) : fs(fs)
    {
        this->app_control = app_control;
        this->app_control_mapper = app_control_mapper;
        this->logger = logger;
        this->rtc = rtc;
        this->app_config = app_config;
    }

    virtual ~SetControlDataRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SET_CONTROL_DATA_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[SetControl]:[INF]: [HTTP_POST] /control-data");

        int currentHour = this->rtc->getHour();
        int currentMinutes = this->rtc->getMinute();
        int currentSeconds = this->rtc->getSecond();
        int currentTimeCalculation = ((currentHour * 60) * 60) + (currentMinutes * 60) + currentSeconds;     

        bool haveChanges = false;
        for (uint8_t i = 0; i < AppFunctions::TOTAL_CORE_FUNCTIONS; i++)
        {
            // Save changes only if unit param is preset!
            if (request->getParam("unit")->value() == this->app_control->CONTROLS[i].unit)
            {
                this->app_control->CONTROLS[i].time.HOUR = currentHour;
                this->app_control->CONTROLS[i].time.MINUTES = currentMinutes;
                this->app_control->CONTROLS[i].time.SECONDS = currentSeconds;

                // TODO: #67 Check if this can be moved in RequestParser
                if (request->hasParam("status"))
                {
                    this->app_control->CONTROLS[i].status = request->getParam("status")->value().toInt();
                    haveChanges = true;
                }

                if (request->hasParam("rule"))
                {
                    this->app_control->CONTROLS[i].rule = request->getParam("rule")->value().toInt();
                    haveChanges = true;
                }

                if (request->hasParam("unit"))
                {
                    this->app_control->CONTROLS[i].unit = request->getParam("unit")->value();
                    haveChanges = true;
                }

                if (request->hasParam("onWhenRule"))
                {
                    this->app_control->CONTROLS[i].onWhenRule = request->getParam("onWhenRule")->value().toInt();
                    haveChanges = true;
                }

                if (request->hasParam("onWhenCondition"))
                {
                    this->app_control->CONTROLS[i].onWhenCondition = request->getParam("onWhenCondition")->value().toInt();
                    haveChanges = true;
                }

                if (request->hasParam("onWhenValue"))
                {
                    long value = request->getParam("onWhenValue")->value().toInt();

                    // Set new value
                    this->app_control->CONTROLS[i].onWhenValue = value;

                    // Time based: SET ON_TIME
                    if (this->app_control->CONTROLS[i].onWhenRule == RuleType::TIME_RULE)
                    {
                        this->app_control->CONTROLS[i].time.ON_TIME = currentTimeCalculation + value;
                    }
                    haveChanges = true;
                }

                if (request->hasParam("offWhenRule"))
                {
                    this->app_control->CONTROLS[i].offWhenRule = request->getParam("offWhenRule")->value().toInt();
                    haveChanges = true;
                }

                if (request->hasParam("offWhenCondition"))
                {
                    this->app_control->CONTROLS[i].offWhenCondition = request->getParam("offWhenCondition")->value().toInt();
                    haveChanges = true;
                }

                if (request->hasParam("offWhenValue"))
                {
                    long value = request->getParam("offWhenValue")->value().toInt();

                    // Change from original value
                    // Set new value
                    this->app_control->CONTROLS[i].offWhenValue = value;

                    // Time based: SET OFF_TIME
                    if (this->app_control->CONTROLS[i].offWhenRule == RuleType::TIME_RULE)
                    {
                        // RTC time + On time = The exact time when to stop
                        this->app_control->CONTROLS[i].time.OFF_TIME = currentTimeCalculation + this->app_control->CONTROLS[i].onWhenValue;
                    }
                    haveChanges = true;
                }

                break;
            }
        }

        if (haveChanges){
            this->logger->log("[SET_CONTROL]:[INF]: Saving CONTROL FILE Storage");

            this->app_control_mapper->MapToStorage(SD, this->app_config->APP_CONTROL_FILE_PATH, this->app_control);
        }
          
        request->send(200, "text/plain", "OK");
    }

private:
    Routes routes;
    app_control_t *app_control;
    AppControlMapper *app_control_mapper;
    LabLogger *logger;
    ESP32Time *rtc;
    fs::FS &fs;
    app_config_t *app_config;
};

#endif