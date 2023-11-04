#ifndef SERVE_CONTROL_REQUEST_HANDLER_H
#define SERVE_CONTROL_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeControlRequestHandler : public AsyncWebHandler
{
public:
    ServeControlRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeControlRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_CONTROL_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetControl]:[INF]: [SERVE] /control");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif