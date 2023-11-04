#ifndef SERVE_PRIVACY_REQUEST_HANDLER_H
#define SERVE_PRIVACY_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServePrivacyRequestHandler : public AsyncWebHandler
{
public:
    ServePrivacyRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }
    virtual ~ServePrivacyRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_PRIVACY_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetPrivacy]:[INF]: [SERVE] /privacy");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif