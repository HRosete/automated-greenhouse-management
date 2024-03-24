# Automated Greenhouse Management System

This repository contains the Arduino code and Python script for an automated greenhouse management system. The system utilizes various sensors and actuators to create a controlled environment for your plants.


## Features

-   **Sensor Monitoring:** Reads temperature, humidity, light levels, and soil moisture using DHT22 sensors, LDR sensors, and analog moisture sensors.
-   **Time-based Control:** Turns on lights based on time and light sensor readings.
-   **Temperature Regulation:** Activates fans based on temperature thresholds.
-   **Irrigation Control:** Opens a valve for watering during specific hours based on soil moisture readings (optional).
-   **Stepper Motor Control:** Controls a stepper motor for potential tasks within the greenhouse (movement pattern depends on implementation).
-   **Data Logging:** Prints sensor readings to the serial monitor and logs data to a MySQL database (requires configuration).


### Hardware Requirements

-   Arduino Uno or compatible board
-   DHT22 sensors (x2)
-   LDR sensors (x2)
-   Analog moisture sensors (x2) (optional)
-   Stepper motor
-   Driver for the stepper motor
-   Other components like fans, valves, and relays depending on your implementation


### Software Requirements

-   Arduino IDE
-   Python 3
-   MySQL database (optional)


## Getting Started

1. Clone this repository.
2. Install the required libraries (refer to code comments).
3. Configure pin connections and database details (if applicable) according to your hardware setup.
4. Upload the Arduino code to your board.
5. Run the Python script to establish serial communication and manage data logging.

**Note:** This is a basic framework, and additional functionalities or hardware components can be integrated based on your specific needs.

**_Feel free to explore the code, modify it for your requirements, and contribute to this project!_**
