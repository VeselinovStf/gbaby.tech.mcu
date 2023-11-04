#ifndef SERVE_TELEGRAM_NOTICATIONS_REQUEST_HANDLER_H
#define SERVE_TELEGRAM_NOTICATIONS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeTelegramNoticifationsRequestHandler : public AsyncWebHandler
{
public:
    ServeTelegramNoticifationsRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeTelegramNoticifationsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_TELEGRAM_NOTIFICATIONS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeTelegramNoticifations]:[INF]: [SERVE] /telegram-notifications");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif