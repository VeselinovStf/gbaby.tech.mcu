#ifndef NOT_FOUND_REQUEST_HANDLER_H
#define NOT_FOUND_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"
#include "server/routes.h"
#include "utility/log/lab_logger.h"

class NotFoundRequestHandler : public ArRequestHandlerFunction
{
public:
    NotFoundRequestHandler() {
    }

    virtual ~NotFoundRequestHandler() {}

    static void handleRequest(AsyncWebServerRequest *request)
    {
        request->send(SD, "/data/ui/index.html", "text/html");
    }
};

#endif