#ifndef SERVE_CREDENTIALS_REQUEST_HANDLER_H
#define SERVE_CREDENTIALS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeCredentialsRequestHandler : public AsyncWebHandler
{
public:
    ServeCredentialsRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeCredentialsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_CREDENTIALS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeCredentials]:[INF]: [SERVE] /");

        request->send(SD, "/data/ui/index", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif