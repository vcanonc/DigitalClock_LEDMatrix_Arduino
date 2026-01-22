# ⏰ LED Matrix Digital Clock – Arduino

This project is a digital clock built using an Arduino Uno and a LED matrix display. It shows the current time and date, automatically adjusts brightness based on ambient light, and triggers audible alerts at predefined hours using a buzzer.

The project focuses on real-time clock management, visual output on LED matrices, and simple environmental interaction through sensors.

## ✨ Features

- **Time Display**  
  Shows the current hour and minute with a blinking colon effect.

- **Date Display**  
  Displays the full date (day of the week, day, month, and year) periodically.

- **RTC-Based Timekeeping**  
  Uses an external RTC module to maintain accurate time independently of power cycles.

- **Automatic Brightness Control**  
  Adjusts LED matrix intensity automatically based on ambient light conditions.

- **Audible Alerts**  
  Triggers a buzzer alarm at predefined hours of the day.

## 🔧 Hardware Components

- **Microcontroller:** Arduino Uno  
- **Display:** LED Matrix (MAX7219-based)  
- **RTC Module:** DS3231  
- **Light Sensor:** Digital light sensor module  
- **Buzzer:** Active buzzer  
- **Communication:** SPI (LED matrix), I2C (RTC)

## 🧠 System Behavior

- The clock updates once per second and avoids unnecessary redraws.
- The colon between hours and minutes blinks every second.
- Time is shown by default, while the date is displayed once per minute using a scrolling animation.
- Brightness is reduced automatically when low ambient light is detected.
- Audible alerts are triggered at **06:00**, **12:00**, and **19:00**.

## 🎛️ Display and Animations

The project uses the **MD_Parola** and **MD_MAX72xx** libraries to handle:
- Text rendering
- Custom fonts
- Smooth scrolling and transition animations

Different animations are used depending on whether the display is switching between time and date.

## 🔌 Power Requirements

The system operates at **5V**, provided directly to the Arduino Uno and peripherals.

## 🗂️ Project Structure

This repository contains:
- The main Arduino sketch
- Custom font definition
- Display control logic
- RTC handling and alarm logic
- Light-based brightness control

The code is structured around clear functional blocks to allow reuse of individual features in other Arduino-based projects.

## 🎯 Purpose and Scope

This project was developed as a practical embedded systems exercise, combining real-time clock management, visual output on LED matrices, and basic sensor integration. It is intended for educational and personal use.

## 📄 License

This project is licensed under the **MIT License**.  
You are free to use, modify, and distribute this software, provided that the original copyright notice and license are included.

See the `LICENSE` file for more details.
