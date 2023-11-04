#ifndef SERVE_HYDRO_REQUEST_HANDLER_H
#define SERVE_HYDRO_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class GetHydroRequestHandler : public AsyncWebHandler
{
public:
    GetHydroRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~GetHydroRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_HYDRO_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetHydro]:[INF]: [SERVE] /hydro");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif