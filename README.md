# gbaby.tech.mcu

This repository contains the code for the *gbaby.tech.mcu* project, developed for the ESP32 microcontroller. The project is designed to manage a gardening system by controlling various devices, such as water pumps, lights, and ventilation systems. Additionally, it provides a server endpoint for handling different tasks remotely, enabling smart automation of your garden.

## Project Overview

The *gbaby.tech.mcu* project utilizes the ESP32 microcontroller to automate and monitor key aspects of a gardening system. The system is capable of managing water pumps for irrigation, controlling lights for optimal plant growth, and adjusting ventilation to maintain proper airflow. The ESP32 acts as the central control unit, with a web server providing endpoints to handle and trigger various tasks remotely.

### Key Features

- **Water Pump Control**: Automatically or manually control water pumps for irrigation based on user inputs or preset schedules.
- **Lighting Control**: Manage lights to ensure optimal lighting conditions for plant growth.
- **Ventilation Control**: Regulate ventilation to maintain the correct temperature and humidity levels in the garden.
- **Remote Task Management**: The ESP32 exposes a server endpoint to handle different tasks, including:
  - Turning on/off pumps, lights, and ventilation.
  - Scheduling and automation for tasks such as watering and lighting cycles.
  - Receiving data from sensors for monitoring environmental conditions.

### Architecture

- **ESP32 Microcontroller**: Acts as the core device that manages all hardware and communicates with the server endpoints.
- **Web Server**: Provides a simple API to interact with the system remotely. The endpoints are designed to manage tasks like turning devices on/off or querying system status.
- **Hardware**:
  - **Water Pumps**: Controlled through GPIO pins, connected to the ESP32.
  - **Lights**: Managed via relays or transistors controlled by the ESP32.
  - **Ventilation**: Fans or ventilation systems connected and controlled via relays or transistors.
  
### Use Cases

- **Automated Gardening**: The system can run autonomously, managing irrigation, lighting, and ventilation based on sensor data or predefined schedules.
- **Remote Monitoring**: Users can check the status of their gardening system and adjust settings remotely through the web interface.
- **Smart Home Integration**: The project can be extended for integration with other smart home platforms for a more comprehensive garden automation system.

## Installation

### Hardware Requirements

- **ESP32 Microcontroller**: The core of the system.
- **Relay Modules**: For controlling water pumps, lights, and ventilation.
- **Water Pumps**: For irrigation control.
- **Lights**: For controlling garden lighting.
- **Ventilation Fans**: For managing airflow.

### Software Requirements

1. **VS Code**: The code is developed using the VS Code and Platform IO, which is compatible with the ESP32.
2. **Libraries**: The project uses specific libraries for controlling hardware and managing server endpoints. Ensure you have the necessary libraries installed in the Arduino IDE.

### Setup Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/gbaby.tech.mcu.git
