#ifndef RESTART_CREDENTIALS_REQUEST_HANDLER_H
#define RESTART_CREDENTIALS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "core/app_config.h"

class RestartCredentialsSettingsRequestHandler : public AsyncWebHandler
{
public:
    RestartCredentialsSettingsRequestHandler(app_config_t *app_config, AppConfigMapper *app_config_mapper, LabLogger *logger)
    {
        this->app_config = app_config;
        this->app_config_mapper = app_config_mapper;
        this->logger = logger;
    }

    virtual ~RestartCredentialsSettingsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.RESTART_CREDENTIALS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[RestartCredentials]:[INF]: [HTTP_GET] /restart-credentials");

        this->app_config->INPUT_SSID = "A1_01QW2"; // TODO: Move them to consts - or get them from credentials
        this->app_config->INPUT_PASSWORD = "12345678";
        this->app_config->IS_CONFIRMED = 0;
        this->app_config->WIFI_STA_ENABLED = 0;

        this->app_config_mapper->MapToStorage(this->app_config->APP_CONFIG_FILE_PATH, this->app_config);

        this->logger->shutDown();

        esp_restart();

        // TODO: #50 UI Create Succes Restart Page
        request->send(200, "text/plain", "OK");
    }

private:
    Routes routes;
    app_config_t *app_config;
    AppConfigMapper *app_config_mapper;
    LabLogger *logger;
};

#endif