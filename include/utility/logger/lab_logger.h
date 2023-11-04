#ifndef LAB_LOGGER_H
#define LAB_LOGGER_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include <SD.h>
#include "utility/log/log_type.h"

class LabLogger
{
public:
    LabLogger(LogType logType, const char *destination)
    {
        // loggerMutex = xSemaphoreCreateMutex();
        this->logType = logType;
        this->destination = destination;
    }

    /*
        Starting file logging functionality
    */
    void begin()
    {
        if (this->logType == LogType::PRODUCTION_LEVEL)
        {
            if (!this->isLogFileOpen)
            {
                this->logfile = SD.open(this->destination, FILE_APPEND);
                this->isLogFileOpen = true;
            }
        }
    }

    /*
        Shuts down file logging
    */
    void shutDown()
    {
        if (this->logType == LogType::PRODUCTION_LEVEL)
        {
            this->logfile.close();
            this->isLogFileOpen = false;
        }
    }

    /*
        Cleans Log file by: Close, Rewrite and open agen file logging.
    */
    void clean()
    {
        this->logfile.close();
        this->isLogFileOpen = false;

        this->logfile = SD.open(this->destination, FILE_WRITE);
        this->logfile.close();

        this->begin();
    }

    void log(const char *message)
    {
        // xSemaphoreTake(loggerMutex, portMAX_DELAY);

        if (this->logType == LogType::DEBUG_LEVEL)
        {
            // Log the message
            Serial.println(message);

            if (this->isLogFileOpen)
            {
                this->logfile.println(message);
                this->logfile.flush();
            }
        }
        else if (this->logType == LogType::PRODUCTION_LEVEL)
        {
            if (this->isLogFileOpen)
            {
                this->logfile.println(message);
                this->logfile.flush();
            }
        }

        // xSemaphoreGive(loggerMutex);
    }

private:
    SemaphoreHandle_t loggerMutex;
    LogType logType;
    const char *destination;
    File logfile;
    bool isLogFileOpen;
};

#endif