#ifndef ZIP_UTILITY_H
#define ZIP_UTILITY_H

#include <Arduino.h>
#include <unzipLIB.h>
#include <SD.h>

// Functions to access a file on the SD card
static File myfile;
//
// Callback functions needed by the unzipLIB to access a file system
// The library has built-in code for memory-to-memory transfers, but needs
// these callback functions to allow using other storage media
//
void *myOpen(const char *filename, int32_t *size)
{
    myfile = SD.open(filename);
    *size = myfile.size();
    return (void *)&myfile;
}

void myClose(void *p)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    File *f = (File *)pzf->fHandle;
    if (f)
        f->close();
}

int32_t myRead(void *p, uint8_t *buffer, int32_t length)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    File *f = (File *)pzf->fHandle;
    return f->read(buffer, length);
}

int32_t mySeek(void *pFile, int32_t iPosition, int iType)
{
    ZIPFILE *pzf = (ZIPFILE *)pFile;
    File *f = (File *)pzf->fHandle;
    if (iType == SEEK_SET)
        return f->seek(iPosition);
    else if (iType == SEEK_END)
    {
        return f->seek(iPosition + pzf->iSize);
    }
    else
    { // SEEK_CUR
        long l = f->position();
        return f->seek(l + iPosition);
    }
}

class ZipUtility
{
    public:
        bool isDirectory(const char *str)
        {
            std::string s(str); // Convert the const char* to a std::string
            size_t len = s.length();

            if (len > 0 && s[len - 1] == '/')
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void crreateDirectory(const char *filename)
        {
            if (!SD.exists(filename))
            {
                if (!SD.mkdir(filename))
                {
                    Serial.printf("[ZipUtility]:[ERR]: Failed to create directory: %s\n", filename);
                }
            }
        }

        void createFile(const char *filename, int fileSize)
        {       
            File sdFile = SD.open(filename, FILE_WRITE);
            if (sdFile)
            {
                char buffer[512];
                int bytesRead;

                zip.openCurrentFile(); // Move this line outside the loop

                while (fileSize > 0)
                {
                    int bytesToRead = fileSize < sizeof(buffer) ? fileSize : sizeof(buffer); // Ensure we don't read more than remaining fileSize

                    bytesRead = zip.readCurrentFile((uint8_t *)buffer, bytesToRead);

                    if (bytesRead <= 0)
                    {
                        break; // End of file
                    }

                    sdFile.write((const uint8_t *)buffer, bytesRead);
                    fileSize -= bytesRead;
                }

                sdFile.close();
            }
            else
            {
                Serial.println("[ZipUtility]:[ERR]: Failed to open SD file for writing.");
            }

            zip.closeCurrentFile(); // Close the currently open ZIP file
        }

        bool openZip(const char *zipFilePath)
        {
            if (!this->isoZipOpen)
            {
                this->isoZipOpen = true;

                int rc;
                char szComment[256];
                char szName[256];
                unz_file_info fi;

                rc = rc = this->zip.openZIP(zipFilePath, myOpen, myClose, myRead, mySeek);
                if (rc == UNZ_OK)
                {                  
                    zip.gotoFirstFile();
                    rc = UNZ_OK;

                    while (rc == UNZ_OK)
                    {
                        // Display all files contained in the archive
                        rc = zip.getFileInfo(&fi, szName, sizeof(szName), NULL, 0, szComment, sizeof(szComment));
                        if (rc == UNZ_OK)
                        {
                            if (szName[0] != '/')
                            {
                                // Add a '/' at the beginning of the string
                                memmove(szName + 1, szName, strlen(szName) + 1); // Shift the string to make room
                                szName[0] = '/';                                 // Add the '/'
                            }

                            if (!isDirectory(szName))
                            {
                                createFile(szName, fi.uncompressed_size);
                            }
                            else
                            {

                                int len = strlen(szName);

                                // Check if the string ends with a '/'
                                if (len > 0 && szName[len - 1] == '/')
                                {
                                    // Remove the trailing '/'
                                    szName[len - 1] = '\0';
                                }

                                crreateDirectory(szName);
                            }
                        }
                        rc = zip.gotoNextFile();
                    } // while more files...

                    zip.closeZIP();

                    this->isoZipOpen = false;
                    return true;
                }
                else
                {
                    Serial.println(rc);

                    this->isoZipOpen = false;
                    return false;
                }
            }
        }

private:
    bool isoZipOpen;
    // statically allocate the UNZIP structure (41K)
    UNZIP zip;
};

#endif