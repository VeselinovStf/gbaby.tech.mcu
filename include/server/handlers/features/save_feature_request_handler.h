#ifndef SAVE_FEATURE_REQUEST_HANDLER_H
#define SAVE_FEATURE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "core/app_feature.h"

#include "server/core/web_app_server.h"
#include "core/app_control.h"
#include "rules/rule_types.h"

#include "ESP32Time.h"

class SaveFeatureRequestHandler : public AsyncWebHandler
{
public:
    SaveFeatureRequestHandler(
        AppFeatureMapper *app_feature_mapper,
        LabLogger *logger,
        ESP32Time *rtc,
        action_feature_t *app_feature,
        app_config_t *app_config,
        app_control_t *app_control)
    {
        this->app_feature_mapper = app_feature_mapper;
        this->logger = logger;
        this->rtc = rtc;
        this->app_feature = app_feature;
        this->app_config = app_config;
        this->app_control = app_control;
    }

    virtual ~SaveFeatureRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SAVE_FEATURE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[SaveFeature]:[INF]: [HTTP_POST] /save-as-feature");

        // Name is required
        if (request->hasParam("name"))
        {
            this->logger->log("[HANDLER]:[INF]: Parsing Feature Configuration");

            int currentHour = this->rtc->getHour();
            int currentMinutes = this->rtc->getMinute();
            int currentSeconds = this->rtc->getSecond();
            int currentTimeCalculation = ((currentHour * 60) * 60) + (currentMinutes * 60) + currentSeconds;

            // app_control_t featureState;
            action_feature_t new_feature;
            control_t featureState;

            // Parse new_feature state from request
            featureState.time.HOUR = currentHour;
            featureState.time.MINUTES = currentMinutes;
            featureState.time.SECONDS = currentSeconds;

            new_feature.NAME = request->getParam("name")->value();

            this->logger->log("[HANDLER]:[INF]: Saving new Feature!");
            this->logger->log(new_feature.NAME.c_str());

            // Add to features.json
            this->app_feature_mapper->UpdateToStorage("/data/features.json", this->app_control, &new_feature, &featureState);

            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(404, "text/plain", "Provide Feature Name");
        }
    }

private:
    Routes routes;
    AppFeatureMapper *app_feature_mapper;
    LabLogger *logger;
    ESP32Time *rtc;
    action_feature_t *app_feature;
    app_config_t *app_config;
    app_control_t *app_control;
};

#endif