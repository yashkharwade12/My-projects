# 🏥 Touch-Based Device Control System for Bedridden Patients

An Embedded C project developed using the **LPC2148 ARM7 Microcontroller** that enables bedridden patients to control household devices through a **Resistive Touch Screen**. The system includes **password authentication**, **EEPROM-based password storage**, **LCD display**, and an **emergency buzzer** for patient safety. 

---

# 📌 Project Overview

Patients with limited mobility often depend on others to operate basic electrical devices such as lights, fans, or emergency alarms.

This project provides a secure and user-friendly touch-based control system where:

- A patient enters a password using a 4×4 keypad.
- The password is verified using EEPROM.
- After successful authentication, the touch screen becomes active.
- The patient can control connected devices by touching specific screen locations.
- An emergency buzzer can be activated instantly when assistance is required.
- Passwords can be updated and stored permanently in EEPROM.

---

# ✨ Features

- 🔒 Password Protected Access
- 📱 Resistive Touch Screen Interface
- 💾 EEPROM Password Storage
- 🖥️ 16×2 LCD Display
- ⌨️ 4×4 Matrix Keypad
- 💡 Device Control (LED1 & LED2)
- 🚨 Emergency Buzzer
- 🔄 Password Change Option
- 👨‍⚕️ Designed for Bedridden Patients

---

# 🛠 Hardware Components

| Component | Description |
|-----------|-------------|
| LPC2148 | ARM7 Microcontroller |
| Resistive Touch Screen | User Input |
| 16×2 LCD | Display |
| 4×4 Matrix Keypad | Password Entry |
| AT25LC512 EEPROM | Password Storage |
| LED1 | Device 1 |
| LED2 | Device 2 |
| Buzzer | Emergency Alert |
| 3.3V Power Supply | System Power |

The project requires the LPC2148, resistive touch screen, LCD, keypad, EEPROM, LEDs, and buzzer as its primary hardware components. :contentReference[oaicite:1]{index=1}

---

# 💻 Software Requirements

- Embedded C
- Keil uVision IDE
- Flash Magic
- Proteus (Simulation)

---

# 📂 Project Structure

```
Touch-Based-Device-Control/
│
├── main.c
├── lcd.c
├── lcd.h
├── keypad.c
├── keypad.h
├── uart.c
├── uart.h
├── touch.c
├── touch.h
├── eeprom.c
├── eeprom.h
├── delay.c
├── delay.h
├── README.md
```

---

# ⚙️ System Workflow

```
Power ON
     │
     ▼
Initialize LCD
Initialize UART
Initialize EEPROM
Initialize Keypad
Initialize Touch Screen
     │
     ▼
Enter Password
     │
     ▼
Password Correct?
 ┌───────────────┐
 │               │
No             Yes
 │               │
Retry       Enable Touch Screen
                 │
                 ▼
      Touch Device Icons
       │            │
       ▼            ▼
 Device1 ON/OFF  Device2 ON/OFF
                 │
                 ▼
          Emergency Buzzer
                 │
                 ▼
          Password Update
                 │
                 ▼
              System Ready
```

---

# 📖 Working Principle

### Step 1

The system initializes all peripherals including:

- LCD
- UART
- EEPROM
- Touch Screen
- Keypad

---

### Step 2

The user enters a password using the keypad.

---

### Step 3

The entered password is compared with the password stored inside the EEPROM.

- If incorrect → Access Denied
- If correct → Touch Screen Activated

---

### Step 4

The patient can touch predefined areas on the screen to:

- Turn Device 1 ON/OFF
- Turn Device 2 ON/OFF
- Disable Touch Mode
- Activate Emergency Buzzer

---

### Step 5

The administrator can change the password.

The updated password is permanently stored inside EEPROM.

This sequence follows the implementation procedure described for the project, including initialization, password validation, touch-based device control, buzzer activation, and password update. :contentReference[oaicite:2]{index=2}

---

# 📊 Block Diagram

```
              +----------------+
              | 4×4 Keypad     |
              +--------+-------+
                       |
                       |
      +----------------v----------------+
      |          LPC2148 ARM7           |
      |                                |
      |  UART        SPI        GPIO    |
      +----+-----------+-----------+----+
           |           |           |
           |           |           |
           |           |           |
+----------v--+   +----v-----+  +--v------+
| Touch Screen|   | EEPROM   |  | LCD     |
+-------------+   +----------+  +---------+
                                     |
                          +----------+----------+
                          |                     |
                     Device1 (LED)       Device2 (LED)

                                |
                                |
                           Emergency Buzzer
```

---

# 🎯 Applications

- Hospitals
- Home Healthcare
- Smart Patient Rooms
- Assisted Living Systems
- Nursing Homes
- Medical Automation
- Rehabilitation Centers

---

# 🚀 Future Enhancements

- Wi-Fi Based Monitoring
- Android Mobile Application
- IoT Cloud Integration
- Voice Command Support
- GSM Emergency Alerts
- Relay-Controlled Home Appliances
- Fingerprint Authentication
- Battery Backup System

---

# 🧰 Technologies Used

- Embedded C
- ARM7 LPC2148
- UART Communication
- SPI Communication
- EEPROM Memory Interface
- GPIO Programming
- Interrupt Programming

---

# 👨‍💻 Author

**YASH KHARWADE**

**Skills**

- Embedded C
- ARM7 LPC2148
- Device Driver Development
- Linux
- Embedded Systems
