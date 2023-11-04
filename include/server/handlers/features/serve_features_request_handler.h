#ifndef SERVE_FEATURE_REQUEST_HANDLER_H
#define SERVE_FEATURE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "core/app_feature.h"

#include <SD.h>

#include "server/core/web_app_server.h"
#include "core/app_control.h"

class ServeFeatureRequestHandler : public AsyncWebHandler
{
public:
    ServeFeatureRequestHandler(app_config_t *app_config, app_control_t *app_control, AppFeatureMapper *app_feature_mapper, LabLogger *logger, action_feature_t *action_feature)
    {
        this->app_config = app_config;
        this->app_control = app_control;
        this->app_feature_mapper = app_feature_mapper;
        this->logger = logger;
        this->action_feature = action_feature;
    }

    virtual ~ServeFeatureRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_FEATURE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetFeature]:[INF]: [SERVE] /get-features");

        // TODO: #58 Return dissabled property to UI ellement while is disabled it must be hiden for edit
        // request->send(200, "application/json", this->app_feature_mapper->MapToString(this->app_config->APP_FEATURES_DATA_FILE_PATH, this->app_control, this->action_feature));
        request->send(SD, "/data/features.json", "application/json");
    }

private:
    Routes routes;
    app_config_t *app_config;
    AppFeatureMapper *app_feature_mapper;
    LabLogger *logger;
    app_control_t *app_control;
    action_feature_t *action_feature;
};

#endif