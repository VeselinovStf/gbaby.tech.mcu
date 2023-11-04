#ifndef SERVE_DASHBOARD_REQUEST_HANDLER_H
#define SERVE_DASHBOARD_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeDashboardRequestHandler : public AsyncWebHandler
{
public:
    ServeDashboardRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeDashboardRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_DASHBOARD_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetDashboard]:[INF]: [SERVE] /dashboard");

        request->send(SD, "/data/ui/index.html", "text/html");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif