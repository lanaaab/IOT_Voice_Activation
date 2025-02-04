# Marvin – Smart Voice-Activated Light Bulb 
### 👨‍💻 Developed by  
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
| Component           | Photo |
|---------------|------|
| Esp32 | ![31165e52-3b7b-4ef5-be98-202e6b4c33a5](https://github.com/user-attachments/assets/d8305376-db23-490a-a731-9189dd4170c1)|
| Adafruit neopixel 12 Ring  | ![a497ca2f-a295-4f07-9367-f1e7f704b138](https://github.com/user-attachments/assets/02b20ee7-45f9-4b02-8c39-3dd2a4a4e334)|
| RGB LED | ![b89c2bec-c4cd-4c79-a9ed-ed24de0e8c4f](https://github.com/user-attachments/assets/313be542-a188-4eb3-a3e1-c77116a4e6da)|
| OPEN_SMART mp3 player board, built-in amplifier  | ![5edff7b9-d026-4ebc-8c4d-e24fd955b6d4](https://github.com/user-attachments/assets/6d7599bf-787b-49b0-966b-e96c12946cf4)|
| INMP441 Microphone  | ![dd351aae-5479-4e0d-bb66-febf72616b36](https://github.com/user-attachments/assets/52964047-7aee-49e4-b670-14926ab3ac3f)|
| Button | ![9edc9130-1cfb-4144-bbbb-d071262db454](https://github.com/user-attachments/assets/b1f34372-a740-4f1c-bce1-671a209c3057)|


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
