#ifndef SERVE_SETTINGS_REQUEST_HANDLER_H
#define SERVE_SETTINGS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeSettingsRequestHandler : public AsyncWebHandler
{
public:
    ServeSettingsRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeSettingsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_SETTINGS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetSettings]:[INF]: [SERVE] /settings");

        // TODO: #49 UI Creat Settings page
        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif