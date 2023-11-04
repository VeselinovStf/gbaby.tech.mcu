#ifndef SERVE_ABOUT_REQUEST_HANDLER_H
#define SERVE_ABOUT_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"

class ServeAboutRequestHandler : public AsyncWebHandler
{
public:
    ServeAboutRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }

    virtual ~ServeAboutRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_ABOUT_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeTOCR]:[INFO]: [SERVE] /about");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif