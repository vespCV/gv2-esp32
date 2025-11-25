# Change Log: Himax Grove Vision AI V2 Firmware Modifications

This document lists all changes made to the original Himax firmware to create the modified version for ESP32-S3 communication.

## Overview

The modifications enable UART1 communication on the Grove Vision AI V2 module to send object detection results to an ESP32-S3 microcontroller. The changes primarily focus on:

1. **UART1 Configuration**: Enabling and configuring UART1 on pins PB6 (RX) and PB7 (TX) for Grove connector D6/D7
2. **Detection Result Transmission**: Sending YOLO11 object detection results via UART1 in JSON format
3. **Build Configuration**: Adding necessary compiler defines and build flags

---

## Modified Files

### 1. `EPII_CM55M_APP_S/app/main.c`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/app/main.c`

**Changes**:
- Added debug output in the `TFLM_YOLO11_OD` section
- Added `#include "xprintf.h"` for debug printing
- Added startup banner that displays build date and time:
  ```c
  xprintf("\r\n\r\n========================================\r\n");
  xprintf("*** CUSTOM FIRMWARE WITH UART1 SUPPORT ***\r\n");
  xprintf("*** BUILD DATE: %s %s ***\r\n", __DATE__, __TIME__);
  xprintf("========================================\r\n\r\n");
  ```

**Purpose**: Provides visual confirmation that the custom firmware with UART1 support is running.

---

### 2. `EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/send_result.h`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/send_result.h`

**Changes**:
- Added three new function declarations:
  ```c
  el_err_code_t send_bytes_uart1(const char* buffer, size_t size);
  el_err_code_t send_detections_uart1_simple(const std::forward_list<el_box_t>& boxes);
  void event_reply_uart1(std::string data);
  ```

**Purpose**: Exposes UART1 communication functions for sending detection results.

---

### 3. `EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/send_result.cpp`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/send_result.cpp`

**Changes**:
- Added includes: `<cstdio>` and additional `<cstring>` for string length functions
- Added static global variables for UART1 management:
  ```cpp
  static DEV_UART* g_grove_uart = nullptr;
  static bool g_uart1_initialized = false;
  ```
- Implemented `send_bytes_uart1()` function:
  - Initializes UART1 device on first call (115200 baud)
  - Keeps UART1 open for subsequent transmissions
  - Sends data in 8-byte chunks with delays between chunks
  - Includes debug output to UART0 for troubleshooting
- Implemented `send_detections_uart1_simple()` function:
  - Formats detection boxes in simple text format
  - Sends formatted message via UART1
- Implemented `event_reply_uart1()` function:
  - Formats detection results in JSON format compatible with SSCMA library
  - Wraps data in JSON structure: `{"type": 1, "name": "INVOKE", "code": 0, "data": {...}}`
  - Sends via `send_bytes_uart1()`

**Purpose**: Provides UART1 communication functionality for sending detection results to ESP32-S3.

---

### 4. `EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/cvapp_yolo11n_ob.cpp`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/cvapp_yolo11n_ob.cpp`

**Changes**:
- Added include: `#include "hx_drv_scu.h"` (conditionally for `IP_scu`)
- Added diagnostic output in `cv_yolo11n_ob_init()`:
  - Prints custom firmware banner with build date
  - Includes optional UART1 test message (when `UART_SEND_ALOGO_RESEULT` is defined)
  - Verifies pinmux configuration for PB6/PB7
- Added debug output in detection loop:
  - Prints `g_trans_type` value for debugging
  - Adds debug markers before and after UART1 code section
- Added UART1 transmission in detection callback:
  - Calls `event_reply_uart1()` after standard `event_reply()` to UART0
  - Sends detection results in JSON format via UART1
  - Always sends to UART1 regardless of transfer type setting

**Purpose**: Integrates UART1 transmission into the YOLO11 object detection pipeline.

---

### 5. `EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/tflm_yolo11_od.c`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/tflm_yolo11_od.c`

**Changes**:
- Added UART1 pinmux configuration in `tflm_yolo11_od_pinmux_cfg()`:
  ```c
  #ifdef UART_SEND_ALOGO_RESEULT
  /* UART1 for Grove connector D6/D7 (PB6=RX, PB7=TX) */
  pinmux_cfg.pin_pb6 = SCU_PB6_PINMUX_UART1_RX;     /*!< pin PB6 - D6 (RX)*/
  pinmux_cfg.pin_pb7 = SCU_PB7_PINMUX_UART1_TX;     /*!< pin PB7 - D7 (TX)*/
  #endif
  ```

**Purpose**: Configures GPIO pins PB6 and PB7 for UART1 functionality.

---

### 6. `EPII_CM55M_APP_S/board/epii_evb/pinmux_init.c`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/board/epii_evb/pinmux_init.c`

**Changes**:
- Added UART1 pinmux configuration:
  ```c
  /* UART1 for Grove connector D6/D7 */
  pinmux_cfg.pin_pb6 = SCU_PB6_PINMUX_UART1_RX;     /*!< pin PB6 - D6 (RX)*/
  pinmux_cfg.pin_pb7 = SCU_PB7_PINMUX_UART1_TX;     /*!< pin PB7 - D7 (TX)*/
  ```

**Purpose**: Ensures UART1 pins are configured at board initialization level.

---

### 7. `EPII_CM55M_APP_S/board/epii_evb/platform_driver_init.c`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/board/epii_evb/platform_driver_init.c`

**Changes**:
- Modified UART1 initialization condition:
  ```c
  #if defined(IP_INST_UART1) || defined(IP_INST_NS_UART1) || defined(UART_SEND_ALOGO_RESEULT)
  hx_drv_uart_init(USE_DW_UART_1, HX_UART1_BASE);
  #endif
  ```
  - Added `|| defined(UART_SEND_ALOGO_RESEULT)` to ensure UART1 is initialized when the feature is enabled

**Purpose**: Ensures UART1 driver is initialized when UART1 communication is enabled.

---

### 8. `EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/tflm_yolo11_od.mk`

**Location**: `Himax_gv2_esp32/Seeed_Grove_Vision_AI_Module_V2/EPII_CM55M_APP_S/app/scenario_app/tflm_yolo11_od/tflm_yolo11_od.mk`

**Changes**:
- Added compiler defines:
  ```makefile
  APPL_DEFINES += -DIP_INST_UART1
  APPL_DEFINES += -DIP_INST_NS_UART1
  ```
  - These defines enable UART1 instance in both secure and non-secure domains

**Purpose**: Enables UART1 hardware instance at compile time.

---

## Summary of Functional Changes

1. **UART1 Hardware Configuration**:
   - Pins PB6 (RX) and PB7 (TX) configured for UART1
   - UART1 initialized at 115200 baud
   - UART1 driver enabled in both secure and non-secure domains

2. **Detection Result Transmission**:
   - Detection results sent via UART1 in JSON format
   - Compatible with SSCMA library format expected by ESP32-S3
   - Transmission occurs after each detection cycle

3. **Debug and Diagnostics**:
   - Build date/time displayed on startup
   - Debug messages sent to UART0 for troubleshooting
   - Pinmux configuration verification

4. **Build Configuration**:
   - Compiler defines added to enable UART1 hardware instance
   - Feature flag `UART_SEND_ALOGO_RESEULT` controls UART1 functionality

---

## Technical Details

### UART Configuration
- **Baud Rate**: 115200
- **Pins**: 
  - PB6 (D6) - UART1 RX
  - PB7 (D7) - UART1 TX
- **Format**: 8N1 (8 data bits, no parity, 1 stop bit)

### Data Format
Detection results are sent in JSON format:
```json
{
  "type": 1,
  "name": "INVOKE",
  "code": 0,
  "data": {
    "count": 0,
    "algo_tick": <processing_time>,
    "boxes": [<detection_boxes>]
  }
}
```

### Compatibility
- ESP32-S3 can parse the JSON format directly
- Maintains backward compatibility with original UART0 output (inference can be tested with Himx AI toolkit)

