# GBA_OS 1.1 - Release Notes

**Date:** 2026-07-22  
**Version:** 1.1.0  
**Status:** ✅ Stable

## 🎯 Summary

GBA OS 1.1 adds a **complete power management system** with support for safe shutdown of the TCL L5 (5033TP) device.

---

## ✨ New Features

### 🔌 Power Shutdown System (Power Management)

- ✅ **POWER Button Detection:** Long press detection (3 seconds)
- ✅ **State Machine:** 5 operational states:
  - `POWER_STATE_ACTIVE` - Normal operating system
  - `POWER_STATE_PRESSING` - Button being pressed
  - `POWER_STATE_CONFIRMATION` - Waiting for confirmation (3s)
  - `POWER_STATE_SAVING` - Saving state before shutdown
  - `POWER_STATE_SHUTTING_DOWN` - Shutting down hardware

- ✅ **State Backup:** Backup of registers and critical data
- ✅ **MediaTek Integration:** Integration with PMU (Power Management Unit) of MT6739
- ✅ **Dynamic Configuration:** Customizable timeouts

---

## 📁 New Files

| File | Description |
|------|-------------|
| `power.h` | Power module header (2.6 KB) |
| `power.c` | Power manager implementation (5.8 KB) |

---

## 🔧 Changes

### main.c
- ✅ Power module initialization via `power_init()`
- ✅ State update in main loop via `power_atualizar()`
- ✅ Include of `power.h` header

### Makefile
- ✅ Added `power.c` to compilation list
- ✅ Added `armazenamento.c` to compilation list

---

## 🐛 Bug Fixes

1. **Compilation:** Added `#include <stddef.h>` to define `NULL`
2. **Warnings:** Removed unused `config_padrao` variable
3. **Integration:** Removed unnecessary dependency on `armazenamento.h` in power.c

---

## 📋 Public API

### Available Functions

```c
// Initialize the module
void power_init(void);

// Update state each frame
void power_atualizar(void);

// Force immediate shutdown
void power_solicitar_desligamento(void);

// Get current state
PowerState_t power_obter_estado(void);

// Configure timeouts
void power_configurar(PowerConfig_t* config);

// Get current configuration
PowerConfig_t power_obter_config(void);
```

### Usage Example

```c
// Configure 2 second timeout instead of 3
PowerConfig_t cfg = {
    .tempo_confirmacao_ms = 2000,
    .timeout_salvamento_ms = 5000,
    .habilitado = 1
};
power_configurar(&cfg);
```

---

## 🔗 Included Commits

1. `b459b320` - feat: Add power.h module
2. `4c18e0e5` - feat: Implement power.c module
3. `4071f174` - feat: Integrate power module in main.c
4. `e01260e9` - fix: Add power.c and armazenamento.c to Makefile
5. `02911ed1` - fix: Remove unnecessary dependency
6. `3b01a9ec` - fix: Add stddef.h for NULL

---

## 🧪 Recommended Testing

1. Compile with `make clean && make`
2. Run generated binary: `gba_os.bin`
3. Press POWER button for 3+ seconds
4. Verify safe shutdown sequence

---

## 📝 Technical Notes

- Based on commit: `3b01a9ec467b1ff6c3e97e17c0be2b8185a79815`
- Branch: `main`
- Compiler: `arm-none-eabi-gcc`
- Target CPU: ARM Cortex-A53 (MediaTek MT6739)

---

## 🚀 Planned Future Improvements

- [ ] Graphical confirmation interface before shutdown
- [ ] Suspend/resume support
- [ ] Battery monitoring
- [ ] Operation logging to file

---

It is a GBA emulator operating system for the TCL L5 5033TP with enhanced power management capabilities.
