#ifndef SERVE_CHANGE_SUCCESS_REQUEST_HANDLER_H
#define SERVE_CHANGE_SUCCESS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeChangeSuccessRequestHandler : public AsyncWebHandler
{
public:
    ServeChangeSuccessRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }
    virtual ~ServeChangeSuccessRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_CHANGE_SUCCESS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetPrivacy]:[INF]: [SERVE] /change-success");

        request->send(SD, "/data/ui/change-success.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif