#ifndef SERVE_APP_MAIN_REQUEST_HANDLER_H
#define SERVE_APP_MAIN_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"

class ServeAppMainRequestHandler : public AsyncWebHandler
{
public:
    ServeAppMainRequestHandler(LabLogger *logger)
    {
        this->logger = logger;
    }

    virtual ~ServeAppMainRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == this->routes.SERVE_APP_MAIN_ROUTE)
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        this->logger->log("[ServeTOCR]:[INFO]: [SERVE] /main.js");

        File file = SD.open("/data/ui/main.js");
        if (!file)
        {
            Serial.println("[AppControl]:[ERR]: Failed to create file");
            return;
        }

        if (file)
        {
            // Create a buffer to store the data
            const size_t bufferSize = 10024; // Adjust the buffer size as needed
            uint8_t buffer[bufferSize];

            // Initialize a chunked response
            AsyncWebServerResponse *response = request->beginChunkedResponse("application/javascript",
                                                                             [&](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
                                                                             {
                                                                                 size_t bytesRead = file.read(buffer, maxLen);
                                                                                 return bytesRead;
                                                                             });

            // Send the response
            request->send(response);

            // Close the file when done
            file.close();
        }
        else
        {
            request->send(404, "text/plain", "File not found");
        }
    }

private:
    Routes routes;
    LabLogger *logger;
};

#endif