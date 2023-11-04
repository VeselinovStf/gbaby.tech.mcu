#ifndef SERVE_LIGHTS_REQUEST_HANDLER_H
#define SERVE_LIGHTS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class GetLightsRequestHandler : public AsyncWebHandler
{
public:
    GetLightsRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~GetLightsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_LIGHTS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetLights]:[INF]: [SERVE] /lights");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif