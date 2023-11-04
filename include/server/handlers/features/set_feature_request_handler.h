#ifndef SET_FEATURE_REQUEST_HANDLER_H
#define SET_FEATURE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>

#include "server/core/web_app_server.h"
#include "core/app_control.h"
#include "rules/rule_types.h"

#include "ESP32Time.h"

class SetFeatureRequestHandler : public AsyncWebHandler
{
public:
    SetFeatureRequestHandler(action_feature_t *app_feature, app_control_t *app_control, AppControlMapper *app_control_mapper, AppFeatureMapper *app_feature_mapper, LabLogger *logger, ESP32Time *rtc)
    {
        this->app_control = app_control;
        this->app_control_mapper = app_control_mapper;
        this->logger = logger;
        this->rtc = rtc;
        this->app_feature_mapper = app_feature_mapper;
        this->app_feature = app_feature;
    }

    virtual ~SetFeatureRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SET_FEATURE_DATA_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[SetFeature]:[INF]: [HTTP_POST] /set-feature");

        if (request->hasParam("id"))
        {
            this->logger->log("[SET_FEATURE]:[INF]: Parsing Feature Configuration");

            int currentHour = this->rtc->getHour();
            int currentMinutes = this->rtc->getMinute();
            int currentSeconds = this->rtc->getSecond();
            int currentTimeCalculation = ((currentHour * 60) * 60) + (currentMinutes * 60) + currentSeconds;

            long id = request->getParam("id")->value().toInt();
            control_t new_function_state;

            bool haveChanges = false;
            if (request->hasParam("name"))
            {
                this->app_feature->NAME = request->getParam("name")->value();
                haveChanges = true;
            }


            if (request->hasParam("status"))
            {
                new_function_state.status = request->getParam("status")->value().toInt();
                haveChanges = true;
            }

            if (request->hasParam("rule"))
            {
                new_function_state.rule = request->getParam("rule")->value().toInt();
                haveChanges = true;
            }

            if (request->hasParam("unit"))
            {
                new_function_state.unit = request->getParam("unit")->value();
                haveChanges = true;
            }

            if (request->hasParam("onWhenRule"))
            {
                new_function_state.onWhenRule = request->getParam("onWhenRule")->value().toInt();
                haveChanges = true;
            }

            if (request->hasParam("onWhenCondition"))
            {
                new_function_state.onWhenCondition = request->getParam("onWhenCondition")->value().toInt();
                haveChanges = true;
            }

            if (request->hasParam("onWhenValue"))
            {
                long value = request->getParam("onWhenValue")->value().toInt();

                // Set new value
                new_function_state.onWhenValue = value;

                // Time based: SET ON_TIME
                if (new_function_state.onWhenRule == RuleType::TIME_RULE)
                {
                    new_function_state.time.ON_TIME = currentTimeCalculation + value;
                }
                haveChanges = true;
            }

            if (request->hasParam("offWhenRule"))
            {
                new_function_state.offWhenRule = request->getParam("offWhenRule")->value().toInt();
                haveChanges = true;
            }

            if (request->hasParam("offWhenCondition"))
            {
                new_function_state.offWhenCondition = request->getParam("offWhenCondition")->value().toInt();
                haveChanges = true;
            }

            if (request->hasParam("offWhenValue"))
            {
                long value = request->getParam("offWhenValue")->value().toInt();

                // Change from original value
                // Set new value
                new_function_state.offWhenValue = value;

                // Time based: SET OFF_TIME
                if (new_function_state.offWhenRule == RuleType::TIME_RULE)
                {
                    // RTC time + On time = The exact time when to stop
                    new_function_state.time.OFF_TIME = currentTimeCalculation + new_function_state.onWhenValue;
                }
                haveChanges = true;
            }

            Serial.println(new_function_state.status);
            if (new_function_state.status == 1)
            {
                this->app_feature_mapper->MapFeature(SD, id, "/data/features.json", this->app_control);

                this->app_control_mapper->MapToStorage(SD, "/data/control.json", this->app_control);
            }
            else
            {
                this->app_control_mapper->RestorToDefaultStorage(SD, "/data/control.json", this->app_control);
            }

            this->app_feature_mapper->MapToStorage(SD, id, "/data/features.json", &new_function_state, this->app_feature);

            // Map Function to State
            for (uint8_t i = 0; i < AppFunctions::TOTAL_CORE_FUNCTIONS; i++)
            {
                this->app_control->CONTROLS[i].time.HOUR = currentHour;
                this->app_control->CONTROLS[i].time.MINUTES = currentMinutes;
                this->app_control->CONTROLS[i].time.SECONDS = currentSeconds;
            }

            this->logger->log("[SET_FEATURE]:[INF]: Saving CONTROL FILE Storage");

            // Update main control
            this->app_control_mapper->MapToStorage(SD, "/data/control.json", this->app_control);

            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(404, "text/plain", "Provide Feature Name");
        }
    }

private:
    Routes routes;
    app_control_t *app_control;
    AppControlMapper *app_control_mapper;
    LabLogger *logger;
    ESP32Time *rtc;
    AppFeatureMapper *app_feature_mapper;
    action_feature_t *app_feature;
};

#endif