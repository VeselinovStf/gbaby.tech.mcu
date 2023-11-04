#ifndef SERVE_HISTORY_REQUEST_HANDLER_H
#define SERVE_HISTORY_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include <SD.h>
#include "utility/log/lab_logger.h"

class ServeHistoryRequestHandler : public AsyncWebHandler
{
public:
    ServeHistoryRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~ServeHistoryRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_HISTORY_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[GetPrivacy]:[INF]: [HTTP_GET] /get-history");

        if (!request->hasParam("name") || !request->hasParam("d") || !request->hasParam("m") || !request->hasParam("y"))
        {
            request->send(404, "Invalid Parameter");

            return;
        }

        String type = "/data/history/" + request->getParam("d")->value() + "_" +
                      request->getParam("m")->value() + "_" +
                      request->getParam("y")->value() + "_" +
                      request->getParam("name")->value() + ".json";

        // TODO: #107 Fix SPIFF IS FULL
        if (SD.exists(type))
        {
            File file = SD.open(type);
            if (file)
            {
                file.close();

                // File opened successfully
                request->send(SD, type, "text/plain");

                return;
            }
            else
            {
                file.close();
                // Error opening the file
                request->send(500, "text/plain", "Error opening file");

                return;
            }
        }
        else
        {
            // File does not exist
            request->send(404, "text/plain", "File not found");

            return;
        }

        //AsyncWebServerResponse *response = request->beginResponse(SD, type, "text/plain");
        // TODO: REMOVE THIS
      
        // request->send(SD, type, "text/plain");
        // request->addHeader("Access-Control-Allow-Origin", "*");
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif