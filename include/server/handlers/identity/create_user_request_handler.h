#ifndef CREATE_REQUEST_HANDLER_H
#define CREATE_REQUEST_HANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "utility/log/lab_logger.h"
#include "server/routes.h"

enum Role{
    ADMINISTRATOR_ROLE = 0,
    USER_ROLE = 1
};

class CreateUserRequestHandler : public AsyncWebHandler
{
public:
    CreateUserRequestHandler(LabLogger *logger) { this->logger = logger; }
    virtual ~CreateUserRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->url() == this->routes.CREATE_USER_ROUTE && request->method() == HTTP_POST)
        {
            return true;
        }

        return false;
    }

    void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    {
        File file;

        if (!index)
        {
            Serial.printf("[CreateUser]:[INF]: Request for user Creation %u B\n", total);
        }

        // Initialize a JSON object
        DynamicJsonDocument doc(3024); // Adjust the size according to your data

        // Check if the "users.json" file exists
        if (SD.exists("/data/users.json"))
        {
            // Open the existing JSON file
            file = SD.open("/data/users.json", FILE_READ);
            if (file)
            {
                // Deserialize the JSON data from the file into the JSON object
                DeserializationError error = deserializeJson(doc, file);
                file.close();
                if (error)
                {
                    Serial.println("[CreateUser]:[ERR]: Failed to deserialize JSON");
                    request->send(500, "application/json", "{\"success\":\"false\", \"status\": 500}");
                    return;
                }
            }
            else
            {
                Serial.println("[CreateUser]:[ERR]: Failed to open users.json for reading");
                request->send(500, "application/json", "{\"success\":\"false\", \"status\": 500}");
                return;
            }
        }

        // Request
        String incoming_request_data;

        for (size_t i = 0; i < len; i++)
        {
            incoming_request_data += (char)data[i];
        }

        Serial.printf("[CreateUser]:[INF]: Incoming Data: %s\n", incoming_request_data);

        // Parse the incoming JSON data
        DynamicJsonDocument requestJson(2024); // Adjust the size as needed
        DeserializationError requestError = deserializeJson(requestJson, incoming_request_data);

        if (requestError)
        {
            Serial.println("[CreateUser]:[ERR]: Failed to parse incoming JSON");
            request->send(400, "application/json", "{\"success\":\"false\", \"status\": 400}");
            return;
        }

        // Default user setup!

        // Retrieve the existing 'users' array or create it if it doesn't exist
        JsonArray usersArray = doc.createNestedArray("users");

        // Create a new JSON object to represent the user's data from the incoming request
        JsonObject newUser = usersArray.createNestedObject();

        uint8_t newUser_id = doc["total_users"].as<int>() + 1;

        // Increment the "total_users" property by one
        doc["total_users"] = newUser_id;

        // Assign new values to the new user object based on the incoming data
        newUser["id"] = newUser_id;
        newUser["username"] = requestJson["username"].as<String>();
        newUser["userPassword"] = requestJson["userPassword"].as<String>();

        // User roles
        if (newUser_id == 1){
            newUser["role"] = Role::ADMINISTRATOR_ROLE;
        }else{
            newUser["role"] = Role::USER_ROLE;
        }

            // Reopen the "users.json" file for writing
            file = SD.open("/data/users.json", FILE_WRITE);
        if (!file)
        {
            Serial.println("[CreateUser]:[ERR]: Failed to open users.json for writing");
            request->send(500, "application/json", "{\"success\":\"false\", \"status\": 500}");
            return;
        }

        // Serialize the updated JSON object back to the file
        if (serializeJson(doc, file) == 0)
        {
            Serial.println("[CreateUser]:[ERR]: Failed to write JSON to users.json");
            request->send(500, "application/json", "{\"success\":\"false\", \"status\": 500}");
            file.close();
            return;
        }

        file.close();

        Serial.println("[CreateUser]:[INF]: User Created!");

        request->send(200, "application/json", "{\"success\":\"true\", \"status\": 200}");
    }
    private:
        Routes routes;
        LabLogger *logger;
};

#endif