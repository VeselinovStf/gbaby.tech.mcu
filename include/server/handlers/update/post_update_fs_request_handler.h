#ifndef POST_UPDATE_FS_REQUEST_HANDLER_H
#define POST_UPDATE_FS_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "utility/log/lab_logger.h"
#include <Update.h>
#include "core/app_config.h"
#include "utility/sd_update.h"

#include "server/routes.h"

class PostUpdateFSRequestHandler
{
public:
    PostUpdateFSRequestHandler() {}

    virtual ~PostUpdateFSRequestHandler() {}

    static void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final, LabLogger *logger, app_config_t *app_config, AppConfigMapper *app_config_mapper)
    {
        if (!index)
        {
            // The first part of the file is being uploaded, so create or open the file for writing
            File file = SD.open("/" + filename, "w");
            if (!file)
            {
                logger->log("[PostUpdateFS]:[ERR]: Failed to open file for writing");
                return;
            }
            // You can specify another storage medium like SD instead of SPIFFS
            file.write(data, len);
            file.close();
        }
        else
        {
            // Subsequent parts of the file are being uploaded, so append the data to the open file
            File file = SD.open("/" + filename, "a");
            if (!file)
            {
                logger->log("[PostUpdateFS]:[ERR]: Failed to open file for appending");
                return;
            }
            // You can specify another storage medium like SD instead of SPIFFS
            file.write(data, len);
            file.close();
        }

        if (final)
        {
            // This is the final part of the file, so close the file
            logger->log("[PostUpdateFS]:[INF]: File upload completed");

            request->redirect("/change-success");

            app_config->UPLOAD_SD_FILE_PATH = "/" + filename;

            app_config->REFRESH_STATE = AppConfigRefreshState::SPIFFS_REFRESH;
            app_config_mapper->MapToStorage(app_config->APP_CONFIG_FILE_PATH, app_config);

            app_config->Lifecycle = Lifecycle::RESTART_TASK;
        }
    }

private:
    Routes routes;
    LabLogger *logger;
   
};

#endif