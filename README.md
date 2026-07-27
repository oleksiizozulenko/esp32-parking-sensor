# ESP32 Parking Sensor Project

This project is a simple embedded parking-distance indicator built around an ESP32 board. It uses an ultrasonic sensor to measure distance and provides feedback through an LCD screen, a buzzer, and an LED.

## What it does

The device continuously measures the distance to an object using an HC-SR04 ultrasonic sensor and:

- shows the measured distance on a 16x2 LCD display
- activates a buzzer when the object gets too close
- lights an LED as a visual warning

## Hardware

The project is wired for:

- ESP32 development board
- HC-SR04 ultrasonic sensor
- I2C LCD display
- buzzer
- red LED

## Software stack

- PlatformIO
- Arduino framework
- ESP32 board support
- LiquidCrystal_I2C library

## Project files

- [src/main.cpp](src/main.cpp) - main firmware logic
- [platformio.ini](platformio.ini) - PlatformIO configuration
- [diagram.json](diagram.json) - Wokwi circuit diagram
- [wokwi.toml](wokwi.toml) - Wokwi firmware settings

## Build and upload

1. Install PlatformIO and the VS Code PlatformIO extension.
2. Open the project folder in VS Code.
3. Build the firmware:

```bash
pio run
```

4. Upload to the ESP32 board:

```bash
pio run --target upload
```

## Run in Wokwi

This project also includes a Wokwi simulation setup.

1. Open the project in Wokwi or use the Wokwi extension.
2. Load [diagram.json](diagram.json).
3. Build and flash the firmware from PlatformIO first so the simulation can use the generated binary.

## Notes

- The firmware currently uses the ESP32 GPIO pins defined in [src/main.cpp](src/main.cpp).
- If you change the hardware wiring, update the pin definitions accordingly.
- The Wokwi configuration is included as a starting point for simulation and may need adjustment depending on your local build output path.

## License

This project is for personal experimentation and learning purposes.
