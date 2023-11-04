#include "Arduino.h"
#include "server/core/web_app_server.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <AsyncJson.h>
#include <Update.h>
#include <SD.h>
#include <unzip.h>

#include "server/handlers/home/serve_index_request_handler.h"
#include "server/handlers/update/serve_update_request_handler.h"
#include "server/handlers/update/post_update_request_handler.h"
#include "server/handlers/control/serve_control_request_handler.h"
#include "server/handlers/not_found_request_handler.h"
#include "server/handlers/common/serve_privacy_request_handler.h"
#include "server/handlers/common/serve_toc_request_handler.h"
#include "server/handlers/dashboard/serve_dashboard_request_handler.h"
#include "server/handlers/fans/serve_fans_request_handler.h"
#include "server/handlers/hydro/serve_hydro_request_handler.h"
#include "server/handlers/lights/serve_lights_request_handler copy.h"
#include "server/handlers/notifications/serve_telegram_notifications_request_handler.h"
#include "server/handlers/settings/serve_settings_request_handler.h"
#include "server/handlers/temperature/serve_temperature_request.handler.h"
#include "server/handlers/credentials/serve_credentials_request_handler.h"
#include "server/handlers/credentials/set_credentials_request_handler.h"
#include "server/handlers/control-data/get_control_data_request_handler.h"
#include "server/handlers/control-data/set_control_data_request_handler.h"
#include "server/handlers/credentials/serve_credentials_applied_request_handler.h"
#include "server/handlers/device/restart_device_request_handler.h"
#include "server/handlers/credentials/restart_credentials_request_handler.h"
#include "server/handlers/logs/serve_logs_request_handler.h"
#include "server/handlers/logs/get_logs_request_handler.h"
#include "server/handlers/device/shutdown_device_request_handler.h"
#include "core/app_feature.h"
#include "server/handlers/features/serve_features_request_handler.h"
#include "server/handlers/features/download_features_request_handler.h"
#include "server/handlers/features/save_feature_request_handler.h"
#include "server/handlers/features/load_feature_request_handler.h"
#include "server/handlers/features/set_feature_request_handler.h"
#include "server/handlers/device/restart_device_state_request_handler copy.h"
#include "server/handlers/limits/get_limits_request_handler.h"
#include "server/handlers/water-tank/serve_water_tank_request_handler.h"
#include "server/handlers/notifications/set_notification_request_handler.h"
#include "server/handlers/notifications/get_notification_request_handler.h"
#include "core/app_limit.h"
#include "server/handlers/limits/set_limits_request_handler.h"
#include "server/handlers/common/serve_change_success_request_handler.h"
#include "server/handlers/history/serve_history_request_handler.h"
#include "server/handlers/update/post_update_fs_request_handler.h"
#include "server/handlers/identity/create_user_request_handler.h"
#include "server/handlers/identity/login_request_handler.h"
#include "server/handlers/identity/serve_login_request_handler.h"
#include "server/handlers/common/serve_about_request_handler.h"
#include "server/handlers/identity/setup_account_request_handler.h"
#include "server/handlers/identity/serve_setup_account_request_handler copy.h"

#include "server/routes.h"
#include "utility/log/lab_logger.h"
#include <WiFiClientSecure.h>

#pragma region VARIABLES

// Server
AsyncWebServer server(80);

// OTA
size_t content_len;
#define U_PART U_SPIFFS

#pragma endregion VARIABLES

#pragma region CTOR

WebAppServer::WebAppServer(
    app_config_t &s,
    app_control_t &state,
    LabLogger *logger,
    AppConfigMapper &app_config_mapper,
    AppControlMapper &app_control_mapper,
    ESP32Time *rtc,
    AppFeatureMapper &app_feature_mapper,
    action_feature_t &action_feature,
    AppLimitMapper &app_limit_mapper,
    app_limit_t &app_limits,
    AppNotificationMapper &app_notification_mapper,
    app_notification_t &app_notifications,
    CustomJWT &app_jwt,
    AppUIMapper &app_ui_mapper)
{
  this->app_config = &s;
  this->logger = logger;
  this->app_control = &state;
  this->app_config_mapper = &app_config_mapper;
  this->app_control_mapper = &app_control_mapper;
  this->rtc = rtc;
  this->app_feature_mapper = &app_feature_mapper;
  this->action_feature = &action_feature;
  this->app_limit_mapper = &app_limit_mapper;
  this->app_limits = &app_limits;
  this->app_notification_mapper = &app_notification_mapper;
  this->app_notifications = &app_notifications;
  this->app_jwt = &app_jwt;
  this->app_ui_mapper = &app_ui_mapper;
}

#pragma endregion CTOR

#pragma region BEGIN

void WebAppServer::begin(fs::FS &fs)
{
#pragma region SETUP
  // Load Settings
  this->app_config_mapper->MapFromStorage(SD, this->app_config->APP_CONFIG_FILE_PATH, this->app_config);

  if (this->app_config->IS_CONFIRMED)
  {
    this->app_control_mapper->MapFromStorage(SD, this->app_config->APP_CONTROL_FILE_PATH, this->app_control);
  }

  this->logger->log("[APP]:[INF]: Cleaning WebAppServer Server ...");

  WiFi.softAPdisconnect();
  WiFi.disconnect(true);  // that no old information is stored
  WiFi.mode(WIFI_OFF);    // switch WiFi off
  WiFi.persistent(false); // avoid that WiFi-parameters will be stored in persistent memory
  delay(1000);            // short wait to ensure WIFI_OFF

  
  if (this->app_config->WIFI_STA_ENABLED && this->app_config->IS_CONFIRMED)
  {
    this->logger->log("[APP]:[INF]: WebAppServer Server is Starting WIFI STA");

    IPAddress staIpAddress;
    IPAddress staGateway;
    IPAddress staSubnet;

    staIpAddress.fromString(this->app_config->STA_IP);
    staGateway.fromString(this->app_config->STA_GETEWAY);
    staSubnet.fromString(this->app_config->STA_SUBNETMASK);

    WiFi.mode(WIFI_STA);

    WiFi.config(staIpAddress, staGateway, staSubnet);

    WiFi.begin(this->app_config->INPUT_SSID.c_str(), this->app_config->INPUT_PASSWORD.c_str());
    
    this->logger->log("[APP]:[INF]: WebAppServer Server is Starting .... ");

    delay(2000);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      this->logger->log("[APP]:[ERR]: WebAppServer Server WiFi STA Failed!");

      while (WiFi.status() != WL_CONNECTED)
      {
        this->logger->log("[APP]:[ERR]: Cant connect WIFI STA WebAppServer Server!");

        delay(500);
      }
    }
    else
    {
      this->logger->log("[APP]:[INF]: WebAppServer Server WIFI STA Server Working!");
    }

    this->logger->log("[APP]:[INF]: STA IP Address: ");

    this->logger->log(WiFi.localIP().toString().c_str());
    this->logger->log(WiFi.SSID().c_str());  
  }
  else
  {
    this->logger->log("[APP]:[INF]: WebAppServer Server is Starting SOFT APA");

    // TODO: validate is WebAppServer worrking in AP - https://github.com/espressif/arduino-esp32/issues/2025
    WiFi.mode(WIFI_AP);

    IPAddress apIpAddress;
    IPAddress apGateway;
    IPAddress apSubnet;

    apIpAddress.fromString(this->app_config->AP_IP);
    apGateway.fromString(this->app_config->AP_GETEWAY);
    apSubnet.fromString(this->app_config->AP_SUBNETMASK);

    WiFi.softAPConfig(apIpAddress, apGateway, apSubnet);

    WiFi.softAP(this->app_config->INPUT_SSID.c_str(), this->app_config->INPUT_PASSWORD.c_str());

    // TODO: #105 Notifiers on AP MODE!
    IPAddress IP = WiFi.softAPIP();
    this->logger->log("[APP]:[INF]: AP IP address: ");
    this->logger->log(IP.toString().c_str());
  }

#pragma endregion SETUP

#pragma region STATIC_RESOURCES

  server.serveStatic("/", SD, "/data/ui/").setDefaultFile("index.html");

#pragma endregion STATIC_RESOURCES

  server.onNotFound(NotFoundRequestHandler::handleRequest);

  if (this->app_config->IS_CONFIRMED)
  {
    server.on(
        this->routes.POST_UPDATE_ROUTE, HTTP_POST, [](AsyncWebServerRequest *request) {},
        [&](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
            size_t len, bool final)
        {
          PostUpdateRequestHandler::handleUpload(request, filename, index, data, len, final, this->logger, this->app_config, this->app_config_mapper);
        });

    server.on(
        this->routes.LOAD_FEATURES_ROUTE, HTTP_POST, [](AsyncWebServerRequest *request) {},
        [&](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
            size_t len, bool final)
        {
          LoadFeatureRequestHandler::handleUpload(request, filename, index, data, len, final, this->logger);
        });

    server.on(
        this->routes.POST_UPDATE_FS_ROUTE, HTTP_POST, [](AsyncWebServerRequest *request) {},
        [&](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
            size_t len, bool final)
        {
          PostUpdateFSRequestHandler::handleUpload(request, filename, index, data, len, final, this->logger, this->app_config, this->app_config_mapper);
        });
  }

    server.addHandler(new CreateUserRequestHandler(this->logger));
    server.addHandler(new LoginRequestHandler(this->app_jwt, this->rtc, this->logger));
    server.addHandler(new SetupAccountRequestHandler(this->app_jwt, this->rtc, this->logger, this->app_config, this->app_config_mapper, this->app_ui_mapper));
    server.addHandler(new ServeIndexRequestHandler(this->logger));
    server.addHandler(new ServeSetupAccountRequestHandler(this->logger));
    server.addHandler(new ServeAboutRequestHandler(this->logger));
    server.addHandler(new ServeLoginRequestHandler(this->logger));
    server.addHandler(new ServePrivacyRequestHandler(this->logger));
    server.addHandler(new ServeUpdateRequestHandler(this->logger));
    server.addHandler(new ServeControlRequestHandler(this->logger));
    server.addHandler(new ServePrivacyRequestHandler(this->logger));
    server.addHandler(new ServeTocRequestHandler(this->logger));
    server.addHandler(new ServeControlRequestHandler(this->logger));
    server.addHandler(new ServeDashboardRequestHandler(this->logger));
    server.addHandler(new GetFansRequestHandler(this->logger));
    server.addHandler(new GetHydroRequestHandler(this->logger));
    server.addHandler(new GetLightsRequestHandler(this->logger));
    server.addHandler(new ServeTelegramNoticifationsRequestHandler(this->logger));
    server.addHandler(new ServeSettingsRequestHandler(this->logger));
    server.addHandler(new ServeTemperatureRequestHandler(this->logger));
    server.addHandler(new ServeUpdateRequestHandler(this->logger));
    server.addHandler(new ServeCredentialsAppliedRequestHandler(this->logger));
    server.addHandler(new RestartDeviceSettingsRequestHandler(this->logger, this->app_config));
    server.addHandler(new DownloadFeatureRequestHandler(this->logger));
    server.addHandler(new ShutDownDeviceSettingsRequestHandler(this->logger));
    server.addHandler(new RestartDeviceStateRequestHandler(this->logger, this->app_control, this->app_config, this->app_config_mapper));
    server.addHandler(new RestartCredentialsSettingsRequestHandler(this->app_config, this->app_config_mapper, this->logger));
    server.addHandler(new ServeFeatureRequestHandler(this->app_config, this->app_control, this->app_feature_mapper, this->logger, this->action_feature));
    server.addHandler(new GetControlDataRequestHandler(this->app_control, this->app_control_mapper, this->logger));
    server.addHandler(new SetControlDataRequestHandler(fs, this->app_config, this->app_control, this->app_control_mapper, this->logger, this->rtc));
    server.addHandler(new ServeLogsRequestHandler(this->logger));
    server.addHandler(new GetLimitsRequestHandler(this->logger));
    server.addHandler(new ServeChangeSuccessRequestHandler(this->logger));
    server.addHandler(new ServeWaterTankRequestHandler(this->logger));
    server.addHandler(new GetNotificationsRequestHandler(this->logger));
    server.addHandler(new ServeHistoryRequestHandler(this->logger));
    server.addHandler(new SetNotificationRequestHandler(this->app_notification_mapper,this->app_notifications, this->logger, this->app_config));
    server.addHandler(new SetLimitsRequestHandler(this->app_limit_mapper, this->app_limits, this->logger));
    server.addHandler(new SaveFeatureRequestHandler(this->app_feature_mapper, this->logger, this->rtc, this->action_feature, this->app_config, this->app_control));
    server.addHandler(new GetLogsRequestHandler(this->app_config, this->logger));
    server.addHandler(new SetFeatureRequestHandler(this->action_feature, this->app_control, this->app_control_mapper, this->app_feature_mapper,this->logger, this->rtc));

  // Remove in production!!!
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "access-control-allow-origin");
  //DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");

  server.begin();
}

#pragma endregion BEGIN

#pragma region DESTROY

void WebAppServer::destroy()
{
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false)); // set as 'false' the promiscuous mode
  ESP_ERROR_CHECK(esp_wifi_stop());                 // it stop soft-AP and free soft-AP control block
  ESP_ERROR_CHECK(esp_wifi_deinit());               // free all resource allocated in esp_wifi_init() and stop WiFi task
  server.end();
}

#pragma endregion DESTROY