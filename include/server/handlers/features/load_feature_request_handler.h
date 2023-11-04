#ifndef LOAD_FEATURE_REQUEST_HANDLER_H
#define LOAD_FEATURE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>

#include "server/core/web_app_server.h"
#include "core/app_control.h"
#include "rules/rule_types.h"

#include "ESP32Time.h"
#include "utility/log/lab_logger.h"

class LoadFeatureRequestHandler : public AsyncWebHandler
{
public:
    LoadFeatureRequestHandler()
    {
    }

    virtual ~LoadFeatureRequestHandler() {}

    static void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final, LabLogger *logger)
    {
        String logmessage = "[LoadFeature]:[INF]: Client:" + request->client()->remoteIP().toString() + " " + request->url();
        logger->log(logmessage.c_str());

        if (!index)
        {
            logmessage = "Upload Start: " + String(filename);

            request->_tempFile = SD.open("/data/features.json", "w");

            Serial.println(logmessage);
        }

        if (len)
        {
            // stream the incoming chunk to the opened file
            request->_tempFile.write(data, len);
            logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
            Serial.println(logmessage);
        }

        if (final)
        {
            logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
            // close the file handle as the upload is now done
            request->_tempFile.close();
            Serial.println(logmessage);
            
            request->redirect("/control");
        }
    }
};

#endif