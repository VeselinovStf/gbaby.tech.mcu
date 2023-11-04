#ifndef GET_LIMITS_REQUEST_HANDLER_H
#define GET_LIMITS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class GetLimitsRequestHandler : public AsyncWebHandler
{
public:
    GetLimitsRequestHandler(LabLogger *logger) {this->logger = logger;}
    virtual ~GetLimitsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.GET_LIMITS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetPrivacy]:[INF]: [HTTP_GET] /get-limits");

        if (request->hasParam("name"))
        {
            String type = "/" + request->getParam("name")->value() + "-environment-limits.json";
            
            request->send(SD, type, "application/json");
        }else{
            request->send(404, "Invalid Request");
        }  
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif