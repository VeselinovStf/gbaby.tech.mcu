#ifndef GET_CONTROL_DATA_REQUEST_HANDLER_H
#define GET_CONTROL_DATA_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>

#include "server/core/web_app_server.h"
#include "core/app_control.h"

class GetControlDataRequestHandler : public AsyncWebHandler
{
public:
    GetControlDataRequestHandler(app_control_t *app_control, AppControlMapper *app_control_mapper, LabLogger *logger) {
        this->app_control = app_control;
        this->app_control_mapper = app_control_mapper;
        this->logger = logger;
    }

    virtual ~GetControlDataRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.GET_CONTROL_DATA_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetControl]:[INF]: [HTTP_GET] /get-control-data");

        // TODO: #58 Return dissabled property to UI ellement while is disabled it must be hiden for edit
        request->send(SD, "/data/control.json", "application/json");
    }

private:
    Routes routes;
    app_control_t *app_control;
    AppControlMapper *app_control_mapper;
    LabLogger *logger;
};

#endif