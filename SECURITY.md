# Security & Safety Guidelines - GBA-OS

## ⚠️ CRITICAL DISCLAIMER

**THIS PROJECT IS EXPERIMENTAL AND UNTESTED ON REAL HARDWARE**

GBA-OS is a bare-metal operating system that operates at the lowest hardware level. **ANY MISTAKE CAN PERMANENTLY DAMAGE YOUR DEVICE.** Proceed only if you understand and accept these risks.

---

## 🚨 Hard Brick Risk

### What is a Hard Brick?

A **Hard Brick** occurs when a device becomes completely unbootable and cannot recover normally. The device will not respond to any input and cannot be revived through standard recovery methods.

### How Can GBA-OS Cause a Hard Brick?

1. **Memory Address Errors**
   - Incorrect memory mapping in `cpu_memoria.c`
   - Accessing protected memory regions
   - Stack overflow or buffer overflow

2. **Bootloader Corruption**
   - Flashing to the wrong partition
   - Incomplete or corrupted binary transfer
   - Power loss during flashing

3. **Bare-Metal Code Issues**
   - Infinite loops without proper interrupt handling
   - Improper CPU frequency scaling
   - Missing initialization sequences

4. **Hardware Incompatibility**
   - Different TCL L5 hardware revisions
   - Variant chip differences (especially MediaTek variants)

### Warning Signs (Before Hard Brick)

- Device doesn't boot after flashing
- Stuck at bootloader screen
- Device gets extremely hot
- Random reboots or freezes
- Unresponsive to any input

---

## 🛡️ Protection Measures

### Before Flashing

**Mandatory Steps:**

1. **✅ Backup Your Stock ROM**
   ```bash
   # Using SP Flash Tool or similar
   # This is your ONLY recovery option in case of Hard Brick
   ```
   - Download from your carrier or manufacturer
   - Verify the file integrity (checksum)
   - Store on external storage

2. **✅ Enable USB Debugging**
   - Required for all communication with the device
   - Settings → Developer Options → USB Debugging

3. **✅ Create a Complete System Backup**
   - Use Android backup tools
   - Back up all personal data
   - Note down important credentials

4. **✅ Test Bootloader Unlock First**
   ```bash
   fastboot oem unlock
   # This will erase all device data but is reversible
   ```

5. **✅ Use Temporary Boot First**
   ```bash
   fastboot boot gba_os.bin
   # This does NOT write to permanent storage
   # Test if the binary works before permanent flashing
   ```

### During Flashing

**Critical Checklist:**

- [ ] Device battery is above 80%
- [ ] Connected via quality USB cable (not through hub)
- [ ] Stable power supply to your PC
- [ ] No antivirus software interfering
- [ ] Device in Fastboot mode (verify with `fastboot devices`)
- [ ] Correct binary file (`gba_os.bin`)
- [ ] No other USB devices connected to PC

### During Boot Testing

- **DO NOT** force power off during first boot
- **DO NOT** disconnect USB cable during boot
- Allow at least 30 seconds for initial boot
- Monitor device temperature
- Watch for any error indicators

---

## 🔧 Recovery from Errors

### If Device Doesn't Boot

**Step 1: Attempt Safe Recovery**
```bash
# Try rebooting to bootloader
adb reboot bootloader

# Or force bootloader mode (device-specific)
# Usually: Hold Power + Volume Down for 10+ seconds
```

**Step 2: Use SP Flash Tool (Last Resort)**

If the above fails:

1. **Download SP Flash Tool**
   - [SP Flash Tool Official](https://spflashtool.com/)

2. **Obtain Stock Firmware**
   - Contact TCL support
   - Download from carrier website
   - Use firmware restoration service

3. **Flash Stock ROM**
   - Enter bootloader/fastboot mode
   - Load stock ROM into SP Flash Tool
   - Initiate flashing process
   - **This will erase ALL data**

4. **Verify Recovery**
   - Device should reboot normally
   - Android should load completely

### If Hard Brick Occurs (Device Won't Boot at All)

**Option 1: Contact Manufacturer**
- Reach out to TCL support
- Provide proof of purchase
- They may offer repair/replacement

**Option 2: Use Professional Recovery Service**
- Search for local phone repair services
- They may have JTAG/eMMC recovery tools
- Cost varies (typically $50-200 USD)

**Option 3: Device Recovery via Specialized Tools**
- Only if you have: JTAG adapter, eMMC reader, or programmer
- Requires advanced technical knowledge
- High risk of permanent damage if done incorrectly

---

## 🔐 Bootloader Security Considerations

### What Unlocking Bootloader Does

- **Allows custom code execution** at the lowest level
- **Disables security checks** for boot images
- **May void warranty** (check with TCL)
- **Erases device data** (security feature)

### Risks of Unlocked Bootloader

1. **Malware Injection**
   - Malicious code can load before OS
   - Full system compromise possible
   - Difficult to detect or remove

2. **Data Theft**
   - Easier to extract user data
   - Encryption can be bypassed

3. **Brick Risk**
   - Wrong boot image causes hard brick
   - No rollback protection

4. **Loss of OTA Updates**
   - Cannot receive automatic security updates
   - Must manually maintain system

### Re-locking Bootloader

```bash
# To restore original bootloader lock:
fastboot oem lock

# WARNING: This will also erase all data
```

---

## 🔍 Verification & Testing

### Before Full Deployment

1. **Temporary Boot Test**
   ```bash
   fastboot boot gba_os.bin
   ```
   - **Safest option** - no permanent changes
   - Test all functionality
   - Monitor for crashes or errors

2. **Check System Logs**
   ```bash
   adb logcat | grep -i error
   ```
   - Watch for kernel panics
   - Monitor memory usage
   - Check for infinite loops

3. **Hardware Health Check**
   - **Temperature**: Should stay below 45°C
   - **Battery Drain**: Normal or expected?
   - **Stability**: No random reboots?

4. **Rollback Test**
   - After temporary boot, reboot normally
   - Verify stock OS still loads
   - Confirm data integrity

### Known Testing Limitations

⚠️ **NOT YET TESTED ON REAL HARDWARE**

- No verified successful boot on actual TCL L5
- Emulator testing only
- Potential unknown hardware issues
- Memory map may be incorrect for some variants

---

## 📋 Pre-Flash Liability Checklist

**You are solely responsible for:**

- [ ] Understanding the risks of bootloader unlocking
- [ ] Accepting the possibility of Hard Brick
- [ ] Having a backup recovery plan
- [ ] Possessing required technical knowledge
- [ ] Having the stock ROM backed up
- [ ] Having SP Flash Tool ready
- [ ] Testing with `fastboot boot` first
- [ ] Not blaming developers for hardware damage
- [ ] Following all steps in INSTALLATION.md precisely
- [ ] Keeping your device powered during flashing

**By proceeding, you acknowledge that:**
- This project is experimental and unsupported
- You understand bare-metal code risks
- You have no warranty protection after bootloader unlock
- You may permanently lose your device
- You assume all legal and financial responsibility

---

## 🚨 Emergency Contacts & Resources

### If Something Goes Wrong

1. **Check This Repository**
   - [GitHub Issues](https://github.com/oticadiotica-sys/GBA_OS/issues)
   - [Discussions](https://github.com/oticadiotica-sys/GBA_OS/discussions)

2. **XDA Developers Forums**
   - [XDA TCL L5 Forum](https://forum.xda-developers.com/)
   - Experienced developers may help

3. **TCL Support**
   - Official support channels
   - May offer hardware repair

4. **Professional Repair**
   - Local phone repair shops
   - Professional recovery services

---

## 📚 Additional Security Reading

- [OWASP Mobile Security Guidelines](https://owasp.org/www-project-mobile-security/)
- [ARM Cortex-A53 Security Features](https://developer.arm.com/documentation/)
- [Android Security & Privacy Documentation](https://source.android.com/security)
- [Linux Kernel Security Documentation](https://www.kernel.org/doc/html/latest/security/)

---

## 📝 Version History

| Version | Date | Status |
|---------|------|--------|
| 1.0 | 2026-09-08 | Initial Security Guidelines |

---

## 👥 Reporting Security Issues

If you discover a security vulnerability in GBA-OS:

1. **DO NOT** create a public GitHub issue
2. **Contact** the maintainer privately
3. **Provide** detailed vulnerability information
4. **Allow time** for a fix before disclosure

---

## ⚖️ Legal Disclaimer

This software is provided "AS IS" without any warranty of any kind, express or implied. The authors and contributors assume no responsibility for:

- Hardware damage or bricking
- Data loss or corruption
- System instability
- Any other damages resulting from use

Use at your own risk.

---

**Last Updated:** September 8, 2026

**For questions or concerns, open an issue in the [GBA-OS Repository](https://github.com/oticadiotica-sys/GBA_OS)**
