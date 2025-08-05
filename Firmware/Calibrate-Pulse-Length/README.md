# Firmware: Calibrate-Pulse-Length

## ℹ️ Info

This firmware will create the pulse length calibration values for a specific Geiger-Müller tube. To get an accurate reading from a tube, the calibration values must be known. These values can then be used in the main Geiger counter firmware [configuration](/Firmware/GMT-Geiger-Counter/Configuration.h) file. The median pulse length for an SBM-20 tube is around 122 µs, but this might differ for different tubes.

## 📋 Instructions

Open the [Calibrate-Pulse-Length.ino](/Firmware/Calibrate-Average-Pulse-Length/Calibrate-Pulse-Length.ino) firmware file inside the Arduino IDE. Plug in the ESP32 S3 via the onboard USB port and verify that the ESP32 boards are installed via the board manager. Under `Tools > Boards > esp32` select the `ESP32S3 Dev Module` then go to `Tools > Port` and select the appropriate port. Flash the firmware by clicking `Upload`. Open up the serial console via `Tools > Serial Monitor` set the baud rate to `115200` and wait until the sample collection is finished. The calibration values will be displayed when the defined number of samples are taken.