#ifndef SET_NOTIFICATION_REQUEST_HANDLER_H
#define SET_NOTIFICATION_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "core/app_notification.h"
#include "utility/log/lab_logger.h"

class SetNotificationRequestHandler : public AsyncWebHandler
{
public:
    SetNotificationRequestHandler(AppNotificationMapper *app_notification_mapper, app_notification_t *app_notification, LabLogger *logger, app_config_t *app_config)
    {
        this->app_notification_mapper = app_notification_mapper;
        this->app_notification = app_notification;
        this->logger = logger;
        this->app_config = app_config;
    }

    virtual ~SetNotificationRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SET_NOTIFICATION_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[SetNotification]:[INF]: [HTTP_GET] /set-notification");

        if (request->hasParam("name"))
        {
            String name = request->getParam("name")->value();

            for (uint8_t i = 0; i < TOTAL_NOTIFICATIONS; i++)
            {
                /* code */

                if (this->app_notification->notifications[i].name == name)
                {
                    // TODO: #91 Move this to more precise place
                    String type = "/data/" + name + "-notifications.json";

                    // Independent from the rest of app_controll
                    if (request->hasParam("apikey"))
                    {
                        this->app_notification->notifications[i].API_KEY = request->getParam("apikey")->value();
                    }

                    if (request->hasParam("id"))
                    {
                        this->app_notification->notifications[i].ID = request->getParam("id")->value();
                    }

                    if (request->hasParam("status"))
                    {
                        this->app_notification->notifications[i].status = request->getParam("status")->value().toInt();
                    }

                    this->app_notification_mapper->MapToStorage(SD, name.c_str(), &this->app_notification->notifications[i]);

                    request->send(200, "application/json", this->app_notification_mapper->MapToString(SD, type.c_str(), &this->app_notification->notifications[i]));

                    this->app_config->Lifecycle = Lifecycle::RESTART_TASK;

                    return;
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
    AppNotificationMapper *app_notification_mapper;
    app_notification_t *app_notification;
    LabLogger *logger;
    app_config_t *app_config;
};

#endif