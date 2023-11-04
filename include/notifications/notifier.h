#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include "utility/log/lab_logger.h"

class Notifier
{
public:
    Notifier(LabLogger *logger)
    {
        this->logger = logger;
    };

    virtual void send(String message){};
private:
    LabLogger *logger;
};

#endif