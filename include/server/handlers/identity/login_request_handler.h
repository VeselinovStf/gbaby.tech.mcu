#ifndef LOGIN_REQUEST_HANDLER_H
#define LOGIN_REQUEST_HANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <CustomJWT.h>
#include <ESP32Time.h>
#include "utility/log/lab_logger.h"
#include "server/routes.h"

class LoginRequestHandler : public AsyncWebHandler
{
public:
    LoginRequestHandler(CustomJWT *jwt, ESP32Time *rtc, LabLogger *logger)
    {
        this->jwt = jwt;
        this->rtc = rtc;
        this->logger = logger;
    }
    virtual ~LoginRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->url() == this->routes.IDENTITY_AUTH_ROUTE && request->method() == HTTP_POST)
        {
            return true;
        }

        return false;
    }

    void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    {
        // // this->jwt->clear();
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
        if (!loginData.containsKey("email") || !loginData.containsKey("password"))
        {
            request->send(400, "application/json", "{\"success\":false, \"status\":400, \"message\":\"Username and password are required\"}");
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
        const char *username = loginData["email"];
        const char *password = loginData["password"];
        bool isUserPasswordConfirmed = false;
        uint8_t id = loginData["id"];

        int totalUsers = usersData["total_users"].as<int>(); // Assuming "total_users" is an integer

        for (int i = 0; i < totalUsers; i++)
        {
            const char *userUsername = usersData["users"][i]["username"];
            const char *userPassword = usersData["users"][i]["userPassword"];

            if (strcmp(username, userUsername) == 0 && strcmp(password, userPassword) == 0)
            {
                userAuthenticated = true;
                id = usersData["users"][i]["id"];
                isUserPasswordConfirmed = usersData["users"][i]["passwordConfirmed"];
                break;
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
            DynamicJsonDocument response(512);

            response["status"] = 200;
            if(!isUserPasswordConfirmed){
                response["status"] = 401;
            }
            
            response["success"] = true;

            JsonObject data = response.createNestedObject("data");
            data["idToken"] = this->jwt->out;

            this->jwt->clear();

            this->jwt->allocateJWTMemory();

            char *new_refresh_token = strdup(new_refresh_token_jsonPayload.c_str());

            Serial.println("Generating a Refresh JWT");
            this->jwt->encodeJWT(new_refresh_token);

            data["refreshToken"] = this->jwt->out;

            // Serialize and send the response
            String jsonResponse;
            serializeJson(response, jsonResponse);

            Serial.printf("Final REFRESH TOKEN Output: %s\nFinalOutput Length: %d\n", this->jwt->out, this->jwt->outputLength);

            this->jwt->clear();

            request->send(200, "application/json", jsonResponse);
        }
        else
        {
            // Authentication failed
            request->send(401, "application/json", "{\"success\":false, \"status\":401, \"message\":\"Authentication failed\"}");
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
    LabLogger *logger;
};

#endif