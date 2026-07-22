# GBA_OS - Game Boy Advance Emulator for TCL L5

<div align="center">

![Status](https://img.shields.io/badge/status-Active%20Development-brightgreen.svg)
![Platform](https://img.shields.io/badge/platform-ARM%20Cortex--A53-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

A bare-metal Game Boy Advance emulator operating system for TCL L5 (5033TP) smartphones

[Overview](#-overview) • [Latest Release](#-latest-release-v110) • [Features](#-features) • [Getting Started](#-getting-started) • [Releases](#-releases)

</div>

---

## 📱 Overview

**GBA_OS** is a custom operating system that emulates the Nintendo Game Boy Advance (GBA) on the TCL L5 (5033TP) smartphone. It runs in bare-metal mode without relying on the Android kernel, providing direct hardware access and optimal performance.

This is a GBA emulator operating system for the TCL L5 5033TP. It hasn't been tested yet, but includes a complete power management system for safe device shutdown.

---
## ✨ Features

### Core Emulation
- ✅ ARM CPU Emulation - Full 32-bit ARM instruction set support
- ✅ Thumb Mode - 16-bit Thumb instruction set for optimized code
- ✅ Memory Management - EWRAM, IWRAM, VRAM emulation
- ✅ PPU Graphics - Direct color rendering (240x160 resolution)
- ✅ Audio Synthesis - Timer-driven FIFO audio synthesis

### System Features
- ✅ Power Management - Safe shutdown with 3-second confirmation (v1.1+)
- ✅ Button Input - D-Pad, A, B, L, R button support
- ✅ Storage - eMMC integration for game ROM and save data
- ✅ Timer System - 4 hardware timers with interrupt support
- ✅ Dynamic Configuration - Customizable timeouts and settings

### Hardware Integration
- ✅ MediaTek MT6739 - Direct PMU (Power Management Unit) control
- ✅ LCD Display - Native TCL L5 screen rendering
- ✅ Audio Output - Speaker and headphone jack support
- ✅ Physical Buttons - Mapped to GBA controls

---

## 🚀 Getting Started

### Prerequisites

```bash
# Install ARM cross-compiler toolchain
sudo apt-get install gcc-arm-none-eabi binutils-arm-none-eabi

# Verify installation
arm-none-eabi-gcc --version
```

### Installation

1. **Clone the repository:**
```bash
git clone https://github.com/oticadiotica-sys/GBA_OS.git
cd GBA_OS
```

2. **Compile the project:**
```bash
make clean
make
```

3. **Output:**
```
gba_os.bin          # Final bare-metal binary
gba_os.elf          # Executable with debug symbols
```

---

## 📋 Project Structure

| Module | Purpose | Status |
|--------|---------|--------|
| `cpu_arm.c` | ARM32 instruction execution | ✅ Done |
| `cpu_thumb.c` | Thumb-16 instruction execution | ✅ Done |
| `cpu_memoria.c` | Memory addressing and access | ✅ Done |
| `ppu.c` | Graphics pipeline (Mode 3) | ✅ Done |
| `gba_timers.c` | 4 GBA hardware timers | ✅ Done |
| `gba_sintetizador.c` | Audio FIFO synthesis | ✅ Done |
| `power.c` | Power management & shutdown | ✅ Done (v1.1+) |
| `controles.c` | Button input handling | ✅ Done |
| `armazenamento.c` | eMMC ROM/save management | ✅ Done |

---

## 🔧 Configuration

### Power Management (v1.1+)

Customize shutdown behavior:

```c
#include "power.h"

// Configure 2-second confirmation timeout
PowerConfig_t config = {
    .tempo_confirmacao_ms = 2000,      // Confirmation time
    .timeout_salvamento_ms = 5000,     // Max save time
    .habilitado = 1                    // Enable power management
};

power_configurar(&config);
```

---

## 📊 Releases

| Version | Date | Status | Changes |
|---------|------|--------|---------|
| [v1.1.0](https://github.com/oticadiotica-sys/GBA_OS/releases/tag/1.1.0) | 2026-07-22 | ✅ Stable | Power Management System |
| [v1.0.0](https://github.com/oticadiotica-sys/GBA_OS/releases/tag/1.0.0) | 2026-07-17 | ✅ Stable | Initial Release |

**[View all releases →](https://github.com/oticadiotica-sys/GBA_OS/releases)**

---

## 🎮 Controls

| GBA Button | TCL L5 Button |
|-----------|---------------|
| D-Pad | Volume Up/Down + Back |
| A | Physical A Button |
| B | Physical B Button |
| L | Left shoulder |
| R | Right shoulder |
| START | Menu Button |
| SELECT | Back Button |
| POWER | Power Button (3s hold) |

---

## 📝 Technical Details

- **Target CPU:** ARM Cortex-A53 (MediaTek MT6739)
- **Compiler:** arm-none-eabi-gcc
- **Language:** C + ARM Assembly
- **Memory:** 2GB RAM, 16GB eMMC
- **Display:** 240x160 pixels (GBA native resolution)

---

## 🤝 Contributing

We welcome contributions! Fork the repository and submit a pull request with your improvements.

### Guidelines
- Follow existing code style
- Write clear commit messages
- Test on actual hardware when possible
- Update documentation for new features

---

## 🐛 Known Issues

- Limited to Mode 3 graphics rendering (direct color only)
- Audio synthesis may be CPU-intensive
- Not yet tested on actual TCL L5 device

[Report a bug →](https://github.com/oticadiotica-sys/GBA_OS/issues)

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

## 👥 Authors

- **oticadiotica-sys** - Project Creator & Maintainer

---

## 🔗 Useful Links

- [GBA Technical Reference](https://github.com/gbdev/awesome-gbdev)
- [ARM Cortex-A53 Documentation](https://developer.arm.com/documentation/ddi0500/)
- [Game Boy Advance Hardware Specs](https://www.gbadev.org/)

---

<div align="center">

**Made with ❤️ for Game Boy Advance enthusiasts**

⭐ Star us on GitHub if you like this project!

[📥 Download Latest Release](https://github.com/oticadiotica-sys/GBA_OS/releases/tag/1.1.0)

</div>
