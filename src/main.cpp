#pragma region INCLUDES

#include <Arduino.h>
#include "server/core/web_app_server.h"
#include <SD.h>

#include "utility/log/lab_logger.h"
#include "rules/rule_validator.h"

#include "core/app_functions.h"
#include "core/app_feature.h"
#include "core/app_workflow.h"
#include "core/app_ui.h"
#include "core/app_limit.h"
#include "core/app_history.h"
#include "core/app_notification.h"

#include "notifications/telegram_notification.h"

#include "utility/zip_utility.h"

#include <ESP32Time.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "DHT.h"
#include <Adafruit_Sensor.h>

#include <Ticker.h>
#include <SPIFFS.h>
#include <SD.h>
#include "utility/sd_update.h"

#include "controls/switch_control.h"

#include <CustomJWT.h>
#include <Crypto.h>
#include <SHA512.h>

#pragma endregion INCLUDES

#define MODE LogType::DEBUG_LEVEL
#define SHA512_HASH_LEN 64
char key[] = "tester";

/*
 * Variables, objects and etc. definitions
 */
#pragma region DEFINITIONS

#pragma region VARIABLES

app_config_t app_config;

// TODO: #103 Move this settings in Device specific seting page
const int TANK_LEVEL_POWER_PIN = 25;
const int TANK_LEVEL_SIGNAL_PIN = 33;
#define DHTPIN 32
const int INTAKE_FAN_PIN = 27;
const int EXHAUST_FAN_PIN = 26;
const int PUMP_ONE_PIN = 16;
const int PUMP_TWO_PIN = 17;
const int LIGHT_PIN = 12;       // 18;
const int OXYGEN_PUMP_PIN = 14; // 19;

int SD_SS_PIN = 5;

// DHT Sensor delay time for accurate response
static const TickType_t DHT_MESURMENT_DELAY = 300 / portTICK_PERIOD_MS;
static const TickType_t WATER_MESURMENT_DELAY = 10 / portTICK_PERIOD_MS;

// Water Sensor
static const int WATER_SENSOR_SENSOR_MIN = 0;
static const int WATER_SENSOR_SENSOR_MAX = 700;

// TODO: #72 Check if bigger delay 3000 want may things work better or test opposed 1000
static const TickType_t task_delay = 2000 / portTICK_PERIOD_MS;

const int CLOCK_IO_PIN = 13; // 22; // THIS PIN IS NOT CONNECTED
const int CLOCK_SCLK_PIN = 35;
const int CLOCK_CE_PIN = 34;

// TODO: Use a global variable to signal long running tasks in app to other servicess !!
//      that way the sensor tasks may be stopped!

#pragma endregion VARIABLES

#pragma region LOGGING

// Logger app_logger(MODE, &sd_storage, app_config.APP_LOG_FILE_PATH);
LabLogger app_logger(MODE, app_config.APP_LOG_FILE_PATH);

#pragma endregion LOGGING

#pragma region TIME

ThreeWire myWire(CLOCK_IO_PIN, CLOCK_SCLK_PIN, CLOCK_CE_PIN); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
// ESP32Time rtc;
ESP32Time rtc(3600); // offset in seconds GMT+1
app_time_t app_time;
/* create a hardware update_change_tracker_timer */
hw_timer_t *update_change_tracker_timer = NULL;

#pragma endregion TIME

#pragma region APP

void generateSignature(char *output, size_t *outputLen, void *secret, size_t secretLen, void *data, size_t dataLen)
{
  uint8_t hashed[SHA512_HASH_LEN];
  memset(hashed, 0, SHA512_HASH_LEN);
  hmac<SHA512>(hashed, SHA512_HASH_LEN, secret, secretLen, data, dataLen);
  Base64URL::base64urlEncode(hashed, SHA512_HASH_LEN, output, outputLen);
}

CustomJWT jwt(key, 256, 40, SHA512_HASH_LEN, "HS512", generateSignature);

app_control_t app_control;
action_feature_t app_feature;
app_limit_t app_limits;
app_notification_t app_notifications;

AppConfigMapper app_config_mapper(&app_logger);
AppControlMapper app_control_mapper(&app_logger);
AppFeatureMapper app_feature_mapper(&app_logger);
AppLimitMapper app_limit_mapper(&app_logger, &app_limits);
AppNotificationMapper app_notification_mapper(&app_logger);
AppUIMapper app_ui_mapper(&app_logger, &app_config);

// WiFiClientSecure client;
TelegramNotifier *telegram_notifier;

WebAppServer app_server(
    app_config,
    app_control,
    &app_logger,
    app_config_mapper,
    app_control_mapper,
    &rtc,
    app_feature_mapper,
    app_feature,
    app_limit_mapper,
    app_limits,
    app_notification_mapper,
    app_notifications,
    jwt,
    app_ui_mapper);

static const uint8_t UPDATE_TASKS_TIME_THRESHOLD_SECUNDS = AppFunctions::TOTAL_CORE_FUNCTIONS + 2;
#pragma endregion APP

#pragma region CONTROL
AppHistory history(&app_logger, "/data/history/", &rtc, &app_control_mapper);

SwitchControl light_control(ActionFunctionIDX::LIGHT_SOURCE, "lightSource", &history);
SwitchControl inputPump_control(ActionFunctionIDX::INPUT_PUMP, "inputPump", &history);
SwitchControl outputPump_control(ActionFunctionIDX::OUTPUT_PUMP, "outputPump", &history);
SwitchControl fanIntake_control(ActionFunctionIDX::INTAKE_FAN, "fanIntake", &history);
SwitchControl fanExhaus_control(ActionFunctionIDX::EXHAUS_FAN, "fanExhaus", &history);
SwitchControl oxygenPump_control(ActionFunctionIDX::OXYGEN_PUMP, "oxygenPump", &history);

ZipUtility zipUtility;

#pragma endregion CONTROL

#pragma region SENSORS

#define DHTTYPE DHT11
// DHTTYPE = DHT11, but there are also DHT22 and 21

DHT dht(DHTPIN, DHTTYPE); // constructor to declare our sensor
                          // static SemaphoreHandle_t dht_mutex;

#pragma endregion SENSORS

#pragma endregion DEFINITIONS

/*
 * Device running tasks definitions
 */
#pragma region TASKS

#pragma region CONFIG_TASKS

/*
  Listens for app_config Lifecycle == Lifecycle::RESTART_TASK

  if TRUE: Shut Downs all functions and restarts esp
*/
void vRestartTask(void *params)
{
  LabLogger *logger = (LabLogger *)params;

  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::RESTART_TASK)
    {
      logger->log("[MAIN]:[INF]: Lifecycle: RESTART");

      app_config.Lifecycle == Lifecycle::FREE_CLOCK;

      logger->shutDown();
      app_server.destroy();

      app_config.Lifecycle == Lifecycle::NORMAL;

      esp_restart();
    }

    vTaskDelay(task_delay);
  }
}

#pragma endregion CONFIG_TASKS

#pragma region DISPATCHING_TASKS

void vLightTask(void *params)
{
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      light_control.roll(RuleValidator::ApplyRules(&app_control.CONTROLS[ActionFunctionIDX::LIGHT_SOURCE], &app_control.CONTROLS[ActionFunctionIDX::LIGHT_SOURCE], &app_control.SENSORS));
    }
    vTaskDelay(task_delay);
  }
}

void vFanExhaustTask(void *params)
{
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      fanExhaus_control.roll(RuleValidator::ApplyRules(&app_control.CONTROLS[ActionFunctionIDX::EXHAUS_FAN], &app_control.CONTROLS[ActionFunctionIDX::EXHAUS_FAN], &app_control.SENSORS));
    }
    vTaskDelay(task_delay);
  }
}

void vFanIntakeTask(void *params)
{
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      fanIntake_control.roll(RuleValidator::ApplyRules(&app_control.CONTROLS[ActionFunctionIDX::INTAKE_FAN], &app_control.CONTROLS[ActionFunctionIDX::INTAKE_FAN], &app_control.SENSORS));
    }
    vTaskDelay(task_delay);
  }
}

void vOutputPumpTask(void *params)
{
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      outputPump_control.roll(RuleValidator::ApplyRules(&app_control.CONTROLS[ActionFunctionIDX::OUTPUT_PUMP], &app_control.CONTROLS[ActionFunctionIDX::OUTPUT_PUMP], &app_control.SENSORS));
    }
    vTaskDelay(task_delay);
  }
}

void vInputPumpTask(void *params)
{
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      inputPump_control.roll(RuleValidator::ApplyRules(&app_control.CONTROLS[ActionFunctionIDX::INPUT_PUMP], &app_control.CONTROLS[ActionFunctionIDX::INPUT_PUMP], &app_control.SENSORS));
    }
    vTaskDelay(task_delay);
  }
}

void vOxygenPumpTask(void *params)
{
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      oxygenPump_control.roll(RuleValidator::ApplyRules(&app_control.CONTROLS[ActionFunctionIDX::OXYGEN_PUMP], &app_control.CONTROLS[ActionFunctionIDX::OXYGEN_PUMP], &app_control.SENSORS));
    }
    vTaskDelay(task_delay);
  }
}

#pragma endregion DISPATCHING_TASKS

#pragma region SENSOR_TASKS

void vDHTTask(void *params)
{
  LabLogger *logger = (LabLogger *)params;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  const TickType_t xFrequency = task_delay; // delay for mS
  while (true)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      app_control.SENSORS.temperature = dht.readTemperature();
      app_control.SENSORS.humidity = dht.readHumidity();
    }

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }

  vTaskDelete(NULL);
}

// TODO: #92 Implement limits handling in main

void vTankLevelTask(void *params)
{
  LabLogger *logger = (LabLogger *)params;

  while (1)
  {
    if (app_config.Lifecycle == Lifecycle::NORMAL)
    {
      // TODO: validate potential danger readings and change app_config.Lifecycle

      digitalWrite(TANK_LEVEL_POWER_PIN, HIGH); // turn the sensor ON
      // vTaskDelay(WATER_MESURMENT_DELAY);             // wait 10 milliseconds
      int value = analogRead(TANK_LEVEL_SIGNAL_PIN); // read the analog value from sensor
      digitalWrite(TANK_LEVEL_POWER_PIN, LOW);       // turn the sensor OFF

      app_control.SENSORS.waterTankLevel = map(value, WATER_SENSOR_SENSOR_MIN, WATER_SENSOR_SENSOR_MAX, 0, 4); // 4 levels
    }

    vTaskDelay(task_delay);
  }
}

#pragma endregion SENSOR_TASKS

#pragma region SETUP

void setup()
{

#pragma region PINS_SETUP
  // Setup Pins

  // Tank Level
  pinMode(TANK_LEVEL_POWER_PIN, OUTPUT);
  digitalWrite(TANK_LEVEL_SIGNAL_PIN, LOW);

  fanIntake_control.setup(INTAKE_FAN_PIN, OUTPUT, ControlState::SWITCH_ON);
  fanIntake_control.setupLimit(&app_limits, &app_control, &app_config);

  fanExhaus_control.setup(EXHAUST_FAN_PIN, OUTPUT, ControlState::SWITCH_ON);
  fanExhaus_control.setupLimit(&app_limits, &app_control, &app_config);

  inputPump_control.setup(PUMP_ONE_PIN, OUTPUT, ControlState::SWITCH_OFF);
  inputPump_control.setupLimit(&app_limits, &app_control, &app_config);

  outputPump_control.setup(PUMP_TWO_PIN, OUTPUT, ControlState::SWITCH_OFF);
  outputPump_control.setupLimit(&app_limits, &app_control, &app_config);

  oxygenPump_control.setup(OXYGEN_PUMP_PIN, OUTPUT, ControlState::SWITCH_OFF);
  oxygenPump_control.setupLimit(&app_limits, &app_control, &app_config);

  light_control.setup(LIGHT_PIN, OUTPUT, ControlState::SWITCH_OFF);
  light_control.setupLimit(&app_limits, &app_control, &app_config);

#pragma endregion PINS_SETUP
// TODO: Create exciting UI
#pragma region SERIAL_SETUP

  // Global DEBUG
  if (MODE == LogType::DEBUG_LEVEL)
  {
    Serial.begin(115200);
  }

#pragma endregion SERIAL_SETUP

#pragma region SD_CARD_SETUP

  // Set SD Flash Storage
  //while (!SD.begin(SD_SS_PIN, SPI, 4000000, "/sd", 20))
    while (!SD.begin(SD_SS_PIN,SPI))
    {
      if (MODE == LogType::DEBUG_LEVEL)
      {
        /*
          Only in DEBUG log can be seen!
          Only when SD Flash is ready then the logger
          can be used
        */
        Serial.println("[MAIN]:[INF]: SD: Memory Initialization.....");
      }
    }

#pragma endregion SD_CARD_SETUP
  // TODO: #120 Check if SPIFFS is used in ESP Architecture
  // while (!SPIFFS.begin())
  // {
  //   if (MODE == LogType::DEBUG_LEVEL)
  //   {
  //     /*
  //       Only in DEBUG log can be seen!
  //       Only when SD Flash is ready then the logger
  //       can be used
  //     */
  //     Serial.println("[MAIN]:[INF]: SPIFFS: Memory Initialization.....");
  //   }
  // }

    // File file = SD.open("/data/ui/main.js", FILE_READ, false);
    // if (!file)
    // {
    //   Serial.println("[AppControl]:[ERR]: Failed to create file");
    //   return;
    // }

    // if (file)
    // {
    //   // Create a buffer to store the data
    //   const size_t bufferSize = 1024; // Adjust the buffer size as needed
    //   uint8_t buffer[bufferSize];

    //   while (file.available())
    //   {
    //     // Read data into the buffer
    //     size_t bytesRead = file.read(buffer, bufferSize);

    //     // Process the data in the buffer
    //     // (e.g., you can send it over a network, save it to SPIFFS, etc.)

    //     // In this example, we'll just print the data to the Serial monitor
    //     Serial.write(buffer, bytesRead);
    //   }

    //   // Close the file when done
    //   file.close();
    // }
    // else
    // {
    //   Serial.println("Failed to open the file");
    // }

    // return;


#pragma region APP_LOGGER_SETUP

  app_logger.begin();

#pragma endregion APP_LOGGER_SETUP

#pragma region STORAGE_SETUP_AND_WATCH

  // Set app settings change_detection
  app_config_mapper.MapFromStorage(SD, app_config.APP_CONFIG_FILE_PATH, &app_config);

  // Register tasks, queues and etc. only if app is configured
  if (app_config.IS_CONFIRMED)
  {
    app_notification_mapper.MapFromStorage(SD, "telegram", &app_notifications.notifications[Notification::TELEGRAM_NOTIFICATION]);

    // TODO: #121 Check if AppConfigRefreshState::SPIFFS_REFRESH is used
    /*
      SPIFFS Refresh from previous run
    */
    if (app_config.REFRESH_STATE == AppConfigRefreshState::SPIFFS_REFRESH)
    {

      app_logger.log("[MAIN]:[INF]: Open Data ZIP");
      app_config.Lifecycle = Lifecycle::FREE_CLOCK;

      FileUtility::deleteAllFiles(SD, "/data", 10);

      zipUtility.openZip(app_config.UPLOAD_SD_FILE_PATH.c_str());

      app_logger.log("[MAIN]:[INF]: Deleting Data ZIP");

      FileUtility::deleteFile(SD, app_config.UPLOAD_SD_FILE_PATH.c_str());

      app_config_t refresh_settings;
      app_config_mapper.MapToStorage(app_config.APP_CONFIG_FILE_PATH, &refresh_settings);

      app_logger.log("[MAIN]:[INF]: SD Refreshed! Restarting ...");

      esp_restart();
    }

    // Maps all limits to app_limits
    app_limit_mapper.MapAllFromStorage(SD, &app_limits);

    telegram_notifier = new TelegramNotifier(&app_logger, &app_notifications.notifications[Notification::TELEGRAM_NOTIFICATION], &app_config);

    light_control.addNotifications(telegram_notifier, &app_notifications);
    fanIntake_control.addNotifications(telegram_notifier, &app_notifications);
    fanExhaus_control.addNotifications(telegram_notifier, &app_notifications);
    inputPump_control.addNotifications(telegram_notifier, &app_notifications);
    outputPump_control.addNotifications(telegram_notifier, &app_notifications);
    oxygenPump_control.addNotifications(telegram_notifier, &app_notifications);

    // Wait to complete all setups!
    vTaskDelay(task_delay);

#pragma endregion STORAGE_SETUP_AND_WATCH

#pragma region ACTION_TASKS_REGISTER

    xTaskCreatePinnedToCore(
        vRestartTask,
        "vRestartTask",
        7000,
        &app_logger,
        1,
        NULL,
        0);

    // DHC
    xTaskCreatePinnedToCore(
        vDHTTask,
        "vDHTTask",
        10000,
        &app_logger,
        1,
        NULL,
        0);

    // // WATER LEVEL
    xTaskCreatePinnedToCore(
        vTankLevelTask,
        "vTankLevelTask",
        10000,
        &app_logger,
        1,
        NULL,
        0);

    vTaskDelay(task_delay);

#pragma endregion ACTION_TASKS_REGISTER

#pragma region DISPATCHING_TASKS

    xTaskCreatePinnedToCore(
        vLightTask,
        "vLightTask",
        7000,
        NULL,
        1,
        NULL,
        0);

    xTaskCreatePinnedToCore(
        vInputPumpTask,
        "vInputPumpTask",
        7000,
        NULL,
        1,
        NULL,
        0);

    xTaskCreatePinnedToCore(
        vOutputPumpTask,
        "vInputPumpTask",
        7000,
        NULL,
        1,
        NULL,
        0);

    xTaskCreatePinnedToCore(
        vOxygenPumpTask,
        "vOxygenPumpTask",
        7000,
        NULL,
        1,
        NULL,
        0);

    xTaskCreatePinnedToCore(
        vFanIntakeTask,
        "vFanIntakeTask",
        7000,
        NULL,
        1,
        NULL,
        0);

    xTaskCreatePinnedToCore(
        vFanExhaustTask,
        "vFanExhaustTask",
        7000,
        NULL,
        1,
        NULL,
        0);

#pragma endregion DISPATCHING_TASKS

#pragma region TIME

    Rtc.Begin();

    // Global Time Setup
    // TODO: #111 Time is acting strange!
    RtcDateTime dt = RtcDateTime(__DATE__, __TIME__);

    rtc.setTime(dt.Second(), dt.Minute(), dt.Hour(), dt.Day(), dt.Month() + 1, dt.Year());

    app_time.HOUR = rtc.getHour();
    app_time.MINUTES = rtc.getMinute();
    app_time.SECONDS = rtc.getSecond();

    app_logger.log(rtc.getTime("[MAIN]:[INF]: Started At: %A, %B %d %Y %H:%M:%S").c_str());

#pragma endregion TIME
  }
  else
  {
    app_ui_mapper.MapToWiFiAP(&app_config);
  }

  // if is Configured run app else credentials
  // TODO: #71 Found bug in long running server, maybe the task is stopped or canceled if is not used for long
  app_server.begin(SD);

  vTaskDelete(NULL);
}

#pragma endregion SETUP

#pragma region LOOP

void loop()
{
}

#pragma endregion LOOP