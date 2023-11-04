#ifndef RESTART_DEVICE_STATE_REQUEST_HANDLER_H
#define RESTART_DEVICE_STATE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"
#include <SD.h>

class RestartDeviceStateRequestHandler : public AsyncWebHandler
{
public:
    RestartDeviceStateRequestHandler(LabLogger *logger, app_control_t *app_control, app_config_t *app_config, AppConfigMapper *app_config_mapper)
    {
        this->logger = logger;
        this->app_control = app_control;
        this->app_config = app_config;
        this->app_config_mapper = app_config_mapper;
    }

    virtual ~RestartDeviceStateRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.RESTART_DEVICE_STATE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[RestartDevice]:[INF]: [HTTP_GET] /restart-device-state");

        this->app_config->Lifecycle = Lifecycle::FREE_CLOCK;
        this->app_config->REFRESH_STATE = AppConfigRefreshState::SPIFFS_REFRESH; 
        
        // TODO: #94 Endpoint in witch you can pass only json settings files OR edit them on situ - in UI

        this->app_config_mapper->MapToStorage("/data/app.js", this->app_config);

        // TODO: #50 UI Create Succes Restart Page
        request->redirect("/change-success");
    }

private:
    Routes routes;
    LabLogger *logger;
    app_control_t *app_control;
    app_config_t *app_config;
    AppConfigMapper *app_config_mapper;
};

#endif