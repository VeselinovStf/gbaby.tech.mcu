#ifndef SERVE_UPDATE_REQUEST_HANDLER_H
#define SERVE_UPDATE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"

class ServeUpdateRequestHandler : public AsyncWebHandler
{
public:
    ServeUpdateRequestHandler(LabLogger *logger) { this->logger = logger; }

    virtual ~ServeUpdateRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_UPDATE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetUpdate]:[INF]: [SERVE] /updates");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif