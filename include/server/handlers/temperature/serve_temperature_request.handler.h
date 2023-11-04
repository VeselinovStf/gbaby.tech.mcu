#ifndef SERVE_TEMPERATURE_REQUEST_HANDLER_H
#define SERVE_TEMPERATURE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeTemperatureRequestHandler : public AsyncWebHandler
{
public:
    ServeTemperatureRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeTemperatureRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_TEMPERATURE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetTemperature]:[INF]: [SERVE] /temperature");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif