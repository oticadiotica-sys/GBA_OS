# Installation Guide - GBA-OS

## ⚠️ Critical Safety Warning

**THIS PROJECT HAS NOT YET BEEN TESTED ON AN ACTUAL TCL L5 DEVICE**

Since this code operates at the bare-metal hardware level, any memory address error in files like `tcl_l5_stub.c` or `cpu_memoria.c` can cause an **instant Hard Brick** when attempting to boot, rendering the device permanently unusable.

### Essential Requirements Before Starting:
- ✅ **Have the original factory firmware (Stock ROM) for TCL L5 downloaded on your PC**
- ✅ **Have SP Flash Tool installed and configured**
- ✅ **Complete backup of your data**
- ✅ **Absolute certainty about what you're doing**

**Proceed at your own risk!**

---

## Step 1: Unlock the TCL L5 Bootloader

Before flashing any custom system, the chip security must be released.

### Steps:

1. **Enable USB Debugging** in the device's Developer Options:
   - Go to **Settings → About Phone**
   - Tap **Build Number** 7 times
   - Return to **Settings → Developer Options**
   - Enable **USB Debugging**

2. **Connect the device to your PC** via USB cable

3. **Reboot into Fastboot mode** using the command prompt:
   ```bash
   adb reboot bootloader
   ```

4. **Unlock the bootloader**:
   - For standard devices:
     ```bash
     fastboot oem unlock
     ```
   - For MTK chipsets (verify if TCL L5 uses MTK):
     - Use **MTK Client** or MTK-specific tools
     - Follow the tool manufacturer's instructions

⚠️ **Note**: Bootloader unlocking may erase all device data.

---

## Step 2: Flash the Binary (gba_os.bin)

Since this project is designed to replace the device's recovery or boot partition, flashing must be done via command line on your computer using the **Fastboot** tool.

### Prerequisites:
- Compiled `gba_os.bin` file available
- Fastboot installed and in your system PATH
- Device in Fastboot mode

### Steps:

1. **Put the device in Fastboot mode**:
   - The screen will show a static logo or "fastboot" text

2. **On your PC command prompt**, navigate to the folder containing the compiled file:
   ```bash
   cd /path/to/gba_os.bin
   ```

3. **Test without replacing your original system** (HIGHLY RECOMMENDED):
   ```bash
   fastboot boot gba_os.bin
   ```
   - This will perform a temporary boot without permanent memory writing
   - If it works, you can proceed with confidence

4. **To flash permanently to the recovery partition**:
   ```bash
   fastboot flash recovery gba_os.bin
   ```

5. **Reboot the device**:
   ```bash
   fastboot reboot
   ```

---

## Recovery Procedure (In Case of Error)

If the device fails to boot or enters Hard Brick state:

1. **Download SP Flash Tool** (if you haven't already)
2. **Obtain the original stock firmware** for TCL L5
3. **Use SP Flash Tool** to restore the original firmware:
   - Put the device in recovery mode (usually by holding specific buttons)
   - Open SP Flash Tool
   - Load the stock firmware
   - Start the flashing process
4. The device will be restored to factory state

---

## Final Checklist Before Flashing

- [ ] I have made a complete backup of my data
- [ ] I have the TCL L5 stock firmware downloaded
- [ ] I have SP Flash Tool installed
- [ ] I tested with `fastboot boot gba_os.bin` first
- [ ] I understand the risks of a Hard Brick
- [ ] I am certain I want to proceed

---

## Troubleshooting

### "adb: command not found"
- Install Android SDK Tools or add the adb path to your system PATH

### "fastboot: command not found"
- Install Android SDK Tools or add the fastboot path to your system PATH

### Device doesn't appear in `adb devices`
- Verify USB Debugging is enabled
- Try a different USB cable
- Update the device drivers on your PC

### Bootloader unlock fails
- Make sure the device is in Fastboot mode
- Verify if the bootloader can be unlocked (some models have restrictions)
- Check TCL L5 specific documentation

---

## Useful References

- [Android SDK Platform Tools](https://developer.android.com/studio/releases/platform-tools)
- [MTK Client (for MTK chipsets)](https://github.com/bkerler/mtkclient)
- [SP Flash Tool](https://spflashtool.com/)

---

**Good luck and remember: proceed with caution!** 🔧
