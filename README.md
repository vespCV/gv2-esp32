# gv2-esp32

* [Flash YOLO Models on Grove Vision AI V2](#flash-yolo-models-on-grove-vision-ai-v2)
    * [Flashing SwiftYOLO](#flashing-swiftyolo)
    * [Flashing YOLO11n Model](#flashing-yolo11n-model)
* [Connection between Grove Vision AI v2 and ESP32-S3](#connection-between-grove-vision-ai-v2-and-esp32-s3)
    * [UART Pin Requirements](#uart-pin-requirements)
    * [XIAO ESP32-S3 to Computer](#xiao-esp32-s3-to-computer)
    * [XIAO ESP32-S3 LED Pins](#xiao-esp32-s3-led-pins)

## Flash YOLO Models on Grove Vision AI V2

This guide details how to flash a **SwiftYOLO** model (192x192px) or a **YOLO11n** model (224x224px) onto the **Grove Vision AI V2** module.

### Flashing SwiftYOLO

Use the simplest method for quick deployment and testing.

* **Deployment:** Use the **SenseCraft** website to deploy your model: [SenseCraft](https://sensecraft.seeed.cc/ai/model).
* **Verification:** The function of the model can be checked immediately on the **SenseCraft** site after flashing.

### Flashing YOLO11n Model [work in progress]

This method requires building custom firmware.

1. **Build Firmware:** Compile your firmware.
2. **Adjust Configuration:** Modify the `APP_TYPE` setting in your build configuration to:
    ```
    APP_TYPE = tflm_yolo11_od
    ```
3. **Model Placement:** Copy your YOLO11n model file to the following directory:
    ```
    model_zoo/tflm_yolo11_od/
    ```
4. **Flash:** Flash the compiled firmware onto the Grove Vision AI V2.
5. **Verification:** The model can be checked using the **Himax AI Toolkit**.
    * Download the toolkit: [Himax AI Toolkit](https://github.com/HimaxWiseEyePlus/Seeed_Grove_Vision_AI_Module_V2/releases/download/v1.1/Himax_AI_web_toolkit.zip).
    * Open `index.html` in your browser and connect the Grove Vision AI V2 module.

## Connection between Grove Vision AI v2 and ESP32-S3

- **UART Pin Requirements**:
  - **RXD** from the gv2 socket to **TXD GPIO43 (D6**) on the ESP32-S3
  - **TXD** from the gv2 socket to **RXD GPIO44 (D7)** on the ESP32-S3

#### XIAO ESP32-S3 to Computer
- **Connection**: USB-C cable
- **Purpose**: Programming, serial communication, and power supply
- **Serial Speed**: 115200 baud (as configured in platformio.ini)

#### XIAO ESP32-S3 
- **LED Pins**:
  - **Red LED**: GPIO4 (D3) - Vespa velutina class 3 detection
  - **Yellow LED**: GPIO3 (D2) - Vespa crabro and vespula sp.
  - **Green LED**: GPIO2 (D1) - Apis mellifera

## Connection between Grove Vision AI v2 and ESP32-S3

This section outlines the hardware connections required for communication and power.

---

### UART Pin Requirements

The Grove Vision AI V2 communicates with the ESP32-S3 via UART.

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


