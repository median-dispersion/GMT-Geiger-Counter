# Firmware: Calculate-Average-Pulse-Length

## ℹ️ Info

This firmware will calculate the average pulse length for a specific Geiger-Müller Tube. To get an accurate reading from a tube, the average pulse length must be known. This is not the tube's dead time, but the pulse length the tube driver board generates! This value can then be used to calibrate the main Geiger counter firmware to get more accurate readings. The average pulse length for an SBM-20 tube is around 115 µs, but this might differ for different tubes.

## 📋 Instructions

Open the `CalculateAveragePulseLength.ino` firmware file inside the Arduino IDE. Plug in the ESP32 S3 via the onboard USB port and verify that the ESP32 boards are installed via the board manager. Under `Tools > Boards > esp32` select the `ESP32S3 Dev Module` then go to `Tools > Port` and select the appropriate port. Flash the firmware by clicking `Upload`. Open up the serial console via `Tools > Serial Monitor` set the baud rate to `115200` and wait until the sample collection is finished. The average pulse length will be displayed when the defined number of samples were taken.