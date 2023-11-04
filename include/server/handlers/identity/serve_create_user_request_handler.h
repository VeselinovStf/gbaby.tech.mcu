#ifndef SERVE_CREATE_REQUEST_HANDLER_H
#define SERVE_CREATE_REQUEST_HANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "utility/log/lab_logger.h"
#include "server/routes.h"

class ServeCreateUserRequestHandler : public AsyncWebHandler
{
public:
    ServeCreateUserRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeCreateUserRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_CREATE_USER_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeCreateUser]:[INF]: [SERVE] /create-user");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

    private:
        Routes routes;
        LabLogger *logger;
};

#endif