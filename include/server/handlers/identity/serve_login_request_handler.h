#ifndef SERVE_LOGIN_REQUEST_HANDLER_H
#define SERVE_LOGIN_REQUEST_HANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "utility/log/lab_logger.h"
#include "server/routes.h"

class ServeLoginRequestHandler : public AsyncWebHandler
{
public:
    ServeLoginRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeLoginRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_LOGIN_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeLogin]:[INF]: [SERVE] /login");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

    private:
        Routes routes;
        LabLogger *logger;
};

#endif