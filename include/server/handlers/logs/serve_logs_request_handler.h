#ifndef SERVE_LOGS_REQUEST_HANDLER_H
#define SERVE_LOGS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeLogsRequestHandler : public AsyncWebHandler
{
public:
    ServeLogsRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeLogsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_LOGS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeLogs]:[INF]: [SERVE] /logs");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif