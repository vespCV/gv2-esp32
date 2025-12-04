# Grove Vision AI V2 with SwiftYOLO via SenseCraft

> **Quick setup guide for the Grove Vision AI V2 using the SwiftYOLO model (192x192px) through the SenseCraft platform. Includes UART1 support for ESP32-S3 integration.**

This guide provides a simpler alternative using the SenseCraft web platform. 

---

## Overview

The SwiftYOLO model is a lightweight model optimized for quick deployment and testing. It can identify the same four classes as the YOLO11n model:

- **Class 0**: Apis mellifera (Honeybee)
- **Class 1**: Vespa crabro (European hornet)
- **Class 2**: Vespula sp. (Yellowjacket)
- **Class 3**: Vespa velutina (Asian hornet) - **Target class**

---

## Prerequisites

Before starting, you need:

1. **Grove Vision AI V2** module connected to your computer via USB
2. A web browser (Chrome recommended)

---

## Step 1: Connect Grove Vision AI V2

1. Connect your **Grove Vision AI V2** to your computer using a USB-C cable
2. Make sure no other programs (serial monitors, etc.) are using the USB port
3. Sign in or create a free **[Seeed account](https://sensecraft.seeed.cc/ai/home)**

---

## Step 2: Open SenseCraft AI Model

1. Navigate to the **[vespa velutina detection model](https://sensecraft.seeed.cc/ai/view-model/61707-vespa-velutina-detection)**


---

## Step 3: Deploy to Grove Vision AI V2

1. Click **Deploy** or **Send to Device**

2. Select your Grove Vision AI V2 from the device list
   - If prompted to connect, click **Connect** and select the USB serial port for your device

3. Wait for the deployment to complete (this may take a few minutes)

---

## Step 4: Test the Detection

Once deployed, you can immediately test the model:

1. On the SenseCraft page, you should see a live camera preview from the Grove Vision AI V2

2. Place test images or actual specimens in front of the camera:
   - **Honeybees**: Should be detected as Class 0
   - **European hornets**: Should be detected as Class 1
   - **Yellowjackets**: Should be detected as Class 2
   - **Asian hornets (Vespa velutina)**: Should be detected as Class 3

3. Detection boxes and class labels should appear in real-time on the preview

---

## Troubleshooting

**Problem: Device not detected**
- Make sure the USB cable supports data transfer (not just charging)
- Try a different USB port
- Close any serial monitor programs that might be using the port

**Problem: No detections appear**
- Ensure adequate lighting for the camera
- Try adjusting the confidence threshold in SenseCraft settings
- Verify the model deployed successfully

---

## UART1 Support

Models uploaded via SenseCraft **include UART1 support built in**. This means:

- The Grove Vision AI V2 can send detection results to the ESP32-S3
- After verifying the model works correctly via the SenseCraft preview, you can connect the Grove Vision AI V2 to the ESP32-S3 according to the [Connection Diagram](connection_diagram.md).

---

## Next Steps

After verifying the Grove Vision AI V2 works correctly:

1. Connect the hardware according to the [Connection Diagram](connection_diagram.md)
2. Flash the ESP32-S3 firmware using PlatformIO
3. Test the full system with LED indicators

**Optional:** For improved detection accuracy, you can use the [YOLO11n setup](../README.md#flash-yolo11n-model-on-grove-vision-ai-v2) (224x224px) instead of SwiftYOLO (192x192px).

