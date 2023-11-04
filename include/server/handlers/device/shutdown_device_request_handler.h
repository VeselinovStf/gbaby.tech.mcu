#ifndef SHUTDOWN_DEVICE_REQUEST_HANDLER_H
#define SHUTDOWN_DEVICE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"
#include <SD.h>

class ShutDownDeviceSettingsRequestHandler : public AsyncWebHandler
{
public:
    ShutDownDeviceSettingsRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }

    virtual ~ShutDownDeviceSettingsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SHUTDOWN_DEVICE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ShutDownDevice]:[INF]: [HTTP_GET] /shutdown-device");

        this->logger->shutDown();

        // TODO: #50 UI Create Succes Restart Page
        request->redirect("/change-success");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif