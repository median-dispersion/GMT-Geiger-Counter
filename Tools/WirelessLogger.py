#!/usr/bin/env python3

import argparse
import sys
import requests
import json
import time
from datetime import datetime
from pathlib import Path

# =================================================================================================
# Get the scripts launch arguments
# =================================================================================================
def getLaunchArguments(): 

    # Launch argument parser
    parser = argparse.ArgumentParser(description="A python script for logging data sent by a GMT Geiger Counter via the wireless interface. (https://github.com/median-dispersion/GMT-Geiger-Counter)")
    
    # Add arguments
    parser.add_argument("--address",  type=str, required=True,                    help="Address of the GMT Geiger counter.")
    parser.add_argument("--interval", type=int, required=False, default=60,       help="Request interval in seconds. The default is 60 seconds.")
    parser.add_argument("--output",   type=str, required=False, default="./Logs", help="Path to output file or directory. The default output directory is './Logs'.")
    
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
# Get the request address
# =================================================================================================
def getRequestAddress(address):

    # Make address lowercase
    address = address.lower()

    # If the string contains "://" remove everything in front of it
    if "://" in address: address = address.split("://", 1)[1]

    # Add the leading "http://" to the address
    address = f"http://{address}"

    # Return the updated address
    return address

# =================================================================================================
# Request JSON data
# =================================================================================================
def getJSONData(url):

    # Data
    data = {}
    
    # Try fetching data from URL
    try:

        # Print log message
        log("INFO", f"Requesting data from '{url}'...")

        # Make request
        response = requests.get(url, timeout=30)

        # If response code is 200 OK
        if response.status_code == 200:

            # Parse response as JSON
            data = response.json()

            # If data doesn't contain a date add current date in ISO form
            if "date" not in data: data["date"] = datetime.now().astimezone().isoformat()

            # If the requested resource is disabled raise an exception
            if "enabled" in data["data"] and not data["data"]["enabled"]: raise ValueError(f"Requested resource is disabled!")

            # Return data
            return data

        # If the response code is not 200 OK
        else:

            # Raise an exception
            raise ValueError(f"HTTP Response: {response.status_code}")

    # If request fails leave data empty
    except Exception as exception: 
        
        # Print log message
        log("WARNING", f"Requesting data from '{url}' failed! ({exception})")
        
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
            file.write(f"{json.dumps(data, separators=(",", ":"))},")

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

    # Get the request address
    address = getRequestAddress(arguments.address)

    # Get API endpoints
    endpoints = [
        f"{address}/data/geiger-counter",
        f"{address}/data/cosmic-ray-detector",
        f"{address}/data/system"
    ]

    # Handle keyboard interrupts
    try:

        # Main Loop
        while True:

            # For every endpoint
            for endpoint in endpoints:

                # Get JSON data from endpoint
                data = getJSONData(endpoint)
                
                # If requesting the data was successful
                if data != None: 
                    
                    # Print log message
                    log("INFO", f"Received JSON data: '{data}'!")
                    
                    # Write data to log file
                    writeToLogFile(path, data)

                # Wait for 500 ms before performing the next request
                time.sleep(0.5)

            # Print log message
            log("INFO", f"Going to sleep for {arguments.interval} seconds...")

            # Go to sleep until next interval
            time.sleep(arguments.interval)

    # On keyboard interrupt terminate script
    except KeyboardInterrupt: terminate()

# Start the main function
if __name__ == "__main__": main()