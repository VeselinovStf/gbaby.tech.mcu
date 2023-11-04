#ifndef SERVE_CREDENTIALS_APPLIED_REQUEST_HANDLER_H
#define SERVE_CREDENTIALS_APPLIED_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeCredentialsAppliedRequestHandler : public AsyncWebHandler
{
public:
    ServeCredentialsAppliedRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeCredentialsAppliedRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_CREDENTIALS_APPLIED_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeCredentialsApplied]:[INF]: [HTTP_GET] /credential-applied");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif