#ifndef SERVE_INDEX_REQUEST_HANDLER_H
#define SERVE_INDEX_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"

class ServeIndexRequestHandler : public AsyncWebHandler
{
public:
    ServeIndexRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }

    virtual ~ServeIndexRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_HOME_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[Index]:[INFO]: [SERVE] /");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif