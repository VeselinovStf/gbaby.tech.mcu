#ifndef GET_LOGS_REQUEST_HANDLER_H
#define GET_LOGS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class GetLogsRequestHandler : public AsyncWebHandler
{
public:
    GetLogsRequestHandler(app_config_t *app_setting, LabLogger *logger)
    {
        this->app_setting = app_setting;
        this->logger = logger;
    }

    virtual ~GetLogsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.GET_LOGS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetLogs]:[INF]: [HTTP_GET] /get-logs");

        request->send(SD, "/logs/logs.", "application/json");
    }

private:
    Routes routes;
    app_config_t *app_setting;
    LabLogger *logger;
};

#endif