#ifndef WEB_APP_SERVER_H
#define WEB_APP_SERVER_H

#include "Arduino.h"
#include "server/routes.h"
#include "core/app_config.h"
#include "core/app_control.h"
#include "ESP32Time.h"
#include "utility/log/lab_logger.h"
#include "core/app_feature.h"
#include "core/app_limit.h"
#include <WiFiClientSecure.h>
#include <CustomJWT.h>
#include "core/app_ui.h"

class WebAppServer
{
public:
    WebAppServer(
        app_config_t &s,
        app_control_t &state,
        LabLogger *logger,
        AppConfigMapper &app_config_mapper,
        AppControlMapper &app_control_mapper,
        ESP32Time *rtc,
        AppFeatureMapper &app_feature_mapper,
        action_feature_t &action_feature,
        AppLimitMapper &app_limit_mapper,
        app_limit_t &app_limits,
        AppNotificationMapper &app_notification_mapper,
        app_notification_t &app_notifications,
        CustomJWT &app_jwt,
        AppUIMapper &app_ui_mapper);
    void begin(fs::FS &fs);
    void destroy();
    void extractZipFile();

        private : Routes routes;
    app_control_t *app_control;
    LabLogger *logger;
    app_config_t *app_config;
    action_feature_t *action_feature;
    AppConfigMapper *app_config_mapper;
    AppControlMapper *app_control_mapper;
    ESP32Time *rtc;
    AppFeatureMapper *app_feature_mapper;
    AppLimitMapper *app_limit_mapper;
    app_limit_t *app_limits;
    AppNotificationMapper *app_notification_mapper;
    app_notification_t *app_notifications;
    CustomJWT *app_jwt;
    AppUIMapper *app_ui_mapper;
};
#endif