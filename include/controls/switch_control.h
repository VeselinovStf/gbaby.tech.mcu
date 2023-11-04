#ifndef SWITCH_CONTROL_H
#define SWITCH_CONTROL_H

#include <Arduino.h>
#include "controls/control_state.h"
#include "core/app_history.h"

class SwitchControl
{
public:
    SwitchControl(uint8_t id, String name, AppHistory *history)
    {
        this->id = id;
        this->name = name;
        this->history = history;
    };

    /*
        Setups Control

        @param pin pin - Device GPIO
        @param mode - OUTPUT/INPUT
        @param control_state - Initial State

        The control pinMode is set, then inital pin state is set, ehen switch is set to SWITCH_OFF
    */
    void setup(uint8_t pin, uint8_t mode, bool off_value)
    {
        this->pin = pin;
        this->on_value = !off_value;
        this->off_value = off_value;

        pinMode(pin, mode);

        digitalWrite(pin, off_value);

        this->control_state = ControlState::SWITCH_OFF;
    }

    void addNotifications(TelegramNotifier *notifiers, app_notification_t *app_notifications)
    {
        this->use_notifications = true;
        this->app_notifications = app_notifications;
        this->notifiers = notifiers;
    }

    void setupLimit(app_limit_t *app_limit, app_control_t *app_control, app_config_t *app_config)
    {
        this->use_limit = true;
        this->app_limit = app_limit;
        this->app_control = app_control;
        this->app_config = app_config;
    }

    void On(uint8_t value = HIGH)
    {
        if (this->use_limit)
        {
            if (!AppLimitValidator::validate(this->app_limit, this->app_control))
            {
                this->Off();

                if (this->use_notifications)
                {
                    this->notify("[GROW-BOX]:[LIMIT]: " + this->name);
                }

                for (uint8_t i = 0; i < TOTAL_CORE_FUNCTIONS; i++)
                {
                    this->app_control->CONTROLS[i].status = 0;
                }

                return;
            }
        }

        if (!this->control_state)
        {
            this->control_state = !this->control_state;
            digitalWrite(this->pin, this->on_value);

            if (this->use_notifications)
            {
                this->notify("[GROW-BOX]:[ON]: " + this->name);
            }

            app_control_t c;
            for (uint8_t i = 0; i < TOTAL_CORE_FUNCTIONS; i++)
            {
                c.CONTROLS[i] = this->app_control->CONTROLS[i];
                if (this->app_control->CONTROLS[i].unit == this->name){
                    c.CONTROLS[i].status = 1;
                }
                   
            }
            c.SENSORS = this->app_control->SENSORS;
            
            this->history->save(SD, this->name, &c);
        }
    };

    void Off(uint8_t value = LOW)
    {
        if (this->control_state)
        {
            this->control_state = !this->control_state;
            digitalWrite(this->pin, this->off_value);

            if (this->use_notifications)
            {
                this->notify("[GROW-BOX]:[OFF]: " + this->name);
            }

            app_control_t c;
            for (uint8_t i = 0; i < TOTAL_CORE_FUNCTIONS; i++)
            {
                c.CONTROLS[i] = this->app_control->CONTROLS[i];
                if (this->app_control->CONTROLS[i].unit == this->name)
                {
                    c.CONTROLS[i].status = 0;
                }
            }
            c.SENSORS = this->app_control->SENSORS;

            this->history->save(SD, this->name, &c);
        }
    };

    void roll(uint8_t v)
    {
        if (v == ControlState::SWITCH_ON)
        {
            this->On();
        }
        else
        {
            this->Off();
        }
    }

    uint8_t checkActionFunction(uint8_t check_action_function)
    {
        return id == this->id;
    }

private:
    bool control_state;
    bool use_limit;
    app_config_t *app_config;
    uint8_t pin;
    uint8_t id;
    uint8_t on_value;
    uint8_t off_value;
    app_limit_t *app_limit;
    app_control_t *app_control;
    app_notification_t *app_notifications;
    uint8_t use_notifications;
    TelegramNotifier *notifiers;
    String name;
    AppHistory *history;

        void
        notify(const String &message)
    {
        // String message(from); // Convert the first C-string to a C++ std::string
        // message += m;

        // Notification Mechanism for Control
        if (this->use_notifications)
        {
            for (uint8_t j = 0; j < TOTAL_NOTIFICATIONS; j++)
            {
                if (this->app_notifications->notifications[j].status == NotificationState::ON_NOTIFICATION)
                {
                    this->notifiers->send(message);
                }
            }
        }
    }

    // void getState(uint8_t status){
    //     for (uint8_t i = 0; i < TOTAL_CORE_FUNCTIONS; i++)
    //     {
    //         if (this->name == this->app_control->CONTROLS[i].unit){
    //             return this->app_control->CONTROLS[i].status;
    //         }
               
    //     }
    // }
};

#endif