# Vespa velutina detection with Grove Vision AI V2 and esp32-S3

> **Automated detection system for Vespa velutina (Asian hornet) using computer vision. Combines Grove Vision AI V2 for real-time object detection with ESP32-S3 for processing.**

## Goal

This project implements an automated detection system for **Vespa velutina** (Asian hornet) using computer vision. The system uses a **Grove Vision AI V2** module for real-time object detection and an **ESP32-S3** microcontroller to process detection results and control indicator LEDs.

The detection model can identify four classes:
- **Class 0**: Apis mellifera (Honeybee)
- **Class 1**: Vespa crabro (European hornet)
- **Class 2**: Vespula sp. (Yellowjacket)
- **Class 3**: Vespa velutina (Asian hornet) - **Target class**

## Setup Overview

The system consists of two main components:

1. **Grove Vision AI V2**: Performs object detection using a SwiftYOLO (for test purposes) or YOLO11n (improved detection) model and sends detection results via UART1 to the ESP32-S3
2. **XIAO ESP32-S3**: Receives detection data, processes it, and controls LEDs to indicate detected species

**Key Features:**
- Real-time object detection with YOLO model
- UART1 communication between Grove Vision AI V2 and ESP32-S3
- Visual feedback via colored LEDs (Red for Vespa velutina, Yellow for other hornets, Green for honeybees)
- Custom firmware with UART1 support for Grove Vision AI V2

---

* [Flash YOLO Models on Grove Vision AI V2](#flash-yolo-models-on-grove-vision-ai-v2)
    * [Flashing SwiftYOLO](#flashing-swiftyolo)
    * [Flashing YOLO11n Model](#flashing-yolo11n-model)
* [Connection between Grove Vision AI v2 and ESP32-S3](#connection-between-grove-vision-ai-v2-and-esp32-s3)
    * [UART Pin Requirements](#uart-pin-requirements)
    * [XIAO ESP32-S3 to Computer](#xiao-esp32-s3-to-computer)
    * [XIAO ESP32-S3 LED Pins](#xiao-esp32-s3-led-pins)

## Flash YOLO Models on Grove Vision AI V2

This part details how to flash a **SwiftYOLO** model (192x192px) or a **YOLO11n** model (224x224px) onto the **Grove Vision AI V2** module.

### Flashing SwiftYOLO

Use the [Vespa velutina detection](https://sensecraft.seeed.cc/ai/model/detail?id=61707&time=1763798762425) SwiftYOLO model for quick deployment and testing of the setup. It has UART1 support built in. _Review in progress, will be published to the model list after review and approval_

* **Deployment:** Use the upload the [Vespa velutina detection](https://github.com/vespCV/gv2-esp32/blob/main/models/vespcv_swiftyolo_int8_vela.tflite) to [SenseCraft](https://sensecraft.seeed.cc/ai/model) as long as review is in progress. 
* **Verification:** The function of the model can be checked immediately on the **SenseCraft** site after flashing.

### Flashing YOLO11n Model with UART1 Support

This guide explains how to flash the custom firmware that enables UART1 communication for ESP32-S3 integration.

#### Prerequisites

Before starting, you need:

1. **Python 3** installed on your computer
2. **Grove Vision AI V2** module connected to your computer via USB
3. **This repository** (which includes the model file in the `models` folder)

#### Step 1: Install the Original Repository

1. Open a terminal (Command Prompt on Windows, Terminal on Mac/Linux)
2. Navigate to a folder where you want to install the repository (for example, your Desktop or Documents folder)
3. Run the following command to download the repository:

   ```bash
   git clone --recursive https://github.com/HimaxWiseEyePlus/Seeed_Grove_Vision_AI_Module_V2.git
   ```

4. Wait for the download to complete. This may take a few minutes.

#### Step 2: Install Python Dependencies

1. Navigate into the repository folder:

   ```bash
   cd Seeed_Grove_Vision_AI_Module_V2
   ```

2. Install the required Python packages:

   ```bash
   pip3 install -r xmodem/requirements.txt
   ```

   **Note:** If you get a "command not found" error, try using `pip` instead of `pip3`, or `python -m pip` instead.

#### Step 3: Copy the Custom Firmware Image

This repository includes a pre-built firmware image (`output.img`) with UART1 support. You need to copy it to the Himax repository location.

**Steps:**

1. From this repository (`gv2-esp32`), locate the pre-built firmware image:

   ```
   images/output.img
   ```

2. Copy this file to the Himax repository's output directory:

   ```
   Seeed_Grove_Vision_AI_Module_V2/we2_image_gen_local/output_case1_sec_wlcsp/output.img
   ```

   **Example command** (adjust paths as needed):

   ```bash
   cp images/output.img Seeed_Grove_Vision_AI_Module_V2/we2_image_gen_local/output_case1_sec_wlcsp/output.img
   ```

**Note:** This firmware image includes custom modifications to enable UART1 communication. See the [Change Log](documentation/changes.md) for details on the modifications.

#### Step 4: Copy the Model File

The model file is already provided in this repository. You need to copy it to the Himax repository.

1. From this repository (`gv2-esp32`), copy the model file from the `models` folder:

   ```
   models/vespcv_swiftyolo_int8_vela.tflite
   ```

2. Paste it into the Himax repository's model directory:

   ```
   Seeed_Grove_Vision_AI_Module_V2/model_zoo/tflm_yolo11_od/vespcv_swiftyolo_int8_vela.tflite
   ```

   **Note:** Make sure the file is named exactly `vespcv_swiftyolo_int8_vela.tflite` in the destination folder.

#### Step 5: Find Your USB Port Name

Before flashing, you need to find the name of the USB port where your Grove Vision AI V2 is connected.

**On Mac:**
1. Open Terminal
2. Run: `ls /dev/cu.usbmodem*`
3. You should see something like `/dev/cu.usbmodem58FA1047631` - copy this exact name

**On Linux:**
1. Open Terminal
2. Run: `ls /dev/ttyACM*` or `ls /dev/ttyUSB*`
3. You should see something like `/dev/ttyACM0` - copy this exact name

**On Windows:**
1. Open Device Manager (search for "Device Manager" in the Start menu)
2. Look under "Ports (COM & LPT)"
3. Find "USB Serial Port" or similar - it will show something like "COM3" or "COM4"
4. Note the COM number (e.g., `COM3`)

#### Step 6: Flash the Firmware

1. Make sure your Grove Vision AI V2 is connected to your computer via USB
2. Close any serial monitor or terminal programs that might be using the USB port
3. Open a terminal and navigate to the repository folder:

   ```bash
   cd Seeed_Grove_Vision_AI_Module_V2
   ```

4. Run the flashing command. **Replace only the port name** with your actual USB port name:

   **On Mac/Linux:**
   ```bash
   python3 xmodem/xmodem_send.py \
     --port=/dev/cu.usbmodem###### \
     --baudrate=921600 \
     --protocol=xmodem \
     --file=we2_image_gen_local/output_case1_sec_wlcsp/output.img \
     --model="model_zoo/tflm_yolo11_od/vespcv_swiftyolo_int8_vela.tflite 0xB7B000 0x00000"
   ```

   **On Windows:**
   ```bash
   python xmodem\xmodem_send.py ^
     --port=COM3 ^
     --baudrate=921600 ^
     --protocol=xmodem ^
     --file=we2_image_gen_local\output_case1_sec_wlcsp\output.img ^
     --model="model_zoo\tflm_yolo11_od\vespcv_swiftyolo_int8_vela.tflite 0xB7B000 0x00000"
   ```

   **Important adjustments:**
   - Replace `/dev/cu.usbmodem#######` (Mac) or `COM3` (Windows) with your actual USB port name from Step 5
   - The model filename (`vespcv_swiftyolo_int8_vela.tflite`) is already correct - do not change it
   - On Windows, use backslashes (`\`) instead of forward slashes (`/`) in paths

5. The script will start uploading. You should see a progress bar.

6. **When you see the message "Send data using the xmodem protocol from your terminal":**
   - **Press the RESET button** on your Grove Vision AI V2 module (black button)
   - The upload will continue automatically

7. Wait for the upload to complete. You should see "xmodem_send bin file done!!" when finished.

#### Step 7: Verification

After flashing is complete:

1. The Grove Vision AI V2 will restart after you press the reset button (y does not work)
2. You can verify the firmware is working by:
    - Using the [Himax AI web toolkit](https://github.com/HimaxWiseEyePlus/Seeed_Grove_Vision_AI_Module_V2/releases/download/v1.1/Himax_AI_web_toolkit.zip) select Grove Vsion AI(V2), press `connect` select the port and put some test image in front of the camera to check the detection.
   - Connecting to the module via serial monitor at 921600 baud
   - You should see a startup message: "*** CUSTOM FIRMWARE WITH UART1 SUPPORT ***"
   - The module should start detecting objects and sending results via UART1

#### Troubleshooting

**Problem: "ModuleNotFoundError: No module named 'serial'"**
- Solution: Make sure you installed the requirements: `pip3 install -r xmodem/requirements.txt`

**Problem: "Uart port open fail"**
- Solution: Check that your USB port name is correct and no other program is using it

**Problem: Upload doesn't start**
- Solution: Make sure to press the RESET button on the Grove Vision AI V2 when prompted

**Problem: "File not found"**
- Solution: 
  - Check that the `output.img` file is in the correct location: `we2_image_gen_local/output_case1_sec_wlcsp/output.img`
  - Check that the model file `vespcv_swiftyolo_int8_vela.tflite` is in: `model_zoo/tflm_yolo11_od/`
  - Make sure you copied the model file from the `models` folder in this repository to the Himax repository

## Connection between Grove Vision AI v2 and ESP32-S3

For connection diagrams and prototype images, see the [Connection Diagram documentation](https://github.com/vespCV/gv2-esp32/blob/main/documentation/connection_diagram.md).

This section outlines the hardware connections required for communication and power. LEDs are used for test purposes.

---

### UART Pin Requirements

The Grove Vision AI V2 communicates with the ESP32-S3 via UART and requires separate power connections. 

* **RXD (gv2) $\rightarrow$ TXD (ESP32-S3):** The **RXD** pin from the Grove Vision AI V2 socket connects to **GPIO43 (D6)** on the ESP32-S3.
* **TXD (gv2) $\rightarrow$ RXD (ESP32-S3):** The **TXD** pin from the Grove Vision AI V2 socket connects to **GPIO44 (D7)** on the ESP32-S3.
* **Power (5V):** The **5V** line from the ESP32-S3 connects to the Grove Vision AI V2 socket (or via the Grove cable).
* **GND:** The **GND** line from the ESP32-S3 connects to the Grove Vision AI V2 socket (or via the Grove cable).

### XIAO ESP32-S3 to Computer

* **Connection:** Use a **USB-C cable**.
* **Purpose:** Provides **programming**, **serial communication**, and **power supply**.
* **Serial Speed:** The communication speed is set to **115200 baud** (as configured in `platformio.ini`).

### XIAO ESP32-S3 LED Pins

These pins are configured to indicate specific detection events.

* **Red LED (GPIO4 / D3):** Indicates **Vespa velutina** (Asian hornet) detection (Class 3).
* **Yellow LED (GPIO3 / D2):** Indicates **Vespa crabro** (European hornet) and **Vespula sp.** (Yellowjacket) detection.
* **Green LED (GPIO2 / D1):** Indicates **Apis mellifera** (Honeybee) detection.


