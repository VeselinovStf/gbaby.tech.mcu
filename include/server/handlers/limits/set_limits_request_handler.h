#ifndef SET_LIMITS_REQUEST_HANDLER_H
#define SET_LIMITS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "core/app_limit.h"
#include "utility/log/lab_logger.h"

class SetLimitsRequestHandler : public AsyncWebHandler
{
public:
    SetLimitsRequestHandler(AppLimitMapper *app_limits_mapper, app_limit_t *app_limits,LabLogger *logger)
    {
        this->app_limits_mapper = app_limits_mapper;
        this->app_limits = app_limits;
        this->logger = logger;
    }

    virtual ~SetLimitsRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SET_LIMITS_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[SetLimits]:[INF]: [HTTP_GET] /set-limit");

        if (request->hasParam("name"))
        {
            String name = request->getParam("name")->value();

            for (uint8_t i = 0; i < TOTAL_LIMITS; i++)
            {
                /* code */
                if(this->app_limits->limits[i].name == name){

                    // TODO: #91 Move this to more precise place
                    String type = "/data/" + name + "-environment-limits.json";

                    // Independent from the rest of app_controll                   
                    if (request->hasParam("min"))
                    {
                        this->app_limits->limits[i].min = request->getParam("min")->value().toFloat();
                    }

                    if (request->hasParam("max"))
                    {
                        this->app_limits->limits[i].max = request->getParam("max")->value().toFloat();
                    }

                    this->app_limits_mapper->MapToStorage(SD, type.c_str(), &this->app_limits->limits[i]);

                    request->send(200, "application/json", this->app_limits_mapper->MapToString(&this->app_limits->limits[i]));
                }
            }       
        }
        else
        {
            request->send(404, "Invalid Request");
        }
    }

private:
    Routes routes;
    AppLimitMapper *app_limits_mapper;
    app_limit_t *app_limits;
    LabLogger *logger;
};

#endif