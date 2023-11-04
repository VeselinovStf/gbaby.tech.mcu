#ifndef SET_CREDENTIALS_REQUEST_HANDLER_H
#define SET_CREDENTIALS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"
#include "server/core/web_app_server.h"

typedef struct{
    const char *PARAM_SSID = "ssidInput";
    const char *PARAM_WIFI_PASS = "wifiPasswordInput";
    const char *PARAM_WIFI_STA_ENABLED = "stationMode";
} set_credentials_request_model_t;

class SetCredentialsRequestHandler : public AsyncWebHandler
{
public:
    SetCredentialsRequestHandler(app_config_t *app_config, AppConfigMapper *app_config_mapper, LabLogger *logger)
    {
        this->app_config = app_config;
        this->logger = logger;
        this->app_config_mapper = app_config_mapper;
    }

    virtual ~SetCredentialsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SET_CREDENTIALS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[SetCredentials]:[INF]: [HTTP_GET] /authenticate");

        String inputMessage;

        // TODO: Validations
        // INPUT_SSID
        if (request->hasParam(this->request_model.PARAM_SSID))
        {
            this->app_config->INPUT_SSID = (char *)request->getParam(this->request_model.PARAM_SSID)->value().c_str();
        }

        // INPUT_PASSWORD
        if (request->hasParam(this->request_model.PARAM_WIFI_PASS))
        {
            this->app_config->INPUT_PASSWORD = (char *)request->getParam(this->request_model.PARAM_WIFI_PASS)->value().c_str();
        }

        // STATION MODE
        if (request->hasParam(this->request_model.PARAM_WIFI_STA_ENABLED))
        {
            if (request->getParam(this->request_model.PARAM_WIFI_STA_ENABLED)->value() == "wifi")
            {
                this->logger->log("[CRED]:[INFO]: User Select STA MODE");
                this->app_config->WIFI_STA_ENABLED = true;
            }
            else
            {
                this->logger->log("[CRED]:[INFO]: User Select AP MODE");
                this->app_config->WIFI_STA_ENABLED = false;
            }
        }
        else
        {
            this->logger->log("[CRED]:[INFO]: User Not Selected WIFI MODE - STA MODE");
            this->app_config->WIFI_STA_ENABLED = true;
        }

        this->app_config->IS_CONFIRMED = true;

        this->app_config_mapper->MapToStorage(this->app_config->APP_CONFIG_FILE_PATH, this->app_config);

        // TODO: #5 debugger(IP); Return as result
        request->redirect("/credential-applied");

        vTaskDelay(3000);

        esp_restart();
    }

private:
    Routes routes;
    app_config_t *app_config;
    LabLogger *logger;
    set_credentials_request_model_t request_model;
    AppConfigMapper *app_config_mapper;
};

#endif