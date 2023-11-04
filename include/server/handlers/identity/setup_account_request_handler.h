#ifndef SETUP_ACCOUNT_REQUEST_HANDLER_H
#define SETUP_ACCOUNT_REQUEST_HANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <CustomJWT.h>
#include <ESP32Time.h>
#include "utility/log/lab_logger.h"
#include "server/routes.h"
#include "server/core/web_app_server.h"
#include "core/app_ui.h"

class SetupAccountRequestHandler : public AsyncWebHandler
{
public:
    SetupAccountRequestHandler(
        CustomJWT *jwt, 
        ESP32Time *rtc, 
        LabLogger *logger, 
        app_config_t *app_config, 
        AppConfigMapper *app_config_mapper,
        AppUIMapper *app_ui_mapper)
    {
        this->jwt = jwt;
        this->rtc = rtc;
        this->logger = logger;
        this->app_config = app_config;
        this->app_config_mapper = app_config_mapper;
        this->app_ui_mapper = app_ui_mapper;
    }
    virtual ~SetupAccountRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->url() == "/api/setup-account" && request->method() == HTTP_POST)
        {
            return true;
        }

        return false;
    }

    void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    {
        if (!index)
        {
            Serial.printf("[Login]:[INF]: Request for user Login %u B\n", total);
        }

        String result;

        for (size_t i = 0; i < len; i++)
        {
            result += (char)data[i];
        }

        // Parse the received JSON data
        DynamicJsonDocument loginData(512); // Adjust the size as needed
        DeserializationError error = deserializeJson(loginData, result);
        if (error)
        {
            request->send(400, "application/json", "{\"success\":false, \"status\":400, \"message\":\"Invalid JSON data\"}");
            return;
        }

        // Check if 'username' and 'password' fields are present in the received data
        if (!loginData.containsKey("username") || !loginData.containsKey("oldPassword") || !loginData.containsKey("newPassword"))
        {
            request->send(400, "application/json", "{\"success\":false, \"status\":400, \"message\":\"User Data is required\"}");
            return;
        }

        if (!loginData.containsKey("ssid") ||
            !loginData.containsKey("ssidPassword") ||
            !loginData.containsKey("ipAddress") ||
            !loginData.containsKey("subnetmask") ||
            !loginData.containsKey("gateway") ||
            !loginData.containsKey("connectionType"))
        {
            request->send(400, "application/json", "{\"success\":false, \"status\":400, \"message\":\"Connection Data is required\"}");
            return;
        }

        // Load the existing "/data/users.json" file
        File file = SD.open("/data/users.json", FILE_READ);
        if (!file)
        {
            request->send(500, "application/json", "{\"success\":false, \"status\":500, \"message\":\"Failed to open users.json\"}");
            return;
        }

        // Parse the "users.json" file
        DynamicJsonDocument usersData(1024); // Adjust the size as needed
        error = deserializeJson(usersData, file);
        file.close();

        if (error)
        {
            request->send(500, "application/json", "{\"success\":false, \"status\":500, \"message\":\"Failed to parse users.json\"}");
            return;
        }

        // Check if the provided 'username' and 'password' match any entry in the "users.json" data
        bool userAuthenticated = false;
        const char *username = loginData["username"];
        const char *password = loginData["oldPassword"];
        const char *newPassword = loginData["newPassword"];
        const char *ssid = loginData["ssid"];
        uint8_t connectionType = loginData["connectionType"].as<uint8_t>();
        const char *connectionPassword = loginData["ssidPassword"];
        const char *ipAddress = loginData["ipAddress"];
        const char *subnetmask = loginData["subnetmask"];
        const char *gateway = loginData["gateway"];

        uint8_t id = loginData["id"];

        int totalUsers = usersData["total_users"].as<int>(); // Assuming "total_users" is an integer

        for (int i = 0; i < totalUsers; i++)
        {
            const char *userUsername = usersData["users"][i]["username"];
            const char *userPassword = usersData["users"][i]["userPassword"];

            if (strcmp(username, userUsername) == 0 && strcmp(password, userPassword) == 0)
            {
                // Only for not confirmed
                if (usersData["users"][i]["passwordConfirmed"].as<uint8_t>() == 0)
                {
                    userAuthenticated = true;
                    usersData["users"][i]["username"] = username;
                    usersData["users"][i]["userPassword"] = newPassword;
                    usersData["users"][i]["passwordConfirmed"] = 1;
                    break;
                }
            }
        }

        if (userAuthenticated)
        {
            // Generate a JWT token
            // You should implement your own JWT token generation logic here
            // Retrieve the 'id' value from loginData and convert it to a string
            String idString = String(id);

            int TOKEN_EXPIRATION = 60;

            // Get the current time
            int secs = rtc->getSecond();

            // Calculate the expiration times
            int tokenExpires = secs + 60;
            int refreshTokenExpires = secs + 120; // 2 minutes (2 * 60 seconds)

            // Format the expiration times as strings similar to JavaScript Date
            char new_token_expires[32];         // Adjust the buffer size as needed
            char new_refresh_token_expires[32]; // Adjust the buffer size as needed

            snprintf(new_token_expires, sizeof(new_token_expires), "%d-%02d-%02dT%02d:%02d:%02d", rtc->getYear(), rtc->getMonth(), rtc->getDay(), rtc->getHour(), rtc->getMinute(), tokenExpires);

            snprintf(new_refresh_token_expires, sizeof(new_refresh_token_expires), "%d-%02d-%02dT%02d:%02d:%02d", rtc->getYear(), rtc->getMonth(), rtc->getDay(), rtc->getHour(), rtc->getMinute(), refreshTokenExpires);

            String new_token_jsonString = "{\"data1\": " + idString + ", \"exp\": \"" + new_token_expires + "\"}";
            String new_refresh_token_jsonPayload = "{\"data1\": " + idString + ", \"exp\": \"" + new_refresh_token_expires + "\"}";

            // Convert the JSON string to a char array (C-string)
            char *new_token = strdup(new_token_jsonString.c_str());

            this->jwt->allocateJWTMemory();

            Serial.println("Generating a JWT");
            this->jwt->encodeJWT(new_token);

            Serial.printf("Final TOKEN Output: %s\nFinalOutput Length: %d\n", this->jwt->out, this->jwt->outputLength);

            // Generate a refresh token (you can use a similar approach as JWT or any other method)

            // Calculate the expiration time (you can use a library or calculate manually)
            unsigned long expirationTime = 1500000; /* calculate the expiration time in seconds from now */

            // Prepare the response JSON
            DynamicJsonDocument response(1024);
            response["status"] = 200;
            response["success"] = true;

            JsonObject data = response.createNestedObject("data");
            data["idToken"] = this->jwt->out;

            this->jwt->clear();

            char *new_refresh_token = strdup(new_refresh_token_jsonPayload.c_str());

            Serial.println("Generating a Refresh JWT");
            this->jwt->encodeJWT(new_refresh_token);

            data["refreshToken"] = this->jwt->out;

            // Serialize and send the response
            String jsonResponse;
            serializeJson(response, jsonResponse);

            this->jwt->clear();

            file = SD.open("/data/users.json", FILE_WRITE);
            if (!file)
            {
                Serial.println("[ChangeUserPassword]:[ERR]: Failed to open users.json for writing");
                request->send(500, "application/json", "{\"success\":\"false\", \"status\": 500}");
                return;
            }

            // Serialize the updated JSON object back to the file
            if (serializeJson(usersData, file) == 0)
            {
                Serial.println("[ChangeUserPassword]:[ERR]: Failed to write JSON to users.json");
                request->send(500, "application/json", "{\"success\":\"false\", \"status\": 500}");
                file.close();
                return;
            }

            file.close();

            //  Update Configuration !!!
            this->app_config->INPUT_SSID = ssid;
            this->app_config->INPUT_PASSWORD = connectionPassword;          

            // WIFI MODE
            if (connectionType == 0)
            {
                this->logger->log("[SetUpAccount]:[INFO]: User Select STA MODE");

                this->app_config->WIFI_STA_ENABLED = true;
                this->app_config->STA_IP = ipAddress;
                this->app_config->STA_GETEWAY = gateway;
                this->app_config->STA_SUBNETMASK = subnetmask;
            }
            else
            {
                this->logger->log("[SetUpAccount]:[INFO]: User Select AP MODE");

                this->app_config->WIFI_STA_ENABLED = false;
                this->app_config->AP_IP = ipAddress;
                this->app_config->AP_GETEWAY = gateway;
                this->app_config->AP_SUBNETMASK = subnetmask;
            }

            // SETUP Is Confirmed
            this->app_config->IS_CONFIRMED = true;

            this->app_config_mapper->MapToStorage(this->app_config->APP_CONFIG_FILE_PATH, this->app_config);

            this->app_ui_mapper->MapToWiFiSTA(this->app_config);

            // Set for Server restart   

            request->send(200, "application/json", jsonResponse);   

            esp_restart();
        }
        else
        {
            // Authentication failed
            request->send(401, "application/json", "{\"success\":false, \"status\":401, \"message\":\"SetUp failed\"}");
        }
    }

public:
    CustomJWT *jwt;
    ESP32Time *rtc;
    const String &createJsonDataResponse(float id_token, float expires_at, float refresh_token, float user_id, uint8_t status, uint8_t success)
    {
        return "{\"id_token\":" + String(id_token) +
               ",\"expires_at\":" + String(expires_at) +
               ",\"refresh_token\":" + String(refresh_token) +
               ",\"user_id\":" + String(user_id) + " }";
    };
    Routes routes;
    app_config_t *app_config;
    LabLogger *logger;
    AppConfigMapper *app_config_mapper;
    AppUIMapper *app_ui_mapper;
};

#endif