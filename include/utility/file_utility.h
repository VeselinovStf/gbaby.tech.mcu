#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include <FS.h>

class FileUtility
{
public:
    static void removeDir(fs::FS &fs, const char *path)
    {
        if (!fs.rmdir(path))
        {
            Serial.println("rmdir failed");
        }
    }

    static void deleteFile(fs::FS &fs, const char *path)
    {
        if (!fs.remove(path))
        {
            Serial.println("rmfile failed");
        }
    }

    static void deleteAllFiles(fs::FS &fs, const char *dirname, uint8_t levels)
    {
        File root = fs.open(dirname);
        if (!root)
        {
            Serial.println("Failed to open directory");
            return;
        }
        if (!root.isDirectory())
        {
            Serial.println("Not a directory");
            return;
        }

        File file = root.openNextFile();

        if (!file)
        {
            // No more files or directories, exit the loop
            Serial.println("No more files or directories.");
            
            return;
        }

        while (file)
        {
            if (file.isDirectory())
            {
                deleteAllFiles(fs, file.path(), levels - 1);

                removeDir(fs, file.path());
            }
            else
            {
                deleteFile(fs, file.path());
            }
            file = root.openNextFile();
        }
        
        file.close();
        root.close();
    }

    static void createDir(fs::FS &fs, const char *path)
    {
        if (!fs.mkdir(path))
        {
            Serial.println("mkdir failed");
        }
    }

    static void writeFile(fs::FS &fs, const char *path, File &f)
    {
        File file = fs.open(path, FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed to open file for writing");
            return;
        }

        while (f.available())
        {
            file.print(char(f.read()));
        }

        //Serial.println(file.path());

        file.close();
    }

    static void remapFiles(fs::FS &spiffs, fs::FS &sd, const char *spiffsPath, const char *sdPath)
    {
        File spiffsRoot = spiffs.open(spiffsPath);
        if (!spiffsRoot)
        {
            Serial.println("SPIFFS Failed to open directory");
            return;
        }
        if (!spiffsRoot.isDirectory())
        {
            Serial.println("SPIFFS Not a directory");
            return;
        }

        File file = spiffsRoot.openNextFile(); // SPIFFS STORES ALL IN /

        while (file)
        {
            if (!file.isDirectory())
            {
                writeFile(sd, file.path(), file);
            }
            file = spiffsRoot.openNextFile();
        }

        file.close();
        spiffsRoot.close();
       
    }

    static bool replaceInFile(const char *find, const char *replace, const char *source)
    {
        // Open the file
        File file = SD.open(source, FILE_READ);
        if (!file)
        {
            Serial.println("[FileUtility]:[ERR]:  Failed to open replace for reading");
            return false;
        }

        // Read the file into a string
        String fileContent;
        while (file.available())
        {
            char c = file.read();
            fileContent += c;
        }
        file.close();

        // Replace "localhost" with your desired IP address or hostname
        fileContent.replace(find, replace);

        // Open the file for writing
        file = SD.open(source, FILE_WRITE);
        if (!file)
        {
            Serial.println("[FileUtility]:[ERR]: Failed to open replace for writing");
            return false;
        }

        // Write the modified content to the file
        file.print(fileContent);
        file.close();

        return true;
    }
};

#endif