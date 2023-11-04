#ifndef DOWNLOAD_FEATURE_REQUEST_HANDLER_H
#define DOWNLOAD_FEATURE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "core/app_feature.h"

#include <SD.h>

#include "server/core/web_app_server.h"
#include "core/app_control.h"

class DownloadFeatureRequestHandler : public AsyncWebHandler
{
public:
    DownloadFeatureRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }

    virtual ~DownloadFeatureRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.DOWNLOAD_FEATURE_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[DownloadFeature]:[INF]: [HTTP_GET] /download-features");

        request->send(SD, "/data/features.json", "application/json");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif