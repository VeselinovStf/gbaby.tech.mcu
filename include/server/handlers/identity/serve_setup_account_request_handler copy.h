#ifndef SERVE_SETUP_ACCOUNT_REQUEST_HANDLER_H
#define SERVE_SETUP_ACCOUNT_REQUEST_HANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include "utility/log/lab_logger.h"
#include "server/routes.h"

class ServeSetupAccountRequestHandler : public AsyncWebHandler
{
public:
    ServeSetupAccountRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeSetupAccountRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_SETUP_ACCOUNT_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeCreateUser]:[INF]: [SERVE] /setup-account");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif