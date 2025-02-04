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


## 📂 Project Structure  

- **ESP32/** – Source code for the ESP32 firmware.  
- **Documentation/** – Wiring diagram and basic operating instructions.  
- **Unit Tests/** – Tests for individual hardware components (input/output devices).  
- **Parameters/** – Description of configurable parameters.  
- **Assets/** – 3D printed parts and audio files used in this project.


## Hardware Components  

| Component                          | Photo |
|------------------------------------|------|
| **ESP32**                          | <img src="https://github.com/user-attachments/assets/d8305376-db23-490a-a731-9189dd4170c1" width="100"> |
| **Adafruit Neopixel 12 Ring**      | <img src="https://github.com/user-attachments/assets/02b20ee7-45f9-4b02-8c39-3dd2a4a4e334" width="100"> |
| **RGB LED**                        | <img src="https://github.com/user-attachments/assets/313be542-a188-4eb3-a3e1-c77116a4e6da" width="100"> |
| **OPEN_SMART MP3 Player Board**    | <img src="https://github.com/user-attachments/assets/6d7599bf-787b-49b0-966b-e96c12946cf4" width="100"> |
| **INMP441 Microphone**             | <img src="https://github.com/user-attachments/assets/52964047-7aee-49e4-b670-14926ab3ac3f" width="100"> |
| **Button**                         | <img src="https://github.com/user-attachments/assets/b1f34372-a740-4f1c-bce1-671a209c3057" width="100"> |


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
