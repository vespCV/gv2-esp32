# Hardware Connection Diagram

**UART Communication (115200 baud):**
- GV2 RXD (D6) → ESP32-S3 TXD (GPIO43/D6) optional
- GV2 TXD (D7) → ESP32-S3 RXD (GPIO44/D7)

**Power:**
- ESP32-S3 5V → Grove Vision AI V2 5V
- ESP32-S3 GND → Grove Vision AI V2 GND

**USB:**
- Computer → Grove Vision AI V2 (USB-C) for programming/power
- Computer → ESP32-S3 (USB-C) for programming/power

**LEDs (for testing):**
- ESP32-S3 GPIO2 (D1) → Green LED (Bees - Class 0)
- ESP32-S3 GPIO3 (D2) → Yellow LED (Hornets - Class 1,2)
- ESP32-S3 GPIO4 (D3) → Red LED (Vespa velutina - Class 3)
- All LED cathodes → Common GND

## Prototype Images

![Prototype Image 1](https://github.com/vespCV/gv2-esp32/blob/main/media/prototype1.jpg)

![Prototype Image 2](https://github.com/vespCV/gv2-esp32/blob/main/media/prototype2.jpg)

## Demo Video

<a href="https://www.youtube.com/shorts/Ib386vmAq0A?autoplay=1">
  <img src="https://img.youtube.com/vi/Ib386vmAq0A/maxresdefault.jpg" alt="Prototype Demo Video" width="50%" style="max-width: 560px;">
</a>

## Notes

**LEDs**: Use current-limiting resistors (typically 220Ω-1kΩ) in series with each LED
**Power**: The Grove Vision AI V2 can be powered via its Grove connector or via its socket
**UART**: Grove Vision AI V2 RXD (D6) ──> ESP32-S3 TXD (GPIO43/D6) is optional, not needed for detection from Grove Vision AI V2 to ESP32-S3

