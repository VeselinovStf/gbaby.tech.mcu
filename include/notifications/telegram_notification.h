#ifndef TELEGRAM_NOTIFICATION_H
#define TELEGRAM_NOTIFICATION_H

#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include "utility/log/lab_logger.h"
#include "notifications/notifier.h"
#include "core/app_notification.h"

class TelegramNotifier : public Notifier
{
public:
    TelegramNotifier(LabLogger *logger, notification_t *telegram_notifier_config, app_config_t *app_config) : Notifier(logger), client(WiFiClientSecure())
    {
        this->logger = logger;
        this->telegram_notifier_config = telegram_notifier_config;
        this->app_config = app_config;
        this->client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
        this->bot = new UniversalTelegramBot(this->telegram_notifier_config->API_KEY, this->client);
        this->cache = "";
    };

    /*
        Sends messages to Telegram.
        NOTE: Sends one message at a time 
              the rest are accumulated and 
              send when bot is free
    */
    void send(const String &message)
    {
        // Only one message at a time
        if (!this->in_call)
        {
            this->logger->log("[TelegramNotifier]:[INF]: Sending Messages");
            this->_send(message);

            // After message is send check acumulated messages
            if (this->cache != "")
            {
                // Send Them and clear cache
                this->_send(this->cache);
                this->cache = "";
            }
        }
        else
        {
            // Rest of messages are cached
            this->cache += "\n" + message;
        }
    };

private:
    notification_t *telegram_notifier_config;
    WiFiClientSecure client;
    LabLogger *logger;
    app_config_t *app_config;
    UniversalTelegramBot *bot;
    bool in_call;
    String cache;

    void _send(const String &message)
    {
        this->in_call = true;
        // To variable!
        uint8_t call_result = this->bot->sendMessage(this->telegram_notifier_config->ID, message);
        
        this->in_call = false;
    }
};

#endif