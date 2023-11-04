#ifndef APP_UI_H
#define APP_UI_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "utility/log/lab_logger.h"
#include "core/app_config.h"
#include "utility/file_utility.h"

/*
    Performs differnt Mapping related with UI
*/
class AppUIMapper
{
public:
    AppUIMapper(LabLogger *logger, app_config_t *app_config)
    {
        this->logger = logger;
    }

    /*
        Replaces app_config AP_IP with app_config STA_IP address.
        This action is coused because User can and will select different IP
        than the default BACKEND_URL of UI environment
    */
    void MapToWiFiSTA(app_config_t *app_config)
    {

        this->logger->log("[AppUIMapper]:[INFO]: Mapping To WiFi STA Mode");

        String findAddressFormat = "http://" + app_config->AP_IP + "/api";
         if (!FileUtility::replaceInFile(findAddressFormat.c_str(), app_config->STA_IP.c_str(), "/data/ui/main.js"))
         {
             this->logger->log("[AppUIMapper]:[ERR]: Error Mapping To WiFi STA Mode");
         }
    }

    /*
       In some cases like: Server is not configured, we assumed that this is not first run of app
       and main.js BACKEND_URL is set to new value, that for in this section
       we are replacing BACKEND_URL in UI source ( SD ) with default AP point
    */
    void MapToWiFiAP(app_config_t *app_config)
    {
        this->logger->log("[AppUIMapper]:[INFO]: Mapping To WiFi AP Mode");

        String findAddressFormat = "http://" + app_config->AP_IP + "/api";
        if (!FileUtility::replaceInFile(findAddressFormat.c_str(), app_config->AP_IP.c_str(), "/data/ui/main.js"))
        {
            this->logger->log("[AppUIMapper]:[ERR]: Error Mapping To WiFi AP Mode");
        }
    }

private:
    LabLogger *logger;
};
#endif