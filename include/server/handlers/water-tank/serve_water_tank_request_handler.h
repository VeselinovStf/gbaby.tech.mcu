#ifndef SERVE_WATER_TANK_REQUEST_HANDLER_H
#define SERVE_WATER_TANK_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeWaterTankRequestHandler : public AsyncWebHandler
{
public:
    ServeWaterTankRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeWaterTankRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_WATER_TANK_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeWater]:[INF]: [SERVE] /tankLevel");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif