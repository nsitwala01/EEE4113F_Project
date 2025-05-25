# Automated Sprinkler-Based Predator Deterrent System

## Overview

This project presents an autonomous, non-lethal predator deterrent system designed to protect African penguins at the Boulders Penguin Colony from terrestrial predators such as honey badgers and leopards. The system combines motion detection, machine learning-based image classification, and a water-based deterrent to provide a safe and effective conservation solution.

## Features

- 🐧 Motion detection using a PIR sensor  
- 📷 Image capture and classification via ESP32-CAM and TensorFlow  
- 💧 Automated sprinkler deterrent system  
- 🌐 Web server dashboard for real-time monitoring  
- ☀️ Low-cost, solar-compatible, and energy-efficient  
- 🌱 Environmentally friendly and minimally invasive to penguins  

## Subsystems

### 1. Predator Detection

- ESP32-CAM microcontroller with built-in OV2640 camera  
- HC-SR501 PIR sensor for motion detection  
- Flask web server receives and classifies images using a TensorFlow model  
- Activates deterrent only for known predators (leopards and honey badgers)

### 2. Predator Deterrent

- Water tank (gravity-fed), DC water pump, and Rainjet 360° sprinkler  
- Solenoid valve with delay timer  
- Controlled by GPIO output from the detection system  
- Sprays water for a fixed duration upon predator confirmation

## Hardware Components

- ESP32-CAM module  
- HC-SR501 PIR motion sensor  
- 12V DC water pump (DS3502HF)  
- Rainjet 360° micro sprinkler  
- HKD solenoid valve + delay timer  
- Water storage tank (min. 0.32 m height for pressure)  
- 12V power supply (battery or solar)

## Software

- Embedded C/C++ code for ESP32-CAM (Arduino IDE)  
- Python Flask server for receiving and classifying images  
- TensorFlow ML model for predator classification  
- Basic HTML/CSS UI for server dashboard

## Setup Instructions

1. **Assemble Hardware**  
   - Connect ESP32-CAM with PIR sensor and relay module.  
   - Connect solenoid valve, water pump, and timer to the power circuit.  

2. **Flash Firmware**  
   - Flash the ESP32-CAM with the provided Arduino code (see `/firmware`).  

3. **Run Server**  
   - Navigate to `/web-server` and run `app.py`.  
   - Ensure TensorFlow and Flask are installed.  

4. **Deploy Model**  
   - Place your trained model (e.g., `model.h5`) in the server directory.  
   - Test with sample images for accuracy.  

## Results

- ✅ 96.4% image classification accuracy (leopard, honey badger, human)  
- ✅ PIR sensor detects motion up to 6.5 meters  
- ✅ Sprinkler response activates within 1 second 
- 🔋 Peak current: < 1.0 A  

## Authors

- Masixole Ntshangase (NTSMAS030)  
- Nang’alelwa Sitwala (STWNAN001)  

> Developed for EEE4113F - Department of Electrical Engineering, University of Cape Town, May 2025.
