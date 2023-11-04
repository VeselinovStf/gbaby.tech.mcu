#ifndef GET_NOTIFICATIONS_REQUEST_HANDLER_H
#define GET_NOTIFICATIONS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class GetNotificationsRequestHandler : public AsyncWebHandler
{
public:
    GetNotificationsRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }

    virtual ~GetNotificationsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.GET_NOTIFICATIONS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetNotifications]:[INF]: [HTTP_GET] /get-notifications");

        if (request->hasParam("name"))
        {
            String type = "/" + request->getParam("name")->value() + "-notifications.json";

            request->send(SD, type, "application/json");
        }
        else
        {
            request->send(404, "Invalid Request");
        }
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif