#ifndef POST_UPDATE_REQUEST_HANDLER_H
#define POST_UPDATE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "utility/log/lab_logger.h"
#include <Update.h>

#define U_PART U_SPIFFS

class PostUpdateRequestHandler : public AsyncWebHandler
{
public:
    PostUpdateRequestHandler() { }

    virtual ~PostUpdateRequestHandler() {}

    static void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final, LabLogger *logger, app_config_t *app_config, AppConfigMapper *app_config_mapper)
    {
        int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH;
        if (!index)
        {
            app_config->Lifecycle = Lifecycle::FREE_CLOCK;

            logger->log(("[PostUpdate]:[INF]: Update Start: " + filename).c_str());

            if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
            {
                app_config->Lifecycle = Lifecycle::NORMAL;
                Update.printError(Serial);
            }
        }

        if (Update.write(data, len) != len)
        {
            app_config->Lifecycle = Lifecycle::NORMAL;
            Update.printError(Serial);
        }

        if (final)
        {
            if (Update.end(true))
            {
                logger->log(("[PostUpdate]:[INF]: Update Success: " + (String)index + " : " + len).c_str());

                if (cmd == U_PART)
                {
                    logger->log("[PostUpdate]:[INF]: Setting SD Refresh!");

                    app_config->Lifecycle = Lifecycle::FREE_CLOCK;
                    app_config->REFRESH_STATE = AppConfigRefreshState::SPIFFS_REFRESH;
                    app_config_mapper->MapToStorage(app_config->APP_CONFIG_FILE_PATH, app_config);
                }

                request->redirect("/change-success");

                app_config->Lifecycle = Lifecycle::RESTART_TASK;
            }
            else
            {
                app_config->Lifecycle = Lifecycle::NORMAL;
                Update.printError(Serial);
            }
        }
    }
private:
    Routes routes;
    LabLogger *logger;
};

#endif