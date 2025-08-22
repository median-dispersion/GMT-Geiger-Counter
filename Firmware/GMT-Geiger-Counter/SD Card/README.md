# SD card preparation

## 📀 Formatting the SD card

In order to use an SD card with the Geiger counter, it must be formatted with the following parameters:

- SD card size: **<= 32 GB**
- Partition table: **MBR**
- File system: **FAT32**
- Cluster size: **32 KB**

## 📋 Copying files to the SD card

Copy the [GMT-Geiger-Counter](/Firmware/GMT-Geiger-Counter/SD%20Card/GMT-Geiger-Counter) directory to the root of the SD card. The files contained in this directory are required for these features:

- Logging data to the SD card
- Accessing the Web-App interface