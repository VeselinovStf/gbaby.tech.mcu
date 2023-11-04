#ifndef LAB_TYPE_LOGGER_H
#define LAB_TYPE_LOGGER_H

#include <Arduino.h>

enum LogType
{
    /*
        Debug: Send messages to Serial and file
    */
    DEBUG_LEVEL = 0,
    /*
        Production: Send messages only to file
    */
    PRODUCTION_LEVEL = 1,
    /*
        None: No messages send to file or serial
    */
    NONE_LEVEL = 2
};

#endif