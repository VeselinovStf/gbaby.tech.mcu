#ifndef SERVE_TOC_REQUEST_HANDLER_H
#define SERVE_TOC_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeTocRequestHandler : public AsyncWebHandler
{
public:
    ServeTocRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeTocRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_TOC_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetToc]:[INF]: [SERVE] /toc");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif