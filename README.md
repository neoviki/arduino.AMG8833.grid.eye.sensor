# AMG8833 Grid-Eye Sensor Interfacing with Arduino Due

This code demonstrates how to interface the AMG8833 thermal sensor with the Arduino Due to read thermal data and test both negative and positive temperatures via I2C.

## Requirements

- **Hardware:**
  - Arduino Due
  - AMG8833 Thermal Sensor (Grid-Eye)
  - I am using a custom breakout board

- **Software:**
  - Arduino IDE
  - Adafruit AMG8833 Library (or another compatible library)

## About the AMG8833 Grid-Eye Sensor

The **AMG8833 Grid-Eye** is an 8x8 thermal infrared sensor array that can detect and measure the temperature of objects in its field of view. It is capable of capturing thermal data with a resolution of 8x8 pixels, providing a total of 64 temperature readings. The sensor uses infrared (IR) sensing technology and can detect temperatures in the range of -20°C to +80°C with a high degree of accuracy. 

The AMG8833 communicates over I2C, making it easy to interface with microcontrollers like the Arduino Due. This sensor is commonly used in applications such as temperature monitoring, thermal imaging, and proximity detection.
