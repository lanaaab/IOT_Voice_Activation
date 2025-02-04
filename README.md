### Marvin – Smart Voice-Activated Light Bulb developed by: 
**Lana Abu Romi**  
**Anan Khateb**  
**Daniel Peleg**
  
## Details about our project
This IoT project features a **smart lighting bulb** that responds to **voice activation** using the wake word **"Marvin."** Designed for **accessibility**, it assists **amputees, individuals with mobility limitations, and those with visual impairments** by enabling **hands-free control** of their lighting environment.  

Users can **adjust brightness, change colors, schedule lighting,** and **activate saved lighting modes** for different needs. Additionally, **Marvin can announce the current time and weather,** providing useful information without the need for a separate device. By eliminating the need for **physical switches or mobile apps**, **Marvin enhances independence and convenience** for users who may struggle with traditional lighting controls.  

The **wake word detection** is powered by a **neural network**, while **Google's speech-to-text service** processes user commands, ensuring a **seamless and responsive experience**.


## ✨ Features  

### 🎤 Voice Control  
- Activate the light using the **wake word "Marvin."**  
- Turn the light **on/off** with voice commands.  
- Adjust **brightness** and **color** using speech.  

### ⏳ Scheduling & Timers  
- **Schedule** the light to turn **on/off automatically** at a set time.  
- **Set a timer** to turn off the light in **X minutes.**  
- **Cancel scheduled actions** if needed.  

### 📢 Command Feedback  
- **Audio feedback** after every command, including error reporting.  
- **Visual indicator** to show when to speak, errors, and system status.  

### 📡 Wi-Fi Manager  
- Uses a **Wi-Fi management library** for seamless **network configuration and connectivity handling.**  

### 🔊 Customizable Experience  
- **Custom default color** for scheduled activations and emergency mode.  
- **Adjustable speaker volume** for voice responses and alerts.  

### 🌍 Smart Announcements  
- Hear the **current time and weather** through voice output.  

### 🎭 Preset Scenes  
- Activate lighting modes like **reading, relaxing, or party mode.**  

### 🚨 Emergency Mode  
- **Send an SMS alert** to a selected contact.  
- **Blink red light** and **play a loud sound** to alert people nearby.  


## Folder description:
* ESP32: source code for the esp side (firmware).
* Documentation: wiring diagram + basic operating instructions
* Unit Tests: tests for individual hardware components (input / output devices)
* Parameters: contains description of configurable parameters 
* Assets: 3D printed parts, Audio files used in this project

## Hardware Components:
* Esp32
* Adafruit neopixel 12 ring 
* RGB LED
* OPEN_SMART mp3 player board, built-in amplifier 
* INMP441 Microphone
* Button

## Arduino/ESP libraries installed for the project:
* EspSoftwareSerial - version 6.16.1
* ArduinoJson - version 7.3.0
* Adafruit NeoPixel - version 1.12.3
* WiFiManager - version 2.0.17

## Connection Diagram:
![Connection Diagram](https://github.com/user-attachments/assets/f96d4047-e5e3-4680-9c84-886f65507650)


## Project Poster:
 
This project is part of ICST - The Interdisciplinary Center for Smart Technologies, Taub Faculty of Computer Science, Technion
https://icst.cs.technion.ac.il/
