#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "file_utility.h"
#include <SD.h>
#include <SPIFFS.h>

class SDUpdate
{
public:

    static void PrepareUpdate(fs::FS &sd,const char *sdPath, uint8_t level)
    {
        FileUtility::deleteAllFiles(sd, sdPath, level);
        
        // TODO: #81 Remove this hardcoded values
        FileUtility::createDir(sd, "/data");
        // FileUtility::createDir(sd, "/logs");


        // FileUtility::remapFiles(spiffs, sd, spiffsPath, sdPath);
    };
};

#endif