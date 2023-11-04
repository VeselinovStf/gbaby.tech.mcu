#ifndef RESTART_DEVICE_REQUEST_HANDLER_H
#define RESTART_DEVICE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>

class RestartDeviceSettingsRequestHandler : public AsyncWebHandler
{
public:
    RestartDeviceSettingsRequestHandler(LabLogger *logger, app_config_t *app_config)
    {
        this->logger = logger;
        this->app_config = app_config;
    }

    virtual ~RestartDeviceSettingsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.RESTART_DEVICE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[RestartDevice]:[INF]: [HTTP_GET] /restart-device");

        this->app_config->Lifecycle = Lifecycle::RESTART_TASK;

        // TODO: #50 UI Create Succes Restart Page
        // BUG: #55 Return Response to onother page
        request->send(200, "text/plain", "OK");
    }

private:
    Routes routes;
    LabLogger *logger;
    app_config_t *app_config;
};

#endif