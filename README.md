# My-projects
 # Touch-Based Device Control System for Bedridden Patients

- Designed and developed a password-protected touch-based embedded system using LPC2148 to help bedridden patients independently control essential electrical devices.
- Implemented a resistive touchscreen interface with predefined touch positions for controlling Device 1 and Device 2 (ON/OFF).
- Integrated 4×4 keypad, 16×2 LCD, AT25LC512 EEPROM, UART interrupts, and buzzer for secure authentication, user interaction, and emergency alerts.
- Implemented password validation and password modification, with updated credentials securely stored in EEPROM.
- Developed the application using Embedded C, with Keil C and Flash Magic for programming and hardware implementation.


 # Objectives
- Help bedridden patients operate household appliances independently.
- Provide a simple and user-friendly touch-based interface.
- Ensure secure access through password authentication.
- Store passwords permanently using non-volatile memory.
- Allow password updates without reprogramming the controller.

# ⭐ Key Features
- 🔐 Password Authentication — Secure access using keypad-based password verification.
- 👆 Touch-Based Control — Resistive touchscreen enables easy device control with minimal physical effort.
- 💡 Dual Device Control — Controls two devices (ON/OFF) through predefined touchscreen positions.
- 💾 EEPROM Password Storage — Stores and retrieves the password using AT25LC512 EEPROM.
- 🚨 Emergency Alert — Integrated buzzer provides an emergency alert mechanism.
- 🔄 Password Modification — Allows the user/admin to change the password and save the updated password in EEPROM.
- 📺 LCD Interface — Displays characters, strings, integers, and keypad-related information.

# 🔧 Hardware Components Used
- 🧠 LPC2148 — Main ARM7 microcontroller
- 👆 Resistive Touch Screen Module — Touch-based user interface for device control
- 📺 16×2 LCD — Displays system information and user inputs
- 🔢 4×4 Matrix Keypad — Used for password entry and modification
- 💡 Device 1 (LED1) — Represents/controls the first device
- 💡 Device 2 (LED2) — Represents/controls the second device
- 🔊 Buzzer — Provides an emergency alert
- 💾 AT25LC512 EEPROM — Stores and retrieves the system password

