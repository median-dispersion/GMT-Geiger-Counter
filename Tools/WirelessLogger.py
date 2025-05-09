#!/usr/bin/env python3

import argparse
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
    parser = argparse.ArgumentParser(description="A script for logging data from the GMT Geiger counter via the wireless interface.")
    
    # Add arguments
    parser.add_argument("--address",  type=str, required=True,              help="Address of the GMT Geiger counter.")
    parser.add_argument("--interval", type=int, required=False, default=60, help="Request interval in seconds.")
    parser.add_argument("--output",   type=str, required=False,             help="Path to the output file.")
    
    # Parse arguments
    return parser.parse_args()

# =================================================================================================
# Get the request address
# =================================================================================================
def getAddress(address):

    # Make address lowercase
    address = address.lower()

    # If the string contains "://" remove everything in front of it
    if "://" in address: address = address.split("://", 1)[1]

    # Add "http://" to the address
    address = f"http://{address}"

    # Return the updated address
    return address

# =================================================================================================
# Get the output file path
# =================================================================================================
def getOutputFile(path):

    # Use specified path or current directory if no output path was provided
    if path is not None: path = Path(path)
    else:                path = Path("./")

    # If path doesn't contain a file use current date and time for the file name
    if path.suffix != ".json": path = path / f"Log_{datetime.now().isoformat()}.json"

    # Create output directory if it doesn't exists
    path.parent.mkdir(parents=True, exist_ok=True)

    # Return the output file path
    return str(path)

# =================================================================================================
# Request JSON data
# =================================================================================================
def getJSONData(url):

    # Data
    data = {}
    
    # Try fetching data from URL
    try:

        # Print info
        print(f"{datetime.now().isoformat()} [INFO] >> Requesting data from '{url}'...")

        # Make request
        response = requests.get(url)

        # If request is successful
        if response.status_code == 200:

            # Parse response as JSON
            data = response.json()

            # Add current date to data
            data["date"] = datetime.now().isoformat()

            # Clear the data if the requested resource is disabled
            if "enabled" in data["data"] and not data["data"]["enabled"]: data = {}

    # If request fails leave data empty
    except Exception as error: 
        
        # Print info
        print(f"{datetime.now().isoformat()} [ERROR] >> Requesting data from '{url}' failed!")

    # Return data
    return data

# =================================================================================================
# Write JSON data to log file
# =================================================================================================
def writeLogFile(data, file):

    # Only write to log file if there is data
    if data:

        # Convert JSON data into a string
        data = json.dumps(data, separators=(",", ":"))

        # Add a trailing ","
        data += ","

        # Append data to the log file
        with open(file, "a") as log: log.write(data)

# =================================================================================================
# Main
# =================================================================================================
def main():

    arguments = getLaunchArguments()            # Get launch arguments
    output    = getOutputFile(arguments.output) # Get output file path
    address   = getAddress(arguments.address)   # Get request address

    # Handle keyboard interrupts
    try:

        # Loop forever
        while True:

            # Get geiger counter data and write it to the log
            writeLogFile(getJSONData(f"{address}/data/geiger-counter"), output)

            # Wait for 500ms before performing the next request
            time.sleep(0.5)

            # Get cosmic ray detector data and write it to the log
            writeLogFile(getJSONData(f"{address}/data/cosmic-ray-detector"), output)

            # Wait for 500ms before performing the next request
            time.sleep(0.5)

            # Get system info data and write it to the log
            writeLogFile(getJSONData(f"{address}/data/system"), output)

            # Print info
            print(f"{datetime.now().isoformat()} [INFO] >> Going to sleep for {arguments.interval} seconds...")

            # Go to sleep until next interval
            time.sleep(arguments.interval)

    # On keyboard interrupt
    except KeyboardInterrupt:

        # Print info
        print(f"{datetime.now().isoformat()} [INFO] >> Keyboard interrupt, quitting!")

        # Quit
        quit()

# Start the main function
if __name__ == "__main__": main()