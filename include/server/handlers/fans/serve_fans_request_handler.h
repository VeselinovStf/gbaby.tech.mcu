#ifndef SERVE_FANS_REQUEST_HANDLER_H
#define SERVE_FANS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class GetFansRequestHandler : public AsyncWebHandler
{
public:
    GetFansRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~GetFansRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_FANS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetFans]:[INF]: [SERVE] /fans");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif