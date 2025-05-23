#!/usr/bin/env python3

import argparse
import sys
import serial
import time
import json
from datetime import datetime
from pathlib import Path

# =================================================================================================
# Get launch arguments
# =================================================================================================
def getLaunchArguments():

    # Launch argument parser
    parser = argparse.ArgumentParser(description="A python script for logging data sent by a GMT Geiger Counter via the serial interface. (https://github.com/median-dispersion/GMT-Geiger-Counter)")
    
    # Add arguments
    parser.add_argument("--port",   type=str, required=True,                    help="Serial port.")
    parser.add_argument("--baud",   type=int, required=False, default=115200,   help="Serial baud rate. The default is 115200 baud.")
    parser.add_argument("--output", type=str, required=False, default="./Logs", help="Path to output file or directory. The default output directory is './Logs'.")
    
    # Parse arguments
    return parser.parse_args()

# =================================================================================================
# Print a log message
# =================================================================================================
def log(level = "DEBUG", message = "Invalid log message!"):

    # Get the current date and time in ISO form
    date  = datetime.now().astimezone().isoformat()
    
    # Depending on the log level color in the level text
    match level:

        case "DEBUG":   level = f"\033[92m[{level}]\033[0m"
        case "INFO":    level = f"\033[96m[{level}]\033[0m"
        case "WARNING": level = f"\033[93m[{level}]\033[0m"
        case "ERROR":   level = f"\033[91m[{level}]\033[0m"
        case _:         level = f"\033[95m[UNKNOWN]\033[0m"

    # Print log message
    print(f"{date} {level} >> {message}")

# =================================================================================================
# Terminate script execution
# =================================================================================================
def terminate():

    # Print log message
    log("INFO", f"Exiting!")
    
    # Exit
    sys.exit()

# =================================================================================================
# Get the output log file path
# =================================================================================================
def getOutputPath(output):

    # Try getting the output path
    try:

        # Path
        path = Path(output)

        # If there is no output file specified add the output file to the full path
        if path.suffix == "": path = path / f"Log_{datetime.now().astimezone().isoformat()}.json"

        # Create output directory if it doesn't already exist
        path.parent.mkdir(parents=True, exist_ok=True)

        # Check write permissions or create output file if it doesn't exists by appending nothing to it
        open(path, "a").close()

        # Return the output path
        return path

    # If an exception occurs
    except Exception as exception:

        # Print log message
        log("ERROR", f"Failed to initialize the output file! ({exception})")
        
        # Exit
        terminate()

# =================================================================================================
# Connect to serial device
# =================================================================================================
def getSerialDevice(port, baud):

    # Try connecting to the serial device
    try:

        # Connect to serial device in blocking mode
        device = serial.Serial(port, baud, timeout=None)

        # Print log message
        log("INFO", f"Connected to serial device '{port}' at {baud} baud!")

        # Return serial device
        return device

    # If an exception occurs
    except Exception as exception:

        # Print log message
        log("ERROR", f"Failed to connect to serial device '{port}' at {baud} baud! ({exception})")

        # Return no device
        return None

# =================================================================================================
# Read serial data from device
# =================================================================================================
def getSerialData(device):

    # Try reading serial data
    try:

        # Read serial data
        data = device.readline().decode("utf-8").strip()

        # Print log message
        log("INFO", f"New serial data: '{data}'")

        # Return data
        return data

    # If an exception occurs
    except Exception as exception:

        # Print log message
        log("ERROR", f"Failed to communicate with serial device '{device.port}' at {device.baudrate} baud! ({exception})")

        # Return no data
        return None

# =================================================================================================
# Parse serial data as JSON
# =================================================================================================
def getJSONData(data):

    # Try parsing serial data as JSON
    try:

        # Parse data as JSON
        data = json.loads(data)

        # If data doesn't contain a date add current date in ISO form
        if "date" not in data: data["date"] = datetime.now().astimezone().isoformat()

        # Print log message
        log("INFO", f"Parsed JSON data: '{data}'")

        # Return JSON data
        return data

    # If an exception occurs
    except Exception as exception:

        # Print log message
        log("WARNING", f"[WARNING] >> Failed to parse '{data}' as JSON! ({exception})")

        # Return no data
        return None

# =================================================================================================
# Write JSON data to log file
# =================================================================================================
def writeToLogFile(path, data):

    # Try writing to the log file
    try:

        # Open output file
        with open(path, "a") as file:

            # Write JSON to output file as compact as possible
            file.write(json.dumps(data, separators=(",", ":")))

    except Exception as exception:

        # Print log message
        log("ERROR", f"Unable to write to the output file! ({exception})")

        # Exit
        terminate()

# =================================================================================================
# Main
# =================================================================================================
def main():

    # Get launch arguments
    arguments = getLaunchArguments()

    # Get log file output path
    path = getOutputPath(arguments.output)

    # Get serial device
    device = getSerialDevice(arguments.port, arguments.baud)

    # If successfully connected to a serial device
    if device != None:

        # Handle keyboard interrupts
        try:

            # Main loop
            while True:

                # Get serial data
                data = getSerialData(device)

                # If reading serial data was successful
                if data != None: 
                    
                    # Parse serial data as JSON
                    data = getJSONData(data)

                    # If parsing the serial data as JSON was successful
                    if data != None:

                        # Write JSON data to log file
                        writeToLogFile(path, data)

                # If reading serial data failed
                else:

                    # Close connection to serial device
                    device.close()

                    # Reset serial device
                    device = None

                    # Loop until a new device connection is established
                    while device == None:

                        # Print log message
                        log("INFO", f"Reconnecting serial device in 5 seconds...")

                        # Wait for 5 seconds
                        time.sleep(5)

                        # Reconnect serial device
                        device = getSerialDevice(arguments.port, arguments.baud)

        # On keyboard interrupt terminate script
        except KeyboardInterrupt: terminate()

# Start the main function
if __name__ == "__main__": main()