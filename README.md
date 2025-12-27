# STM32F1 Bare-Metal Register Programming

## Overview
This repository contains **bare-metal embedded firmware examples for STM32F1 microcontrollers**, developed using **direct register-level programming without HAL or high-level libraries**.

The project focuses on understanding **STM32 internal architecture, peripheral registers, and low-level hardware control**, making it suitable for learning and practicing **embedded firmware fundamentals**.

---

## Target MCU
- **STM32F1 series** (ARM Cortex-M3)
- Programming at **register level**
- No HAL / No Arduino / No abstraction layers

---

## Implemented Features
- GPIO configuration and control using registers
- RCC clock configuration
- Basic peripheral initialization
- Low-level delay and timing control
- Bare-metal startup and main loop structure

---

## Development Environment
- **Toolchain:** KeilC / ARM GCC
- **Language:** Embedded C
- **Debugging:** ST-Link
- **Version Control:** Git & GitHub

---

## Project Structure
```text
.
├── Core/            # Startup code and main application
├── Drivers/         # Register definitions and low-level drivers
├── Inc/             # Header files
├── Src/             # Source files
└── README.md
```
## Learning Objectives
- Understand STM32F1 register maps and peripheral control
- Practice low-level embedded C programming
- Gain experience with bare-metal firmware development
- Build a strong foundation for embedded systems and hardware-related roles
  
---

## Notes
- This project is intended for educational purposes and focuses on clarity and understanding rather than library abstraction.

## Author
- **thanhtra3105**
- Computer Engineering – Embedded Systems
- GitHub: https://github.com/thanhtra3105
